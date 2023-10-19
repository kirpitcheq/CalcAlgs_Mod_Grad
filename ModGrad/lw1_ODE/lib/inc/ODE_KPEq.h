#pragma once
#include <iostream>
#include <vector>
#include <KPEq/revpolnot_kp.hpp>
#include <optional>

namespace ODE_KPeQ
{
    /*must be two methods: (no need classes for this because too simple, but use class may be if polnot input)*/
    /*
     * y' = f(x,y)
     * need initial conditions
     * y(x0) = y0
     * RungeKutta meth:
     * y - y0 = y0'(x - x0) - main equa
     * y0' = f(x0,y0) - first point of tangent
     *
     * y1 - y0 = y0'(x1 - x0)
     * h = (x - x0) - this must be change for rule of Runge (but maybe more than this)
     * y1 - y0 = y0'*h
     * y1 = y0 + y0'*h
     * other points:
     * y_i = y_i-1 + y_i-1'*h
     * Eiler meth have a first order of accuracy
     */

    /*
     * functions for need eq
     */

    /*
     *  ( u^2 + x ) * u` = 1
     *  => u' = 1 / (u^2 + x)
     * u(1) = 0
     */
    double first_func(double x, double u);
    /*
     * 1 - 2*x*u*u` = u^3*u`
     * => 1 = (2*x*u + u^3)*u`
     * => u` = 1 / (2*x*u + u^3)
     * u(0.5) = 0
     */
    double second_func(double x, double u);
    /*
     * u`(x) = x^2 + u^2
     * u` = (x^2 + u^2) / x
     * u` = x + (u^2)/x
     * u(0) = 0
     */
    double third_func(double x, double u);

    using namespace std;
    using T = double;
    using XYNode = pair<T,T>;
    using TwinVectT = vector<XYNode>;
    using FuncODE = double (*)(double x, double y);
    TwinVectT EilCalc(double x0, double y0, double step, FuncODE func);
    // TwinVectT EilCalc(double x0, double y0, double step, double xmax, string expression);

    TwinVectT EilCalc(double x0, double y0, double step, double xmax, std::string expression, 
                        double stepAutoAcc = 0, std::vector<double>* stepsvect = nullptr);
} // namespace ODEquaKPeQ
