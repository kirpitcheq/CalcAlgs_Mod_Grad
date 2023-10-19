#pragma once
#include <functional>

namespace KPEq
{
    namespace ODE
    {

        class ODE_Base
        {
        };
        class ODE : ODE_Base
        {
        public:
            using TVal = double;
            using ODEFunc = std::function<TVal(TVal, TVal)>;
            ODE(ODEFunc func, TVal step)
                : func(func), step(step) { /*check args*/ }
            TVal calcNextByRunge1(TVal xlast, TVal ylast){ return (ylast + step * func(xlast,ylast)); }
            TVal calcNextByRunge2(TVal xlast, TVal ylast){ 
                TVal alpha = 1/2;
                TVal k1 = func(xlast,ylast); // ? no step mul?
                TVal k2 = step * func(xlast + step/(2*alpha), ylast + step/(2*alpha) * k1);
                return (ylast + (1-alpha)*k1 + alpha*k2);
            }
            TVal calcNextByRunge4(TVal xlast, TVal ylast, TVal step){ 
                TVal k1 = step * func(xlast,ylast);
                TVal k2 = step * func(xlast+step/2,ylast+k1/2);
                TVal k3 = step * func(xlast+step/2,ylast+k2/2);
                TVal k4 = step * func(xlast+step,ylast+k3);
                return (ylast + (k1 + 2*k2 + 2*k3 + k4) / 6 );
            }
            TVal calcFunc(TVal x, TVal y) { return func(x, y); }
            TVal getStepValue() { return step; }

        private:
            ODEFunc func;
            TVal step;
        };

        /*with 2 vars*/
        class ODESysEqua 
        {
        public:
            using TVal = double;
            using ODEFunc = std::function<TVal(TVal, TVal, TVal)>;
            ODESysEqua(ODEFunc func1, ODEFunc func2, TVal step)
                : func1(func1), func2(func2), step(step)
            { /*check args*/ }
            //             std::pair<TVal,TVal> calcNextByRunge1(TVal xlast, TVal ylast){ return (ylast + step * func(xlast,ylast)); }
            std::pair<TVal,TVal> calcNextByRunge2(TVal & xlast, const TVal ylast, const TVal zlast){
                TVal alpha = 1/2;
                TVal k1 = func1(xlast,ylast,zlast); // ? no step mul?
                TVal j1 = func2(xlast,ylast,zlast); // ? no step mul?
                TVal k2 = step * func1(xlast + step/(2*alpha), ylast + step/(2*alpha) * k1, zlast + step/(2*alpha) * j1);
                TVal j2 = step * func2(xlast + step/(2*alpha), ylast + step/(2*alpha) * k1, zlast + step/(2*alpha) * j1);
                xlast+=step;
                return {
                    ylast + (1-alpha)*k1 + alpha*k2,
                    zlast + (1-alpha)*j1 + alpha*j2,
                }; //here return pair
            }
            std::pair<TVal,TVal> calcNextByRunge4(TVal & xlast, TVal ylast, TVal zlast){
                TVal k1 = step * func1(xlast,ylast,zlast);
                TVal j1 = step * func2(xlast,ylast,zlast);
                TVal k2 = step * func1(xlast+step/2,ylast+k1/2, zlast+j1/2);
                TVal j2 = step * func2(xlast+step/2,ylast+k1/2, zlast+j1/2);
                TVal k3 = step * func1(xlast+step/2,ylast+k2/2, zlast+j2/2);
                TVal j3 = step * func2(xlast+step/2,ylast+k2/2, zlast+j2/2);
                TVal k4 = step * func1(xlast+step,ylast+k3,zlast+j3);
                TVal j4 = step * func2(xlast+step,ylast+k3,zlast+j3);
                xlast += step;
                return {
                    ylast + (k1 + 2*k2 + 2*k3 + k4) / 6 ,
                    zlast + (j1 + 2*j2 + 2*j3 + j4) / 6 ,
                };
            }
        private:
//            ODEFunc func = std::bind(&ODESysEqua::calcFunc, this, std::placeholders::_1, std::placeholders::_2);
            ODEFunc func1;
            ODEFunc func2;
            TVal step;
        };
    }
}
