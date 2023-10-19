#pragma once
#include <functional>
#include <exception>
#include <string>
#include <fmt/core.h>

namespace KPEq{

    class Integral
    {
    private:
        using IntegrandFunc = std::function<double (double)>;
        IntegrandFunc integrand_func;
        double from;
        double to;
    public:
        Integral(IntegrandFunc & integrand_func, double from, double to) : integrand_func(integrand_func), from(from), to(to) {
            if(from > to) {
                throw std::invalid_argument((std::string)fmt::format("Wrong arguments: from {} more than to {}", from, to));
            }
        }
        // Integral(RPN_KPEq expression, double from, double to);

        //eps -> approx step but need even numb of intervals
        double calcBySympson(double step){
            if(step <= 0) {
                throw std::invalid_argument((std::string)fmt::format("Wrong argument: step {} must be more than 0", step)); //or must be more than 2?
            }

            double result = (integrand_func(from) + integrand_func(to)) ;
            bool isEven = true;
            for(double x = from + step; x < to; x+=step){
                if(isEven)
                    result += (integrand_func(x) * 4); 
                else
                    result += (integrand_func(x) * 2);
                isEven = isEven ? false : true; //simple toogle
            }
            result *= step;
            result /= 3;
            return result;
        }
    };
    
}