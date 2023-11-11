cmake_minimum_required(VERSION 3.0.0)
project(KPEq_ODE VERSION 0.1.0 LANGUAGES C CXX)


add_library(${PROJECT_NAME} ODE.cpp ./inc/KPEq/ODE.hpp)
target_include_directories(${PROJECT_NAME} PUBLIC inc)
target_link_libraries(${PROJECT_NAME} PUBLIC revpolnot_kp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

