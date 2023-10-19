#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QStandardItemModel>
#include <QString>
#include <KPEq/msgboxsmpl.h>
#include <string>
#include <exception>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    chart = new KPEq::Chart;
    ui->graphicsView->setMinimumSize(QSize(0,250));
    ui->graphicsView->setChart(chart);
//    KPEq::setQPalette(window(), KPEq::ThemeQ::Black);
    //all in one need without div, only include just what need (into one lib several .cpp and .h files)
}

MainWindow::~MainWindow() {
    delete ui;
}


bool KPEq::findIntervalValues(RPN_KP &expression, double interval_start, double interval_end, double step, list<pair<double, double> > &out_vals, bool stepByX, ODE_KPeQ::TwinVectT refCoords)
{
    double temp;
    if (!step)
        return false;
    double i;
    if(refCoords.empty() == false){
        for(auto [x,y] : refCoords){
            if(stepByX){
                if (!expression.setX(x))
                    return false;
            }
            else{
                if (!expression.setY(y))
                    return false;
            }
            expression.calculate(temp);
            auto pairval = stepByX == true ? pair<double,double>(x, temp) : pair<double,double>(temp, y);
            out_vals.push_back(pairval);
        }
    }
    else{
        for (i = interval_start; i <= interval_end; i += step)
        {
            if(stepByX){
                if (!expression.setX(i))
                    return false;
            }
            else{
                if (!expression.setY(i))
                    return false;
            }
            expression.calculate(temp);
            auto pairval = stepByX ? pair<double,double>(i, temp) : pair<double,double>(temp, i);
            out_vals.push_back(pairval);
        }
        // i += step;
        if (i - step != interval_end)
        {
            if(stepByX){
                if (!expression.setX(interval_end))
                    return false;
            }
            else{
                if (!expression.setY(interval_end))
                    return false;
            }
            expression.calculate(temp);
            auto pairval = stepByX ? pair<double,double>(i, temp) : pair<double,double>(temp, i);
            out_vals.push_back(pairval);
        }
    }
    //здесь добавить условие деления нацело и в случае false печать конечных значений самостоятельно вне стека
    return true;
}
void MainWindow::on_calcBtn_clicked()
{
    try
    {
    /* derevexpr */
    auto derivexpr = ui->derivativeTEdit->toPlainText();
    if(derivexpr.isEmpty()){
        // MsgBoxSmpl("Empty clear derivative expression");
        // return;
    }
    auto x0 = ui->x0Spin->value();
    auto y0 = ui->y0Spin->value();
    auto xmax = ui->xmaxSpin->value();
    auto step = ui->stepSpin->value();
    ODE_KPeQ::TwinVectT pointsVect;
    std::vector<double> stepVect;
    try{
        if(ui->autoStepCheck->isChecked()){
                pointsVect = ODE_KPeQ::EilCalc(x0,y0,step, xmax, derivexpr.toStdString(), ui->stepSpin->value(), &stepVect);
        }
        else{
            pointsVect = ODE_KPeQ::EilCalc(x0,y0,step, xmax, derivexpr.toStdString());
        }
    }
    catch(const std::exception & ex){
        MsgBoxSmpl(QString::fromStdString("Derivative expression: " + string(ex.what())));
        return;
    }
    chart->updateSeries(pointsVect);

    //if 'x' - exit
    /* analyt expression */
    auto analytexpr= ui->analytTEdit->toPlainText();
    ODE_KPeQ::TwinVectT analytpoints;
    if(!analytexpr.isEmpty()){
        auto values_list = std::list<std::pair<double,double>>();
        auto analytstdstr = analytexpr.toStdString();
        RPN_KP rpnexpr(analytstdstr);
        bool stepByX = ui->xyChoiseBox->currentIndex() == 0 ? true : false;
        /* below maybe with x or with y and pointVect need as reference y values if x */
        auto hasResult = KPEq::findIntervalValues(rpnexpr, x0, xmax, step, values_list, stepByX, pointsVect);
        //SIMPLE EXPRESSION AND NEED SELECT X OR Y THIS
        //need check with only by one value (maybe check with rpnexpr)
        ODE_KPeQ::TwinVectT analytPointsVect(std::make_move_iterator(std::begin(values_list)), std::make_move_iterator(std::end(values_list)));
        chart->addVectSeries(analytPointsVect);
        analytpoints = analytPointsVect;
    }

    std::vector<ODE_KPeQ::TwinVectT> approxsVectsVector;
    auto approxexount = ui->analytAprxTableIn->rowCount();
    for(auto row = 0; row < approxexount; row++){
        auto values_list = std::list<std::pair<double,double>>();
        auto approxexpritem = ui->analytAprxTableIn->item(row, 1);
        QString approxexpr;
        if(approxexpritem == NULL || (approxexpr = approxexpritem->text()).isEmpty())
            continue;
        RPN_KP rpnexpr(approxexpr.toStdString());
        KPEq::findIntervalValues(rpnexpr, x0, xmax, step, values_list); //maybe with x or with y =
        ODE_KPeQ::TwinVectT vectorFromList(std::make_move_iterator(std::begin(values_list)), std::make_move_iterator(std::end(values_list)));
        approxsVectsVector.push_back(vectorFromList);
    }

    auto model = new QStandardItemModel(ui->tableView);
    int row = 0;
#ifdef TRANSFER_DEREV_VECT_TO_MODEL
    /*table view model set*/
    for(auto [x,y] : pointsVect){
        model->setItem(row,0, new QStandardItem(QString::number(x)));
        model->setItem(row,1, new QStandardItem(QString::number(y)));
        row++;
    }
#endif
    row = 0;
    auto col = 2;
    for(auto [x,y] : analytpoints){ //coord may be x or y!!!
        //list for y or x to ui need
//        if(...) here x or y main in analyt expr
        model->setItem(row,col, new QStandardItem(QString::number(x))); //how common x if main y on the analyt expr ???
        model->setItem(row,col+1, new QStandardItem(QString::number(y))); //how common x if main y on the analyt expr ???
        row++;
    }
    col+=2;
    row = 0;
    for(auto approxVect : approxsVectsVector){
        chart->addVectSeries(approxVect);
        for(auto [x,y] : approxVect){ //coord may be x or y!!!
            auto coord  = y;
            model->setItem(row,col, new QStandardItem(QString::number(coord))); //how common x if main y on the analyt expr ???
            row++;
        }
        col++; // ?? not see 1st coords of y (index 3)
    }

    ui->tableView->setModel(model);

    chart->createDefaultAxes();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}


void MainWindow::on_decimalsCountSpin_valueChanged(int arg1)
{
    ui->stepSpin->setDecimals(arg1);
    ui->x0Spin->setDecimals(arg1);
    ui->y0Spin->setDecimals(arg1);
}


void MainWindow::on_analytAprxSpin_valueChanged(int arg1)
{
    ui->analytAprxTableIn->setRowCount(arg1);
    for(int row = 0; row < arg1; row++) {
        ui->analytAprxTableIn->setItem(row, 0, new QTableWidgetItem(QString("%1 approx:").arg(row+1)));
    }
}

