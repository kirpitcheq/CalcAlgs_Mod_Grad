#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>
#include <KPEq/newtpoly.hpp>
#include <KPEq/integral.hpp>
#include <KPEq/rungeode.hpp>
#include <exception>
#include <fmt/core.h>
#include <cmath>
#include <numbers>
#include <QVXYModelMapper>
#include <memory>

//#define MOD_LW2_DEBUG
#ifdef MOD_LW2_DEBUG
    #define PRINTTB(VAL) fmt::print("{} {}\t",#VAL,VAL)
    #define PRINTLN(VAL) fmt::print("{} {}\n",#VAL,VAL)
    #define PRINTNEWLN() fmt::print("\n")
#else
    #define PRINTTB(VAL) ;
    #define PRINTLN(VAL) ;
    #define PRINTNEWLN() ;
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    double R = 0.35;
    double l_p = 12; //sm -> m
    double L_k = 187e-6; //from spin or TexBox
    double C_k = 268e-6;
    double R_k = 0.25; //from spin or TexBox
    double U_co = 1400;
    double I_0 = 0;
    double T_w = 2000;

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
    initdata_model->setHeaderData(0, Qt::Orientation::Horizontal, "InitVars");
    initdata_model->setHeaderData(0, Qt::Orientation::Vertical,  "R");
    initdata_model->setHeaderData(1, Qt::Orientation::Vertical,  "l_p");
    initdata_model->setHeaderData(2, Qt::Orientation::Vertical,  "L_k");
    initdata_model->setHeaderData(3, Qt::Orientation::Vertical,  "C_k");
    initdata_model->setHeaderData(4, Qt::Orientation::Vertical,  "R_k");
    initdata_model->setHeaderData(5, Qt::Orientation::Vertical,  "U_co");
    initdata_model->setHeaderData(6, Qt::Orientation::Vertical,  "I_0");
    initdata_model->setHeaderData(7, Qt::Orientation::Vertical,  "T_w");
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

    this->I_T0_m_data_model = new KPEq::Q::ItemModel(I_T0_m_data); //here need ref from model for data access
    I_T0_m_data_model->setHeaderData(0, Qt::Orientation::Horizontal, "I");
    I_T0_m_data_model->setHeaderData(1, Qt::Orientation::Horizontal, "T0");
    I_T0_m_data_model->setHeaderData(2, Qt::Orientation::Horizontal, "m");
    ui->tableV_I_T0_m->setModel(I_T0_m_data_model);

    KPEq::Q::Tcont Tk_sigma = {
//        {2000, 0.001},
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

    this->Tk_sigma_model = new KPEq::Q::ItemModel(Tk_sigma); //here need ref from model for data access
    Tk_sigma_model->setHeaderData(0, Qt::Orientation::Horizontal, "T");
    Tk_sigma_model->setHeaderData(1, Qt::Orientation::Horizontal, "𝜎");
    ui->tableV_Sigma_T->setModel(Tk_sigma_model);

    /* set geometry of items of all tables */
    QVector<QTableView*> tables;
    tables  << ui->tableV_InitVals
            << ui->tableV_I_T0_m
            << ui->tableV_OutData
            << ui->tableV_Sigma_T;
    for(auto tble : tables)
    {
    }

    /* chart init*/
    this->chart = new KPEq::Chart;
    chart->legend()->show();
    ui->graphicsView->setChart(this->chart);

    results_model = new KPEq::Q::ItemModel(this);
    ui->tableV_OutData->setModel(results_model); // тут по идее м.б. замедление т.к. данные не передаются... они копируются
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calculateBtn_clicked()
{
    bool isCachable = true;
    double R   = (*initdata_model)[0][0];//Tk_sigma_model[0][1];
    double l_p = (*initdata_model)[1][0]; //sm -> m
    double L_k = (*initdata_model)[2][0]; //from spin or TexBox
    double C_k = (*initdata_model)[3][0];
    double R_k = (*initdata_model)[4][0]; //from spin or TexBox
    double U_co= (*initdata_model)[5][0];
    double I_0 = (*initdata_model)[6][0];
    double T_w = (*initdata_model)[7][0];
    /* init values */
    double I = I_0;
    double U = U_co;

    int polypow = ui->polypowSpin->value(); //from spin polypow interpollation
    double integr_step = ui->integstepSpin->value();
    /* ----------------------------------------------------------------- */

    /* this need set by common table.. but count of values difference! */
    /* is need caching values ? */

    KPEq::Interpoll::SrcNodesType tabledata_T0I;
    KPEq::Interpoll::SrcNodesType tabledata_mI;
    for(auto i = 0; i < this->I_T0_m_data_model->rowCount(); i++){
        auto I  = (*I_T0_m_data_model)[i][0];
        auto T0 = (*I_T0_m_data_model)[i][1];
        auto m  = (*I_T0_m_data_model)[i][2];
        tabledata_T0I.push_back	({I,T0});
        tabledata_mI.push_back	({I,m});
    }
    KPEq::Interpoll::SrcNodesType tabledata_SigmT;
    for(auto i = 0; i < this->Tk_sigma_model->rowCount(); i++){
        auto T  	= (*Tk_sigma_model)[i][0];
        auto sigma  = (*Tk_sigma_model)[i][1];
        tabledata_SigmT.push_back({T,sigma});
    }

    /* ************************************************************* */
    /* init interpollation objects */

    static auto T = [T_w](double T0, double m, double z)->double{
        auto T = (T0 - (T_w - T0) * std::pow(z, m));
        return T;
    } ;

    auto newtsmode = KPEq::Interpoll::NewtCnfgEnumC::NORMAL;
//    auto newtsmode = KPEq::Interpoll::NewtCnfgEnumC::LOGARIPHMIC;
    KPEq::Interpoll::NewtPoly newt_T0byI(tabledata_T0I, newtsmode); // interface need but full rework class?
    if(newt_T0byI.isWrong())
        throw std::invalid_argument("wrong table data");
    KPEq::Interpoll::NewtPoly newt_mbyI(tabledata_mI, newtsmode);
    if(newt_mbyI.isWrong())
        throw std::invalid_argument("wrong table data");

    static auto interp_T0_m_byI = [&polypow, &newt_T0byI, &newt_mbyI,T_w](double I) -> std::pair<double,double>
    {
        if(newt_T0byI.setConfig(abs(I), polypow) < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto T0 = newt_T0byI.calc();
        if(!T0.has_value())
            throw std::runtime_error("T0 hasn't value");
        PRINTTB(T0.value());

        if(newt_mbyI.setConfig(abs(I), polypow) < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto m = newt_mbyI.calc();
        if(!m.has_value())
            throw std::runtime_error("m hasn't value");
        PRINTTB(m.value());
       
        return {T0.value(),m.value()};
    };


    KPEq::Interpoll::NewtPoly newt_SigmByT(tabledata_SigmT, newtsmode);
    if(newt_SigmByT.isWrong())
        throw std::invalid_argument("wrong table data");

    static auto sigm = [&polypow, & newt_SigmByT](double T)->double
    {
        if(newt_SigmByT.setConfig(T, polypow) < 0)
            throw std::invalid_argument("newt_SigmByT wrong cnfg");

        auto sigma = newt_SigmByT.calc();
        if(!sigma.has_value())
            throw std::runtime_error("sigma hasn't value");
        PRINTTB(sigma.value());

        return sigma.value();
    };

    std::tuple<double,double,double> Rp_T0_m_tuple;
    static auto R_p = [l_p, R, integr_step, &Rp_T0_m_tuple](double I) -> double
    {
        auto pair_T0_m = interp_T0_m_byI(I);
        auto T0 = pair_T0_m.first;
        auto m = pair_T0_m.second;
        KPEq::IntegrandFunc integrfunc = [T0,m](double z) -> double {
            auto iterIntegr = sigm(T(T0,m,z))*z;
            PRINTTB(iterIntegr);
            return iterIntegr;
        };

        KPEq::Integral integr(integrfunc, 0, 1);
        auto integr_sigmaByTByT0nZ_mul_z = integr.calcBySympson(integr_step);
        PRINTLN(integr_sigmaByTByT0nZ_mul_z);

        auto R_p = ( l_p / (2 * M_PI * R * R * (integr_sigmaByTByT0nZ_mul_z))); PRINTTB(R_p);

        Rp_T0_m_tuple = std::tuple(R_p,T0,m);
        return R_p;
    };

    auto di_dt = [R_k, L_k](double t, double I, double U) -> double {
        auto R_p_ByI = R_p(I);
        auto result = ( (U - (R_k + R_p_ByI) * I) / L_k );
        return result;
    };
    auto du_dt = [C_k](double t, double I, double U) -> double {
        auto result = ( -I / C_k );
        return result;
    };
    /* ************************************************************* */

    /* init ODE */
    double modTime = ui->modtimeSpin->value();
    double modStep = ui->modstepSpin->value();
    double time = 0;

    double RpScale = 1000.0, T0Scale = 0.1, mScale = 10.0;
    QLineSeries * results_series_I = new QLineSeries(); 	results_series_I->setName("I(t)");
    QLineSeries * results_series_U = new QLineSeries(); 	results_series_U->setName("U(t)");
    QLineSeries * results_series_Rp = new QLineSeries(); 	results_series_Rp->setName(QString("Rp(t)*%1").arg(RpScale));
    QLineSeries * results_series_IRp = new QLineSeries(); 	results_series_IRp->setName(QString("I(t)*Rp(t)"));
    QLineSeries * results_series_T0 = new QLineSeries(); 	results_series_T0->setName(QString("T0(t)*%1").arg(T0Scale));
//    QLineSeries * results_series_m = new QLineSeries(); 	results_series_m->setName(QString("m(t)*%1").arg(mScale));
    QVector<QLineSeries*> series_vect;
    series_vect << results_series_I << results_series_U << results_series_Rp << results_series_IRp << results_series_T0 ;

    /* calc ODE */
    KPEq::ODE::ODESysEqua odeSysEqua(di_dt, du_dt, modStep);

    KPEq::Q::Tcont results;
    results.push_back({time,I,U});

    while(time < modTime)
    {
        auto I_U_pair = odeSysEqua.calcNextByRunge1(time, I, U); //here need switch to choise Runge, need realize Runge1
        I = I_U_pair.first; 	PRINTLN(I);
        U = I_U_pair.second; 	PRINTLN(U); PRINTNEWLN();
        if(I!=I || U != U) 
            break; // is NaN

        /* set data to table */
        double Rp,T0,m;
        std::tie(Rp,T0,m) = Rp_T0_m_tuple;
        results.push_back({time,I,U,Rp*RpScale,I*Rp,T0*T0Scale});
    }

    (*results_model) << std::move(results);
    results_model->setHeaderData(0, Qt::Orientation::Horizontal,  "t");
    results_model->setHeaderData(1, Qt::Orientation::Horizontal,  "I");
    results_model->setHeaderData(2, Qt::Orientation::Horizontal,  "U");
    results_model->setHeaderData(3, Qt::Orientation::Horizontal,  QString("Rp*%1").arg(RpScale));
    results_model->setHeaderData(4, Qt::Orientation::Horizontal,  "I*Rp");
    results_model->setHeaderData(5, Qt::Orientation::Horizontal,  QString("T0*%1"));


    /* add results to chart */
    bool isFirst = true;
    int counter = 1;
        fmt::print("{}\n", counter);
    chart->removeAllSeries();
    for(auto s : series_vect)
    {
        s->clear();
        auto map = new QVXYModelMapper(this);
        map->setXColumn(0);
        fmt::print("{}\n", counter);
        map->setYColumn(counter++);
        map->setModel(results_model);
        map->setSeries(s);
        if(isFirst)
        {
            this->chart->updateSeries(s);
            isFirst = false;
        }
        else
            this->chart->addSeries(s);
        this->chart->createDefaultAxes();
    }
}
