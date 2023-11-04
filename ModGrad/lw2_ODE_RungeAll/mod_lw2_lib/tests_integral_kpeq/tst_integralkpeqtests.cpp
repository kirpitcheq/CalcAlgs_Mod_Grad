#include <catch2/catch.hpp>
#include <KPEq/integral.hpp>
#include <cmath>
#include <iostream>
#include <iomanip>

TEST_CASE("My first test with Catch2", "[fancy]")
{
    REQUIRE(0 == 0);
}

using namespace KPEq;
TEST_CASE("IntegralKPEqSimpleTest", "[fancy]")
{
    IntegrandFunc sincos = [](double x)->double{return (sin(x)+cos(x));};
    Integral integral(sincos, 0, 1);

    double result = integral.calcBySympson(0.1);
    double mustbe = sin(1) - cos(1) + 1;
    std::cout << "result, mustbe: "
              << std::setprecision (10)
              << result
              << ","
              << mustbe
              << std::endl;
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,0.000001));
}
TEST_CASE("SimpleTest_SeveralSteps", "[fancy]")
{
    IntegrandFunc sincos = [](double x)->double{return (sin(x)+cos(x));};
    Integral integral(sincos, 0, 1);

    double mustbe = sin(1) - cos(1) + 1;

    double result = integral.calcBySympson(0.01);
    std::cout << "result, mustbe: "
              << std::setprecision (12)
              << result
              << ","
              << mustbe
              << std::endl;
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,1e-9));

    result = integral.calcBySympson(0.001);
    std::cout << "result, mustbe: "
              << std::setprecision (17)
              << result
              << ","
              << mustbe
              << std::endl;
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,1e-14));

    result = integral.calcBySympson(0.0001);
    std::cout << "result, mustbe: "
              << std::setprecision (19)
              << result
              << ","
              << mustbe
              << std::endl;
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,1e-14));

    // up step -> get more err
    result = integral.calcBySympson(0.0000001);
    std::cout << "result, mustbe: "
              << std::setprecision (25)
              << result
              << ","
              << mustbe
              << std::endl;
    // less then prev!
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,1e-11));
}

TEST_CASE("IntegralKPEqSimple", "[fancy]")
{
    IntegrandFunc xsquare = [](double x)->double{return x*x;};
    Integral integral(xsquare, 0, 1);

    double result = integral.calcBySympson(0.1);

    // int(x^2) = x^3 / 3 => 1/3 - 0
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(1.0/3.0,0.1));
}

TEST_CASE("IntegralKPEqDiffAccur", "[fancy]")
{
    IntegrandFunc mdle = [](double x)->double{return x*x;};
    Integral integral(mdle, 0, 1);

    double result = integral.calcBySympson(0.1);
    // int(x^2) = x^3 / 3 => 1/3 - 0
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(1.0/3.0,0.1));

    result = integral.calcBySympson(0.01);
    // int(x^2) = x^3 / 3 => 1/3 - 0
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(1.0/3.0,0.1));

    result = integral.calcBySympson(0.001);
    // int(x^2) = x^3 / 3 => 1/3 - 0
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(1.0/3.0,0.1));
}

TEST_CASE("LowLimitNotNull_UpLimitMoreFloat", "[fancy]")
{
    double uplimit = 2.53;
    double lolimit = 0.34;

    IntegrandFunc toatan = [](double x) {return (1.0)/(4.0+(x*x));};
    Integral integral(toatan, lolimit, uplimit);
    double result = integral.calcBySympson(0.001);

    auto analyt_toatan = [](double x){return ( (1.0)/(2.0) ) * (atan(x / 2.0) );};
    double mustbe = analyt_toatan(uplimit) - analyt_toatan(lolimit);

    std::cout << "result, mustbe: "
              << std::setprecision (15)
              << result
              << ","
              << mustbe
              << std::endl;
    REQUIRE_THAT(result, Catch::Matchers::Floating::WithinAbsMatcher(mustbe,1e-14));
}
