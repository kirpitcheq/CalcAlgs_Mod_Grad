#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>
#include <KPEq/newtpoly.hpp>
#include <KPEq/integral.hpp>
#include <KPEq/rungeode.hpp>
#include <exception>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calculateBtn_clicked()
{
    static double L = 10; //from spin or TexBox
    static double R_k = 10; //from spin or TexBox
    static double R = 0.4;
    static double l_p = 1; //from spin or TexBox
    static int polypow = 2; //from spin polypow interpollation
    static double z = 0;
    static double T_w = 0;

    static KPEq::Interpoll::SrcNodesType tabledata_T0I;
    static KPEq::Interpoll::NewtPoly newt_T0byI(tabledata_T0I);
    if(newt_T0byI.isWrong())
        throw std::invalid_argument("wrong table data");
    static KPEq::Interpoll::SrcNodesType tabledata_mI;
    static KPEq::Interpoll::NewtPoly newt_mbyI(tabledata_mI);
    if(newt_mbyI.isWrong())
        throw std::invalid_argument("wrong table data");
    static auto T = [](double I, double z)->double{
        auto configErr = newt_T0byI.setConfig(I, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto T0 = newt_T0byI.calc().value();
        configErr = newt_mbyI.setConfig(I, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto m = newt_mbyI.calc().value();
        return (T0 - (T_w - T0) * std::pow(z, m));
    } ;

    static KPEq::Interpoll::SrcNodesType tabledata_SigmT;
    static KPEq::Interpoll::NewtPoly newt_SibmByT(tabledata_SigmT);
    if(newt_SibmByT.isWrong())
        throw std::invalid_argument("wrong table data");
    static auto sigm = [](double I)->double{
        auto configErr = newt_SibmByT.setConfig(T(I,z), polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtByT wrong cnfg");
        return newt_SibmByT.calc().value();
    };
    static auto R_p = [](double I) -> double {
        KPEq::IntegrandFunc integrfunc = [](double I)->double{ return sigm(T(I,z))*z; };
        KPEq::Integral integr(integrfunc, 0, 1);
        return ( l_p / 2 * M_PI * R * R * (integr.calcBySympson(I)) ) ;
    };
    auto di_dt = [](double t, double I, double U) -> double {
        return ( ((U - (R_k + R_p(I))) * I) / L  );
    };
    double modtime = 10;
    double time = 0;
    while(time < modtime){

        //maybe need functional wrapper and some interface for table data for Qchart-newt-QTable
    }
}
