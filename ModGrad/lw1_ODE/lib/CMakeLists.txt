cmake_minimum_required(VERSION 3.0.0)
project(KPEq_ODE VERSION 0.1.0 LANGUAGES C CXX)


add_library(${PROJECT_NAME} ODE.cpp ./inc/KPEq/ODE.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC inc)

include(FetchContent)
FetchContent_Declare(
    SaDT_n_STaD
    GIT_REPOSITORY https://github.com/kirpitcheq/SaDT_n_STaD.git
    GIT_TAG        RPN_Fix
)
FetchContent_MakeAvailable(SaDT_n_STaD)
target_link_libraries(${PROJECT_NAME} PUBLIC revpolnot_kp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

