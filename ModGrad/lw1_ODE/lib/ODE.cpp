#include <KPEq/ODE.hpp>
#include <exception>
namespace KPEq{
    TwinVectT EilCalc(double x0, double y0, double step, FuncODE func){ //end condition by func arg?
        TwinVectT result;

        #define END_COND x0
        int i = 0;
        double y0d = func(x0, y0);
        double x = x0;
        result.push_back({x0,y0d});
        while(i < 10){

            double xlast = result.back().first;
            double ylast = result.back().second;
            double y = ylast + y0d*step;
            y0d = func(xlast,ylast);
            result.push_back({x,y});
            i++;
            x += step;
        }
        return result;
    }
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
    TwinVectT EilCalc(double x0, double y0, double step, double xmax, std::string expression, 
                        double stepAutoAcc, std::vector<double>* stepsvect){
        if(expression.empty())
            return TwinVectT();
        TwinVectT result;

        double x = x0;
        double y0d;
        result.push_back({x0,y0});
        /*
         * RPN expression calc x, need use it for y
         *x set as y in expression
         */
        RPN_KP func(expression); //init with expr: infix->postfix, parse one time
        bool isFirst = true;
        while(x <= xmax){ //when need stop ?
            double ystorage;
            double xlast = result.back().first;
            double ylast = result.back().second;
            func.setX(xlast);
            func.setY(ylast);
            if(func.calculate(y0d) == false) // stack back and swap x/y to corr value
                throw std::invalid_argument("wrong expression");
                // throw std::exception("wrong expression");
            double y = ylast + y0d*step; // acc : aver div two derivative (of funcs), need first derivative y1 (?)
            if(stepAutoAcc != 0){ //only if have this value and not def
                //first time each iter? 
                if(isFirst){
                    result.push_back({x, y}); 
                    isFirst = false;
                }
                else{
                    //compare y //here need auto toogle once up or down for block neighbour branch
                    if(abs((ylast - y)/y) >= stepAutoAcc)
                        step/=2;
                    else
                    {
                        isFirst = true;
                        x+=step;
                    }
                        //break this
                        // step*=2; //recalc or save it somehow?
                }
                continue;
            } // x0 or xlast compare with xnow (here xlast with xnow)
            else{
                if(isFirst == false)
                    isFirst = true;
                result.push_back({x,y}); //rewrite value y only!
                // if(stepsvect != nullptr)
                    // stepsvect->push_back(step);

                x += step;
            }
        }
        return result;
    }

    double first_func(double x, double u) { return 1 / (u * u + x); }
    double second_func(double x, double u) { return 1 / (2 * x * u + u * u * u); }
    double third_func(double x, double u) { return x + ((u * u) / x); }


} // namespace ODEquaKPeQ
