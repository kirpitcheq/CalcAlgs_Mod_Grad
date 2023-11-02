cmake_minimum_required(VERSION 3.25.0)
set (CMAKE_CXX_STANDARD 20)
enable_testing()
include(CTest)

include(FetchContent)
FetchContent_Declare(
    gtest
    GIT_REPOSITORY  https://github.com/google/googletest.git
    GIT_TAG         728ec52
)
FetchContent_MakeAvailable(gtest)
include(GoogleTest)

project(lw1_NewtonPolynomTests VERSION 0.1.0 LANGUAGES C CXX)
add_executable(${PROJECT_NAME} newtpoly_tests.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
  KPEq_NewtonPolynom
)
gtest_add_tests(lw1_NewtonPolynomTests .noArgs AUTO)

project(lw1_NewtonPolynomRootsTests VERSION 0.1.0 LANGUAGES C CXX)
add_executable(${PROJECT_NAME} newtpolyroots_tests.cpp)
target_link_libraries(
  ${PROJECT_NAME}
  GTest::gtest_main
  KPEq_NewtonPolynom
)
gtest_add_tests(lw1_NewtonPolynomRootsTests .noArgs AUTO)
