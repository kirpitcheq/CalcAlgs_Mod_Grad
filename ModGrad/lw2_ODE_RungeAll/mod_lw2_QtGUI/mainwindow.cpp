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
    /* init values */
    double I = 0.3;
    double U = 1400;
    static double L = 10; //from spin or TexBox
    static double R_k = 10; //from spin or TexBox
    static double R = 0.4;
    static double l_p = 1; //from spin or TexBox
    static int polypow = 2; //from spin polypow interpollation
    static double T_w = 0;
    static double integr_step = 0.05;
    static double C_k = 0.000268;
    /* ------------- */

    /* this need set by common table.. but count of values difference! */
    /* is need caching values ? */
    static KPEq::Interpoll::SrcNodesType tabledata_T0I;
    static KPEq::Interpoll::SrcNodesType tabledata_mI;
    static KPEq::Interpoll::SrcNodesType tabledata_SigmT;

    /* init interpollation objects */
    static KPEq::Interpoll::NewtPoly newt_T0byI(tabledata_T0I); // interface need but full rework class?
    if(newt_T0byI.isWrong())
        throw std::invalid_argument("wrong table data");
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

    static KPEq::Interpoll::NewtPoly newt_SigmByT(tabledata_SigmT);
    if(newt_SigmByT.isWrong())
        throw std::invalid_argument("wrong table data");
    static auto sigm = [](double T)->double{
        auto configErr = newt_SigmByT.setConfig(T, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtByT wrong cnfg");
        return newt_SigmByT.calc().value();
    };

    static auto R_p = [](double I) -> double {
        KPEq::IntegrandFunc integrfunc = [I](double z)->double{ return sigm(T(I,z))*z; };
        KPEq::Integral integr(integrfunc, 0, 1);
        return ( l_p / 2 * M_PI * R * R * (integr.calcBySympson(integr_step)));
    };

    auto di_dt = [](double t, double I, double U) -> double {
        return ( ((U - (R_k + R_p(I))) * I) / L  );
    };
    auto du_dt = [](double t, double I, double U) -> double {
        return ( -I / C_k );
    };

    double modtime = 10;
    double time = 0;
    double modStep = 0.1;

    KPEq::ODE::ODESysEqua odeSysEqua(di_dt, du_dt, modStep);
    while(time < modtime){
        std::tie(I,U) = odeSysEqua.calcNextByRunge2(time, I, U); //here need switch to choise Runge, need realize Runge1
        /* set data to table */
    }
}
