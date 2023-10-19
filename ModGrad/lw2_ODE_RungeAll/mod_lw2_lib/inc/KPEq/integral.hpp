#pragma once
#include <functional>
#include <exception>
#include <string>

namespace KPEq{

    using IntegrandFunc = std::function<double (double)>;
    class Integral
    {
    private:
        IntegrandFunc integrand_func;
        double from;
        double to;
    public:
        Integral(IntegrandFunc & integrand_func, double from, double to);
        // Integral(RPN_KPEq expression, double from, double to);
        //eps -> approx step but need even numb of intervals
        double calcBySympson(double step);
    };
    
}
