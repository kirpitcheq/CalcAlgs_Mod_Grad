#include <KPEq/integral.hpp>
#include <fmt/core.h>

KPEq::Integral::Integral(IntegrandFunc &integrand_func, double from, double to) : integrand_func(integrand_func), from(from), to(to) {
    if(from > to) {
        throw std::invalid_argument((std::string)fmt::format("Wrong arguments: from {} more than to {}", from, to));
    }
}

double KPEq::Integral::calcBySympson(double step) // step or numb of intervals maybe?
{
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
