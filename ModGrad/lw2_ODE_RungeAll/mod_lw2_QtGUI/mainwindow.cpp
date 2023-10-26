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

    static double R = 0.35;
    static double l_p = 12; //sm -> m
    static double L_k = 187; //from spin or TexBox
    static double C_k = 0.000268;
    static double R_k = 0.25; //from spin or TexBox
    static double T_w = 2000;
    double U_co = 1400;
    double I_0 = 0.3;

    /* need set row names */
    /* for input values need map data table (string values with row names for use this further */
    KPEq::Q::Tcont init_data = {
        {R},
        {l_p},
        {L_k},
        {C_k},
        {R_k},
        {U_co},
        {I_0},
        {T_w},
    };

    this->initdata_model = new KPEq::Q::ItemModel(init_data); //here need ref from model for data access
    ui->tableV_InitVals->setModel(initdata_model);

    KPEq::Q::Tcont I_T0_m_data = {
        { 0.5, 	6730, 0.50 	},
        { 1, 	6790, 0.55 	},
        { 5, 	7150, 1.7	},
        { 10, 	7270, 3		},
        { 50,	8010, 11	},
        { 200, 	9185, 32	},
        { 400, 10010, 40	},
        { 800, 11140, 41	},
        { 1200,12010, 39	}
    };

    KPEq::Q::Tcont Tk_sigma = {
        {4000, 0.031},
        {5000, 0.27 },
        {6000, 2.05 },
        {7000, 6.06 },
        {8000, 12.0 },
        {9000, 19.9 },
        {10000, 29.6},
        {11000, 41.1},
        {12000, 54.1},
        {13000, 67.7},
        {14000, 81.5}
    };
}


void MainWindow::on_calculateBtn_clicked()
{
    /* init values */
    static double L = 10; //from spin or TexBox
    double I = 0.3;
    double U = 1400;

    static int polypow = 2; //from spin polypow interpollation
    static double integr_step = 0.05;

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
