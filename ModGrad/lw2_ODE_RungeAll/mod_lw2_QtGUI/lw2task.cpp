#include "lw2task.h"

//LW2Task::LW2Task()
//{

//    auto newtsmode = KPEq::Interpoll::NewtCnfgEnumC::LOGARIPHMIC;

//    int polypow = ui->polypowSpin->value();
//    double integr_step = ui->integstepSpin->value();

//    KPEq::Interpoll::NewtPoly newt_T0byI(tabledata_T0I, newtsmode); // interface need but full rework class?
//    KPEq::Interpoll::NewtPoly newt_mbyI(tabledata_mI, newtsmode);
//}

LW2Task::LW2Task(int polypow, double integr_step, KPEq::Interpoll::NewtCnfgEnumC newtsmode, const KPEq::Q::ItemModel &modelinits, const KPEq::Q::ItemModel &modelIT0, const KPEq::Q::ItemModel &modelIm, const KPEq::Q::ItemModel &modelTSigm)
    : polypow(polypow), integr_step(integr_step), newtsmode(newtsmode),
      tabledata_T0I(nodesFromModels(modelIT0, 0, 1)),
      tabledata_mI(nodesFromModels(modelIm, 0 , 2)),
      tabledata_SigmT(nodesFromModels(modelTSigm,0,1)),
      newt_T0byI(tabledata_T0I, newtsmode),
      newt_mbyI(tabledata_mI, newtsmode),
      newt_SigmByT(tabledata_SigmT, newtsmode)
{
    if(newt_T0byI.isWrong())
        throw std::invalid_argument("wrong table data I-T0");
    if(newt_mbyI.isWrong())
        throw std::invalid_argument("wrong table data I-m");
    if(newt_SigmByT.isWrong())
        throw std::invalid_argument("wrong table data T-sigma");

    R   = modelinits[0][0];//Tk_sigma_model[0][1];
    l_p = modelinits[1][0]; //sm -> m
    L_k = modelinits[2][0]; //from spin or TexBox
    C_k = modelinits[3][0];
    R_k = modelinits[4][0]; //from spin or TexBox
    U_co= modelinits[5][0];
    I_0 = modelinits[6][0];
    T_w = modelinits[7][0];
}

void LW2Task::calcODE(double modTime,
                      double modStep,
                      QString ser_id,
                      KPEq::Q::ItemModel &results_model,
                      KPEq::Chart &chart,
                      std::function<ODEResV(ODEMeth::*, ODETVal&,const ODETVal,const ODETVal)){
    auto di_dt = [this](double t, double I, double U) -> double
    {
        auto R_p_ByI = R_p(I);
        auto result = ( (U - (R_k + R_p_ByI) * I) / L_k );
        return result;
    };
    auto du_dt = [this](double t, double I, double U) -> double
    {
        auto result = ( -I / C_k );
        return result;
    };
    /* ************************************************************* */

    /* init ODE */

    double RpScale = 1000.0, T0Scale = 0.1, mScale = 10.0;
    QLineSeries * results_series_I = new QLineSeries(); 	results_series_I->setName(QString("%1:I(t)").arg(ser_id));
    QLineSeries * results_series_U = new QLineSeries(); 	results_series_U->setName(QString("%1:U(t)").arg(ser_id));
    QLineSeries * results_series_Rp = new QLineSeries(); 	results_series_Rp->setName(QString("%1:Rp(t)*%2").arg(ser_id).arg(RpScale));
    QLineSeries * results_series_IRp = new QLineSeries(); 	results_series_IRp->setName(QString("%1:I(t)*Rp(t)").arg(ser_id));
    QLineSeries * results_series_T0 = new QLineSeries(); 	results_series_T0->setName(QString("%1:T0(t)*%2").arg(ser_id).arg(T0Scale));
    //    QLineSeries * results_series_m = new QLineSeries(); 	results_series_m->setName(QString("m(t)*%1").arg(mScale));
    QVector<QLineSeries*> series_vect;
    series_vect << results_series_I << results_series_U << results_series_Rp << results_series_IRp << results_series_T0 ;

    /* calc ODE */
    KPEq::ODE::ODESysEqua odeSysEqua(di_dt, du_dt, modStep);

    double time = 0;
    double I = I_0;
    double U = U_co;
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

    results_model << std::move(results);
    results_model.setHeaderData(0, Qt::Orientation::Horizontal,  "t");
    results_model.setHeaderData(1, Qt::Orientation::Horizontal,  "I");
    results_model.setHeaderData(2, Qt::Orientation::Horizontal,  "U");
    results_model.setHeaderData(3, Qt::Orientation::Horizontal,  QString("Rp*%1").arg(RpScale));
    results_model.setHeaderData(4, Qt::Orientation::Horizontal,  "I*Rp");
    results_model.setHeaderData(5, Qt::Orientation::Horizontal,  QString("T0*%1"));


    /* this chart must be common, need semaph or something */
    /* add results to chart */
    int counter = 1;
//    chart.removeAllSeries();
    chart.createDefaultAxes();
    for(auto s : series_vect)
    {
        s->clear();
        auto map = new QVXYModelMapper();
        map->setXColumn(0);
        map->setYColumn(counter++);
        map->setModel(&results_model);
        map->setSeries(s);
        chart.addSeries(s);
        chart.createDefaultAxes();
    }
}

KPEq::Interpoll::SrcNodesType LW2Task::nodesFromModels(const KPEq::Q::ItemModel &model, int first_i, int second_i)
{
    KPEq::Interpoll::SrcNodesType result;
    for(int i = 0; i < model.rowCount(); i++)
    {
        auto first  = model[i][first_i];
        auto second = model[i][second_i];
        result.push_back({first,second});
    }
    return result;
}
