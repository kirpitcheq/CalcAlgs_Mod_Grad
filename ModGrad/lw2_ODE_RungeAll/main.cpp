#include <KPEq/integral.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
// #include <functional>

using myfunc = double (*)(double x);
int main(int, char**){
    std::cout << "Hello, from lw2_ODE_RungeAll!\n";

    using namespace KPEq;
    auto functor = [](double x) -> double { return x+2; };
    
    std::vector<double> vect;
    std::sort(vect.begin(), vect.end());

    myfunc somefunc = functor;
}
