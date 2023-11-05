#ifndef LW2TASK_H
#define LW2TASK_H
#include <KPEq/newtpoly.hpp>
#include <KPEq_Q_Libs/itemmodel.h>
#include <KPEq/chart.h>
#include <KPEq/integral.hpp>

#include <KPEq/rungeode.hpp>
#include <QVXYModelMapper>

#ifdef MOD_LW2_DEBUG
    #define PRINTTB(VAL) fmt::print("{} {}\t",#VAL,VAL)
    #define PRINTLN(VAL) fmt::print("{} {}\n",#VAL,VAL)
    #define PRINTNEWLN() fmt::print("\n")
#else
    #define PRINTTB(VAL) ;
    #define PRINTLN(VAL) ;
    #define PRINTNEWLN() ;
#endif

class LW2Task
{
public:

    using ODETVal = KPEq::ODE::ODESysEqua::TVal;
    using ODEResV = std::pair<ODETVal, ODETVal>;
    using ODEMeth = KPEq::ODE::ODESysEqua;
//	using ODESign = ODEResV ODEMeth::*(ODETVal &xlast, const ODETVal ylast, const ODETVal zlast);
    using ODESign =  ODEResV(ODEMeth::*, ODETVal&,const ODETVal,const ODETVal);
    std::function<ODESign> odemeth;

    LW2Task(int polypow,
        double integr_step,
        KPEq::Interpoll::NewtCnfgEnumC newtsmode,
        const KPEq::Q::ItemModel &modelinits,
        const KPEq::Q::ItemModel &modelIT0,
        const KPEq::Q::ItemModel &modelIm,
        const KPEq::Q::ItemModel &modelTSigm);
    void calcODE(double modTime, double modStep, KPEq::Q::ItemModel & results_model, KPEq::Chart & chart);
private:
    bool isCachable = true;
    /* init values */
    double R, l_p, L_k, C_k, R_k, U_co, I_0, T_w, integr_step;
    int polypow;
    KPEq::Interpoll::NewtCnfgEnumC newtsmode;
    KPEq::Interpoll::SrcNodesType tabledata_T0I, tabledata_mI, tabledata_SigmT;
    KPEq::Interpoll::NewtPoly newt_T0byI,newt_mbyI,newt_SigmByT; // interface need but full rework class?
    /* ----------------------------------------------------------------- */

    /* this need set by common table.. but count of values difference! */
    /* is need caching values ? */


    KPEq::Interpoll::SrcNodesType nodesFromModels(const KPEq::Q::ItemModel & model, int first_i, int second_i);


    /* ************************************************************* */
    /* init interpollation objects */

    double T (double T0, double m, double z) { return (T0 - (T_w - T0) * std::pow(z, m)); }

    std::pair<double,double> interp_T0_m_byI(double I)
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

    double sigm (double T)
    {
        if(newt_SigmByT.setConfig(T, polypow) < 0)
            throw std::invalid_argument("newt_SigmByT wrong cnfg");

        auto sigma = newt_SigmByT.calc();
        if(!sigma.has_value())
            throw std::runtime_error("sigma hasn't value");
        PRINTTB(sigma.value());

        return sigma.value();
    };

    double T0 ;
    double m ;
    std::tuple<double,double,double> Rp_T0_m_tuple;
    auto R_p (double I)
    {
        auto pair_T0_m = interp_T0_m_byI(I);
        T0 = pair_T0_m.first;
        m = pair_T0_m.second;
        KPEq::IntegrandFunc integrfunc = [this](double z) -> double {
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

};

#endif // LW2TASK_H
