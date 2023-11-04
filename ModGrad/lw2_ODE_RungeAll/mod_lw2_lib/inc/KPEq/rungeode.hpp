#pragma once
#include <functional>

namespace KPEq
{
    namespace ODE
    {
        class ODESysEqua 
        {
        public:
            using TVal = double;
            using ODEFunc = std::function<TVal(TVal, TVal, TVal)>;
            ODESysEqua(ODEFunc func1, ODEFunc func2, TVal step);

            /* methods */
            //             std::pair<TVal,TVal> calcNextByRunge1(TVal xlast, TVal ylast){ return (ylast + step * func(xlast,ylast)); }

            std::pair<TVal,TVal> calcNextByRunge1(TVal & xlast, const TVal ylast, const TVal zlast);
            std::pair<TVal,TVal> calcNextByRunge2(TVal & xlast, const TVal ylast, const TVal zlast);
            std::pair<TVal,TVal> calcNextByRunge4(TVal & xlast, TVal ylast, TVal zlast);

        private:
            ODEFunc func1;
            ODEFunc func2;
            TVal step;
        };
    }
}
