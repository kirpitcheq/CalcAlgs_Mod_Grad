#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <cmath>
#include <KPEq/newtpoly.hpp>
#include <KPEq/integral.hpp>
#include <KPEq/rungeode.hpp>
#include <exception>
#include <fmt/core.h>

#define PRINTTB(VAL) fmt::print("{} {}\t",#VAL,VAL)
#define PRINTLN(VAL) fmt::print("{} {}\n",#VAL,VAL)
#define PRINTNEWLN() fmt::print("\n")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#define LIKE_FROM_DOC
#ifdef LIKE_FROM_DOC
    double R = 0.35;
    double l_p = 12; //sm -> m
    double L_k = 187; //from spin or TexBox
    double C_k = 268;
    double R_k = 0.25; //from spin or TexBox
    double U_co = 1400;
    double I_0 = 3;
    double T_w = 2000;
#else
    double R = 0.0035;
    double l_p = 0.012; //sm -> m
    double L_k = 0.000187; //from spin or TexBox
    double C_k = 0.000268;
    double R_k = 0.25; //from spin or TexBox
    double U_co = 1400;
    double I_0 = 0;
    double T_w = 2000;
#endif

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

    this->I_T0_m_data_model = new KPEq::Q::ItemModel(I_T0_m_data); //here need ref from model for data access
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
    ui->tableV_Sigma_T->setModel(Tk_sigma_model);

    this->chart = new KPEq::Chart;
    ui->graphicsView->setChart(this->chart);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_calculateBtn_clicked()
{
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
    static KPEq::Interpoll::SrcNodesType tabledata_T0I;
    static KPEq::Interpoll::SrcNodesType tabledata_mI;
    for(auto i = 0; i < this->I_T0_m_data_model->rowCount(); i++){
        auto I  = (*I_T0_m_data_model)[i][0];
        auto T0 = (*I_T0_m_data_model)[i][1];
        auto m  = (*I_T0_m_data_model)[i][2];
        tabledata_T0I.push_back	({I,T0});
        tabledata_mI.push_back	({I,m});
    }
    static KPEq::Interpoll::SrcNodesType tabledata_SigmT;
    for(auto i = 0; i < this->I_T0_m_data_model->rowCount(); i++){
        auto T  	= (*Tk_sigma_model)[i][0];
        auto sigma  = (*Tk_sigma_model)[i][1];
        tabledata_SigmT.push_back({T,sigma});
    }

    /* init interpollation objects */

    static auto T = [polypow, T_w](double I, double z)->double{
    KPEq::Interpoll::NewtPoly newt_T0byI(tabledata_T0I, KPEq::Interpoll::NewtCnfgEnumC::LOGARIPHMIC); // interface need but full rework class?
    if(newt_T0byI.isWrong())
        throw std::invalid_argument("wrong table data");
    KPEq::Interpoll::NewtPoly newt_mbyI(tabledata_mI, KPEq::Interpoll::NewtCnfgEnumC::LOGARIPHMIC);
    if(newt_mbyI.isWrong())
        throw std::invalid_argument("wrong table data");
        auto configErr = newt_T0byI.setConfig(I, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto T0Opt = newt_T0byI.calc();
        if(!T0Opt.has_value())
            throw std::runtime_error("T0 hasn't value");
        auto T0 = T0Opt.value();
        PRINTTB(T0);
        configErr = newt_mbyI.setConfig(I, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtbyZ wrong cnfg");
        auto mOpt = newt_mbyI.calc();
        if(!mOpt.has_value())
            throw std::runtime_error("m hasn't value");
        auto m = mOpt.value();
        PRINTTB(m);
        auto T = (T0 - (T_w - T0) * std::pow(z, m));
        PRINTTB(z);
        PRINTTB(T);
        return T;
    } ;

    static auto sigm = [polypow](double T)->double{
    KPEq::Interpoll::NewtPoly newt_SigmByT(tabledata_SigmT, KPEq::Interpoll::NewtCnfgEnumC::LOGARIPHMIC);
    if(newt_SigmByT.isWrong())
        throw std::invalid_argument("wrong table data");
        auto configErr = newt_SigmByT.setConfig(T, polypow);
        if(configErr < 0)
            throw std::invalid_argument("newtByT wrong cnfg");
        auto sigmaOpt = newt_SigmByT.calc();
        if(!sigmaOpt.has_value())
            throw std::runtime_error("sigma hasn't value");
        auto sigma = sigmaOpt.value();
        PRINTTB(sigma);
        return sigma;
    };

    static auto R_p = [l_p, R, integr_step](double I) -> double
    {
        KPEq::IntegrandFunc integrfunc = [I](double z) -> double { 
            auto iterIntegr = sigm(T(I,z))*z;
            PRINTTB(iterIntegr);
            return iterIntegr;
        };
        KPEq::Integral integr(integrfunc, 0, 1);
        auto integr_sigmaByTByT0nZ_mul_z = integr.calcBySympson(integr_step);
        PRINTLN(integr_sigmaByTByT0nZ_mul_z);
        auto R_p = ( l_p / (2 * M_PI * R * R * (integr_sigmaByTByT0nZ_mul_z)));
        PRINTTB(R_p);
        return R_p;
    };

    auto di_dt = [R_k, L_k](double t, double I, double U) -> double {
        auto R_p_ByI = R_p(I);
        auto result = ( ((U - (R_k + R_p_ByI)) * I) / L_k );
        return result;
    };
    auto du_dt = [C_k](double t, double I, double U) -> double {
        auto result = ( -I / C_k );
        return result;
    };

    /* calculate ODE */
    double modtime = ui->modtimeSpin->value();
    double modStep = ui->modstepSpin->value();
    double time = 0;

    KPEq::Q::Tcont results;
    QLineSeries * results_series_I = new QLineSeries();
    QLineSeries * results_series_U = new QLineSeries();
    results.push_back({time,I,U});
    results_series_I->append({time,I});
    results_series_U->append({time,U});

    KPEq::ODE::ODESysEqua odeSysEqua(di_dt, du_dt, modStep);
    while(time < modtime){
        auto I_U_pair = odeSysEqua.calcNextByRunge2(time, I, U); //here need switch to choise Runge, need realize Runge1
        I = I_U_pair.first;
        PRINTLN(I);
        U = I_U_pair.second;
        PRINTLN(U);
        PRINTNEWLN();
        if(I!=I || U != U) break; // is NaN
        /* set data to table */
        results.push_back({time,I,U});
        results_series_I->append({time,I});
        results_series_U->append({time,U});
    }
    this->chart->updateSeries(results_series_I);
    this->chart->addSeries(results_series_U);
    this->chart->createDefaultAxes();
    results_model = new KPEq::Q::ItemModel(results);
    ui->tableV_OutData->setModel(results_model);
}
