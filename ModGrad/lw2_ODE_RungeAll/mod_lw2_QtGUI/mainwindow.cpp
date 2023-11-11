#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "lw2task.h"
#include <KPEq/integral.hpp>
#include <KPEq/newtpoly.hpp>
#include <KPEq/rungeode.hpp>
#include <QVXYModelMapper>
#include <cmath>
#include <exception>
#include <fmt/core.h>
#include <memory>
#include <numbers>
#include <KPEq/msgboxsmpl.h>
// #include <thread>

//#define KPEqLib_DEBUG

constexpr double R = 0.35;
constexpr double l_p = 12;     // sm -> m
constexpr double L_k = 187e-6; // from spin or TexBox
constexpr double C_k = 268e-6;
constexpr double R_k = 0.25; // from spin or TexBox
constexpr double U_co = 1400;
constexpr double I_0 = 0;
constexpr double T_w = 2000;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);


  KPEq::Q::Tcont init_data = {
      {R}, {l_p}, {L_k}, {C_k}, {R_k}, {U_co}, {I_0}, {T_w},
  };

  this->initdata_model = new KPEq::Q::ItemModel(
      init_data); // here need ref from model for data access
  initdata_model->setHeaderData(0, Qt::Orientation::Horizontal, "InitVars");
  initdata_model->setHeaderData(0, Qt::Orientation::Vertical, "R");
  initdata_model->setHeaderData(1, Qt::Orientation::Vertical, "l_p");
  initdata_model->setHeaderData(2, Qt::Orientation::Vertical, "L_k");
  initdata_model->setHeaderData(3, Qt::Orientation::Vertical, "C_k");
  initdata_model->setHeaderData(4, Qt::Orientation::Vertical, "R_k");
  initdata_model->setHeaderData(5, Qt::Orientation::Vertical, "U_co");
  initdata_model->setHeaderData(6, Qt::Orientation::Vertical, "I_0");
  initdata_model->setHeaderData(7, Qt::Orientation::Vertical, "T_w");
  ui->tableV_InitVals->setModel(initdata_model);

  KPEq::Q::Tcont I_T0_m_data = {
      {0.5, 6730, 0.50}, {1, 6790, 0.55},  {5, 7150, 1.7},
      {10, 7270, 3},     {50, 8010, 11},   {200, 9185, 32},
      {400, 10010, 40},  {800, 11140, 41}, {1200, 12010, 39}};

  this->I_T0_m_data_model = new KPEq::Q::ItemModel(
      I_T0_m_data); // here need ref from model for data access
  I_T0_m_data_model->setHeaderData(0, Qt::Orientation::Horizontal, "I");
  I_T0_m_data_model->setHeaderData(1, Qt::Orientation::Horizontal, "T0");
  I_T0_m_data_model->setHeaderData(2, Qt::Orientation::Horizontal, "m");
  ui->tableV_I_T0_m->setModel(I_T0_m_data_model);

  KPEq::Q::Tcont Tk_sigma = {//        {2000, 0.001},
                             {4000, 0.031}, {5000, 0.27},  {6000, 2.05},
                             {7000, 6.06},  {8000, 12.0},  {9000, 19.9},
                             {10000, 29.6}, {11000, 41.1}, {12000, 54.1},
                             {13000, 67.7}, {14000, 81.5}};

  this->Tk_sigma_model = new KPEq::Q::ItemModel(
      Tk_sigma); // here need ref from model for data access
  Tk_sigma_model->setHeaderData(0, Qt::Orientation::Horizontal, "T");
  Tk_sigma_model->setHeaderData(1, Qt::Orientation::Horizontal, "𝜎");
  ui->tableV_Sigma_T->setModel(Tk_sigma_model);

  /* set geometry of items of all tables */
  constexpr int INIT_COL_SIZE = 50;
  constexpr int INIT_ROW_SIZE = 10;
  QVector<QTableView *> tables;
  tables << ui->tableV_InitVals << ui->tableV_I_T0_m << ui->tableV_Sigma_T;
  for (auto tble : tables) {
    auto cols = tble->model()->columnCount();
    for (auto col = 0; col < cols; col++) {
      tble->setColumnWidth(col, INIT_COL_SIZE);
    }
    auto rows = tble->model()->rowCount();
    for (auto row = 0; row < rows; row++) {
      tble->setRowHeight(row, INIT_ROW_SIZE);
    }
  }
  ui->tableV_InitVals->setColumnWidth(0, 70);

  //            << ui->tableV_OutData

  /* chart init*/
  this->chart = new KPEq::Chart;
  chart->legend()->show();
  ui->graphicsView->setChart(this->chart);

  /* set results model tables for tableViews */
  results_model = new KPEq::Q::ItemModel(this);
  ui->tableV_OutData->setModel(results_model);
  resultsRK2_model = new KPEq::Q::ItemModel(this);
  ui->tableV_OutData_RK2->setModel(resultsRK2_model);
  resultsRK4_model = new KPEq::Q::ItemModel(this);
  ui->tableV_OutData_RK4->setModel(resultsRK4_model);

  resultsRkRpNull_model= new KPEq::Q::ItemModel(this);
  ui->tableV_OutData_RkRpNull->setModel(resultsRkRpNull_model);
  resultsRk200_model= new KPEq::Q::ItemModel(this);
  ui->tableV_OutData_Rk200->setModel(resultsRk200_model);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_calculateBtn_clicked() {

  const auto R_k_now = (*initdata_model)[4][0]; // reset if 4 tab was selected
  if(R_k_now == 200 && ui->tabWidget->currentIndex() != 4)
    initdata_model->setData(initdata_model->index(4,0),R_k);
//    (*initdata_model)[4][0] = R_k;
  int polypow = ui->polypowSpin->value();
  double integr_step = ui->integstepSpin->value();
  KPEq::Interpoll::NewtCnfgEnumC newtsmode =
      KPEq::Interpoll::NewtCnfgEnumC::NORMAL;


  double modTime = ui->modtimeSpin->value();
  double modStep = ui->modstepSpin->value();
  chart->removeAllSeries();
  using namespace KPEq::ODE;
  //    std::mutex mtx;
  //    std::thread lw2thr_rk1(&LW2Task::calcODE,  &lw2task1,modTime, modStep,
  //    "RK1",
  //                           std::ref(*results_model), std::ref(*chart),
  //                           std::ref(mtx), &ODESysEqua::calcNextByRunge1);
  //    std::thread lw2thr_rk2(&LW2Task::calcODE,  &lw2task2,modTime, modStep,
  //    "RK2",
  //                           std::ref(*resultsRK2_model), std::ref(*chart),
  //                           std::ref(mtx),  &ODESysEqua::calcNextByRunge2);
  //    std::thread lw2thr_rk4(&LW2Task::calcODE,  &lw2task4,modTime, modStep,
  //    "RK4",
  //                           std::ref(*resultsRK4_model),std::ref(*chart),
  //                           std::ref(mtx),  &ODESysEqua::calcNextByRunge4);
  //    lw2thr_rk1.join();
  //    lw2thr_rk2.join();
  //    lw2thr_rk4.join();
  auto rktab = ui->tabWidget->currentIndex();
  switch (rktab)
  {
  case 0:
  {
    LW2Task lw2task1(polypow, integr_step, newtsmode, *initdata_model,
                     *I_T0_m_data_model, *I_T0_m_data_model, *Tk_sigma_model);
    lw2task1.calcODE(modTime, modStep, "RK1", *results_model, *chart,
                     &ODESysEqua::calcNextByRunge1);
  }
  break;
  case 1:
  {
    LW2Task lw2task2(polypow, integr_step, newtsmode, *initdata_model,
                     *I_T0_m_data_model, *I_T0_m_data_model, *Tk_sigma_model);
    lw2task2.calcODE(modTime, modStep, "RK2", *resultsRK2_model, *chart,
                     &ODESysEqua::calcNextByRunge2);
  }
  break;
  case 2:
  {
    LW2Task lw2task4(polypow, integr_step, newtsmode, *initdata_model,
                     *I_T0_m_data_model, *I_T0_m_data_model, *Tk_sigma_model);
    lw2task4.calcODE(modTime, modStep, "RK4", *resultsRK4_model, *chart,
                     &ODESysEqua::calcNextByRunge4);
  }
  break;
  case 3:
  {
    constexpr bool RkRpNull = true;
    LW2Task lw2taskRpRkNull(polypow, integr_step, newtsmode, *initdata_model,
                            *I_T0_m_data_model, *I_T0_m_data_model, *Tk_sigma_model);
    lw2taskRpRkNull.calcODE(modTime, modStep, "RpRkNull", *resultsRkRpNull_model, *chart,
                            &ODESysEqua::calcNextByRunge4, RkRpNull);
  }
  break;
  case 4:
  {
//    (*initdata_model)[4][0] = 200; // R_k
    initdata_model->setData(initdata_model->index(4,0),200);
    LW2Task lw2taskRp200(polypow, integr_step, newtsmode, *initdata_model,
                         *I_T0_m_data_model, *I_T0_m_data_model, *Tk_sigma_model);
    lw2taskRp200.calcODE(modTime, modStep, "Rk200", *resultsRk200_model, *chart,
                         &ODESysEqua::calcNextByRunge4);
  }
  break;
  default:
    MsgBoxSmpl("Select tab which contain method name used for calculating ODE");
    return;
  }
  chart->createDefaultAxes();
  constexpr int INIT_COL_SIZE = 70;
  constexpr int INIT_ROW_SIZE = 10;
  QVector<QTableView *> results_tables_vect;
  results_tables_vect
      << ui->tableV_OutData
      << ui->tableV_OutData_RK2
      << ui->tableV_OutData_RK4
      << ui->tableV_OutData_RkRpNull
      << ui->tableV_OutData_Rk200;
  for (auto tble : results_tables_vect) {
    auto cols = tble->model()->columnCount();
    for (auto col = 0; col < cols; col++) {
      tble->setColumnWidth(col, INIT_COL_SIZE);
    }
    auto rows = tble->model()->rowCount();
    for (auto row = 0; row < rows; row++) {
      tble->setRowHeight(row, INIT_ROW_SIZE);
    }
  }
  chart->createDefaultAxes();
}
