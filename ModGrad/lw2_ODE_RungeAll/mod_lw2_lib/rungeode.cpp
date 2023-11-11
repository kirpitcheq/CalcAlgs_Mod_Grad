#include <KPEq/rungeode.hpp>

KPEq::ODE::ODESysEqua::ODESysEqua(ODEFunc func1, ODEFunc func2, TVal step)
    : func1(func1), func2(func2), step(step)
{ /*check args*/ }

std::pair<KPEq::ODE::ODESysEqua::TVal, KPEq::ODE::ODESysEqua::TVal> KPEq::ODE::ODESysEqua::calcNextByRunge1(TVal &xlast, const TVal ylast, const TVal zlast)
{
    xlast+=step;
    return
    {
        ylast + step * func1(xlast,ylast,zlast),
                zlast + step * func2(xlast,ylast,zlast),
    };
}

std::pair<KPEq::ODE::ODESysEqua::TVal, KPEq::ODE::ODESysEqua::TVal> KPEq::ODE::ODESysEqua::calcNextByRunge2
(TVal &xlast, const TVal ylast, const TVal zlast)
{
    TVal alpha = 1.0;
    TVal k1 = func1(xlast,ylast,zlast); // ? no step mul?
    TVal j1 = func2(xlast,ylast,zlast); // ? no step mul?
    TVal k2 = step * func1(xlast + step/(2.0*alpha), ylast + step/(2.0*alpha) * k1, zlast + step/(2.0*alpha) * j1);
    TVal j2 = step * func2(xlast + step/(2.0*alpha), ylast + step/(2.0*alpha) * k1, zlast + step/(2.0*alpha) * j1);
    xlast+=step;
    return {
        ylast + (1.0-alpha)*k1 + alpha*k2,
        zlast + (1.0-alpha)*j1 + alpha*j2
    };
}

std::pair<KPEq::ODE::ODESysEqua::TVal, KPEq::ODE::ODESysEqua::TVal> KPEq::ODE::ODESysEqua::calcNextByRunge4(TVal &xlast, TVal ylast, TVal zlast){
    TVal k1 = step * func1(xlast,ylast,zlast);
    TVal j1 = step * func2(xlast,ylast,zlast);

    TVal k2 = step * func1(xlast+step/2.0,ylast+k1/2.0, zlast+j1/2.0);
    TVal j2 = step * func2(xlast+step/2.0,ylast+k1/2.0, zlast+j1/2.0);

    TVal k3 = step * func1(xlast+step/2.0,ylast+k2/2.0, zlast+j2/2.0);
    TVal j3 = step * func2(xlast+step/2.0,ylast+k2/2.0, zlast+j2/2.0);

    TVal k4 = step * func1(xlast+step,ylast+k3,zlast+j3);
    TVal j4 = step * func2(xlast+step,ylast+k3,zlast+j3);

    xlast += step;
    auto y_res = ylast + (k1 + 2.0*k2 + 2.0*k3 + k4) / 6.0 ;
    auto z_res = zlast + (j1 + 2.0*j2 + 2.0*j3 + j4) / 6.0;
    return { y_res, z_res };
}
