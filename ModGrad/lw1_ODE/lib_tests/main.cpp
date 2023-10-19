//#define CATCH_CONFIG_MAIN
//#include <catch2/catch_config.hpp>
#include <ODE_KPEq.h>
#include <catch2/catch_test_macros.hpp>

#ifdef OLD_DEF
TEST_CASE( "ODE simple func", "[main]" ) {
    auto some = ODE_KPeQ::EilCalc(0,1,0.1, ODE_KPeQ::first_func);
    for(auto [f,s] : some){
        std::cout << f << ", " << s << std::endl;
    }
}
#endif

TEST_CASE( "ODE with expr", "[main]" ) {
    auto expression = "y*2";
    auto some = ODE_KPeQ::EilCalc(1,1,0.1,10,expression);
    for(auto [f,s] : some){
        std::cout << f << ", " << s << std::endl;
    }
}
