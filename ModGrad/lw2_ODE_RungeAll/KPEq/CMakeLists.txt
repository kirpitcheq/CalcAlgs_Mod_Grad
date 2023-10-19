cmake_minimum_required(VERSION 3.0.0)
project(KPEq VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED)
set (CMAKE_CXX_EXTENSIONS OFF)


add_library(${PROJECT_NAME} integral.cpp inc/KPEq/integral.hpp)
find_package(fmt)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)
target_include_directories(${PROJECT_NAME} PUBLIC inc)

# set_property(TARGET ${PROJECT_NAME} CXX_STANDARD 20)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

