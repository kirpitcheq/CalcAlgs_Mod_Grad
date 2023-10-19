#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QTableWidget>
#include <QMainWindow>
#include <vector>
#include <QChart>
#include <QChartView>
#include <QScatterSeries>
#include <QLineSeries>
#include <QStandardItemModel>
#include "../lw1_lib/modules/newtpoly.hpp"
#include "../lw1_lib/modules/newtpolyroots.hpp"
#include "../lw1_lib/modules/sysequasroot.hpp"
#include "../lw1_libQt/MsgBoxSmpl/msgboxsmpl.h"
#include "../lw1_libQt/Chart/chart.h"
#include <memory>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_calcBtn_clicked();

    void on_nodesCountSpinB_valueChanged(int arg1);

    void on_calcsysequarootBtn_clicked();

    void on_loadExBtn_clicked();

private:
    QChart *qchart;
    Chart *chart;
    QChartView *qchartview;
    QLineSeries *qlineseries;
    Ui::MainWindow *ui;

    using PairTables = std::pair<InterpKPEq::SrcNodesType,InterpKPEq::SrcNodesType>;
    using PairTablesVect = std::vector<PairTables>;
    InterpKPEq::SrcNodesType vect = {{3,4},{4,6}};
    InterpKPEq::XYNodes node = {2,2};
    PairTablesVect exmplvect = {
        {
            {{-0.5,0.707}, {-0.25, 0.924}, {0.0, 1.0}, {0.25,0.924}, {0.5,0.707},{0.75,0.383},{1.0,0}},
            { {0,0} }
        },
        {
            {
             {0.137,0.005},
             {0.343,0.035},
             {0.421,0.055},
             {0.454,0.065},
             {0.626,0.135},
             {0.664,0.155},
             {0.731,0.195},
             {0.843,0.275},
             {0.891,0.315},
             {0.985,0.405}
            },
            {
             {0.1,-0.284},
             {0.3,-0.3},
             {0.4,-0.293},
             {0.48,-0.266},
             {0.6,-0.165},
             {0.68,-0.037},
             {0.75,0.128},
             {0.88,0.610},
             {0.95,0.988},
             {1.03,1.547},
            }
        },
        {
            {
                {0.00,1.000000},
                {0.15,0.838771},
                {0.30,0.655336},
                {0.45,0.450447},
                {0.60,0.225336},
                {0.75,-0.018310},
                {0.90,-0.278390},
                {1.05,-0.552430},
            },
            { {0,0} }
        }
    };

    int setTableByExample(const PairTablesVect & extables, int curEx, QTableWidget* qtable1, QTableWidget* qtable2){
        auto table1 = extables[curEx].first;
        auto table2 = extables[curEx].second;

        qtable1->setRowCount(table1.size());
        qtable2->setRowCount(table2.size());

        int row ;
        row = 0;
        for(auto i : table1){
            qtable1->setItem(row, 0, new QTableWidgetItem(QString::number(i.first)));
            qtable1->setItem(row, 1, new QTableWidgetItem(QString::number(i.second)));
            row++;
        }
        row = 0;
        for(auto i : table2){
            qtable2->setItem(row, 0, new QTableWidgetItem(QString::number(i.first)));
            qtable2->setItem(row, 1, new QTableWidgetItem(QString::number(i.second)));
            row++;
        }
        return 1;
    }

    struct VectNSers{
    public:
        QPointer<QLineSeries> series = new QLineSeries;
//        QLineSeries series;
        InterpKPEq::SrcNodesType vectnodes;
    };

//    const VectNSers& getDataFrom(const QTableWidget & nodesTable, VectNSers&& result = VectNSers()) {
    VectNSers getDataFrom(const QTableWidget & nodesTable){
        using namespace InterpKPEq;

        VectNSers result;
        auto tablerows = nodesTable.rowCount();
//        QLineSeries series;
//        VectNSers result;
        for (int row = 0; row < tablerows; ++row) {
            auto xitem = nodesTable.item(row,0);
            auto yitem = nodesTable.item(row,1);
            if(xitem == nullptr || yitem == nullptr){
//                MsgBoxSmpl("Fill in all the cells of the table!");
//                delete series;
//                return VectNSers();
                throw "Fill in all the cells of the table!";
            }
            bool isdoubleX = false, isdoubleY = false;
            auto xvalue = xitem->data(Qt::DisplayRole).toDouble(&isdoubleX);
            auto yvalue = yitem->data(Qt::DisplayRole).toDouble(&isdoubleY);
            if(!isdoubleX || !isdoubleY)
            {
//                MsgBoxSmpl("Can't recognise value in cells on the row: " + QString::number(row+1));
                throw ("Can't recognise value in cells on the row: " + QString::number(row+1));
            }
            result.series->append(xvalue,yvalue);
            result.vectnodes.push_back(XYNodes(xvalue,yvalue));//here pairs and qchart converts
        }
        return result;
    }



};
#endif // MAINWINDOW_H
