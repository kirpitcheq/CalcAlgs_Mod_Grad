#pragma once
#include <functional>

namespace KPEq{
    class ODE
    {
   public:
       using Container = std::vector<std::pair<double, double>>;
       using ODEFunc = std::function<double(double,double)>;
       ODE(ODEFunc func, double y0, double x0, double xmax, double step) 
       : func(func), initvalue(initvalue), from(from), to(to), step(step)
       {
           // check args
       }

       bool calcByRunge1(Container &resstore)
       {
           double value = initvalue;
           resstore.push_back({from, initvalue});
           while (from < to)
           {
            double xlast = resstore.back().first;
            double ylast = resstore.back().second;
            initvalue = func(xlast, ylast);
            value = ylast + initvalue*step;
            resstore.push_back({from,value});
            from += step;
           }
           return true;
       }
       bool calcByRunge2(Container &resstore)
       {
           double value = initvalue;
           resstore.push_back({from, initvalue});
           while (from < to)
           {
            double xlast = resstore.back().first;
            double ylast = resstore.back().second;
            initvalue = func(xlast, ylast);
            value = ylast + initvalue*step;
            resstore.push_back({from,value});
            from += step;
           }
           return true;
       }
       Container calcByRunge4();
    private:
       ODEFunc func;
       double initvalue;
       double from;
       double to;
       double step;
    };

}