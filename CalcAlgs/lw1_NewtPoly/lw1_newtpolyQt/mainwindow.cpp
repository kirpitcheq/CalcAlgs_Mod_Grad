#include "mainwindow.h"
#include "./ui_mainwindow.h"

#define DARK_THEME_SET

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    chart = new Chart;

    qchartview = new QChartView(chart, this);
    qchartview->setRenderHints(QPainter::Antialiasing);
    ui->qchartForm->addWidget(qchartview);

#ifdef linux
#ifdef DARK_THEME_SET
    QPalette pal = window()->palette();
    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    window()->setPalette(pal);

#endif
#endif

    /* limit example spin box value */
    ui->examplenSpin->setMaximum(exmplvect.size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_calcBtn_clicked()
{
    using namespace std;
    using namespace InterpKPEq;
    //here need set additional function for reverse data and turn it into NewtPoly class with name NewtPoly::findRoots(args)(static meth)

    //from table to qchart, than calculated point must be push as bold point to qchart
    auto data = this->getDataFrom(*ui->nodesTable);
    auto vect = data.vectnodes;
    chart->updateSeries(data.series);

    //and need calculate value ! than need values from table translate to vect
    //for qtable to push pair to vector

    NewtPoly newtpoly(vect);
    if(newtpoly.isWrong()){
        MsgBoxSmpl("Wrong table values");
        return;
    }

    auto xtarget = ui->xTargetSpinB->value();
    auto polypow = ui->polyPowSpinB->value(); //must be more than 0 and less then nodes amount
    auto configAns = newtpoly.setConfig(xtarget, polypow);//here need tests! for good xtarget but wrong polypow
    if(configAns < 0) { //here polypow must be more than 1 AND less than nodes!!!, value may be extrapollation
        MsgBoxSmpl("Wrong config values");
        return;
    }
    else if(configAns == 2) //extrapollation!
        MsgBoxSmpl("Extrapollation xTargetValue!");

    auto desValueY = newtpoly.calc();
    if(desValueY == std::nullopt){
        MsgBoxSmpl("Can't calc this value");
        return;
    }
    //here need add (push) desValueY to serials for qchart

    auto despoint = QPointF(xtarget, desValueY.value());
    chart->addPointToNewSeries(despoint);


    auto resstr = "Calculated value is: " + QString::number(desValueY.value()) + "\n";

    /*View result work table*/
    //need delete old values if have
    auto *model = new QStandardItemModel(ui->workTable);// CHECK THIS PARRENT DELETER

    auto worktablearr = newtpoly.getLastWorkTable();
    for(auto row = 0; row < newtpoly.getWorkTblRows(); row ++)
        for(auto col = 0; col < newtpoly.getWorkTblCols(); col ++)
            model->setItem(row,col,new QStandardItem(QString::number(worktablearr[row][col])));

    ui->workTable->setModel(model);
    ui->workTable->resizeRowsToContents();
    ui->workTable->resizeColumnsToContents();

#define ROOTS
#ifdef ROOTS
    NewtPolyRoots newtpolyroots(vect,polypow);//here need disable extrapollation
    if(newtpolyroots.isWrong()){
        MsgBoxSmpl("Wrong calculate root");
        return;
    }
    auto rootvalue = newtpolyroots.getRoot();//is need check?
    auto rootpoint = QPointF(rootvalue.value(),0);
    chart->addPointToNewSeries(rootpoint);

    resstr+=QString("Root is: ( %1 ; 0 ) \n(displayed as a dot on the graph)").arg(QString::number(rootvalue.value()));
#endif
    ui->resultTextBrowser->setText(resstr); //here result to text browser
}


void MainWindow::on_nodesCountSpinB_valueChanged(int arg1) {
//    auto tablecols = ui->nodesTable->columnCount();
    ui->nodesTable->setRowCount(arg1);
}


void MainWindow::on_calcsysequarootBtn_clicked()
{
    using namespace std;
    using namespace InterpKPEq;
    //here need set additional function for reverse data and turn it into NewtPoly class with name NewtPoly::findRoots(args)(static meth)

    auto table1data = getDataFrom(*ui->nodesTable);
    auto table2data = getDataFrom(*ui->nodes2Table);

    chart->updateSeries(table1data.series);
    chart->addSeries(table2data.series);
    chart->createDefaultAxes();

    SysEquasRoot seroot(table1data.vectnodes, table2data.vectnodes,ui->sysauxtSpinB->value(), ui->sysrootPolyPowSpinB->value());
    seroot.calcAuxTable();
    seroot.calcResTable();
    seroot.findRoot();

    auto auxtabledata = seroot.getAuxTable();
    auto restabledata = seroot.getResTable();
    auto rootvalue = seroot.getRootVal();


    QPointer<QLineSeries> auxseries = new QLineSeries();
    QPointer<QLineSeries> resseries = new QLineSeries();
    for(auto i : auxtabledata){
        auxseries->append(i.first, i.second);
    }
    chart->addSeries(auxseries);
    chart->createDefaultAxes();
    for(auto i : restabledata){
        resseries->append(i.first, i.second);
    }
    chart->addSeries(resseries);
    chart->createDefaultAxes();

    /*View result work table*/
    //need delete old values if have
    auto *modelAux = new QStandardItemModel(ui->tableAuxView);// CHECK THIS PARRENT DELETER
    auto *modelRes = new QStandardItemModel(ui->tableResView);// CHECK THIS PARRENT DELETER

    int row = 0;
    for(auto i : auxtabledata){
        modelAux->setItem(row,0,new QStandardItem(QString::number(i.first)));
        modelAux->setItem(row++,1,new QStandardItem(QString::number(i.second)));
    }
    ui->tableAuxView->setModel(modelAux);
    ui->tableAuxView->resizeRowsToContents();
    ui->tableAuxView->resizeColumnsToContents();

    row = 0;
    for(auto i : restabledata){
        modelRes->setItem(row,0,new QStandardItem(QString::number(i.first)));
        modelRes->setItem(row++,1,new QStandardItem(QString::number(i.second)));
    }

    ui->tableResView->setModel(modelRes);
    ui->tableResView->resizeRowsToContents();
    ui->tableResView->resizeColumnsToContents();

    NewtPoly newtpoly(table1data.vectnodes);
    if(newtpoly.isWrong()){
        MsgBoxSmpl("Wrong table values");
        return;
    }

    auto configAns = newtpoly.setConfig(rootvalue, ui->sysrootPolyPowSpinB->value());//here need tests! for good xtarget but wrong polypow
    if(configAns < 0) { //here polypow must be more than 1 AND less than nodes!!!, value may be extrapollation
        MsgBoxSmpl("Wrong config values");
        return;
    }
    else if(configAns == 2) //extrapollation!
        MsgBoxSmpl("Extrapollation xTargetValue!");

    auto desValueY = newtpoly.calc();
    if(desValueY == std::nullopt){
        MsgBoxSmpl("Can't calc this value");
        return;
    }
    //here need add (push) desValueY to serials for qchart

    auto despoint = QPointF(rootvalue, desValueY.value());
    chart->addPointToNewSeries(despoint);

    auto resstr = QString("Calculated value is: ( %1 : %2 ) \n(displayed as a dot on the graph)").arg(rootvalue).arg(desValueY.value());
    ui->resultTextBrowser_2->setText(resstr); //here result to text browser
}

void MainWindow::on_loadExBtn_clicked()
{
    auto curEx = ui->examplenSpin->value()-1;

    if(! setTableByExample(exmplvect, curEx, ui->nodesTable, ui->nodes2Table))
        MsgBoxSmpl("Can't load example");
//    ui->nodesTable->setItem()
}

