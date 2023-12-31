#include <KPEq/integral.hpp>
#include <fmt/core.h>

KPEq::Integral::Integral(IntegrandFunc &integrand_func, double from, double to) : integrand_func(integrand_func), from(from), to(to) {
    if(from > to) {
        throw std::invalid_argument((std::string)fmt::format("Wrong arguments: from {} more than to {}", from, to));
    }
}

double KPEq::Integral::calcBySympson(double step) // step or numb of intervals maybe?
{
    auto range = to - from ;
    auto amount= (int)(range / step);

    if(step <= 0) {
        throw std::invalid_argument((std::string)fmt::format("Wrong argument: step {} must be more than 0", step)); //or must be more than 2?
    }
    //fix it for b
    double result = (integrand_func(from) + integrand_func(to)) ;
    bool isEven = true;
    // = 1 / 3 (f_2n+0 + 2 + 4 * f_2n+1 + f_2n+2 ) ???
    double x = from + step;
    for(auto i = 1; i < amount; i++)
    {
        if(isEven)
            result += (integrand_func(x) * 4);
        else
            result += (integrand_func(x) * 2);
        isEven = isEven ? false : true; //simple toogle
        x += step;
    }
    result *= step;
    result /= 3;
    return result;
}
