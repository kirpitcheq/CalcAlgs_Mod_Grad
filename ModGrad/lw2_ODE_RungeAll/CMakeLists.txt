cmake_minimum_required(VERSION 3.0.0)
project(lw2_ODE_RungeAll VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED)
set (CMAKE_CXX_EXTENSIONS OFF)

message("C++ Standard: ${CMAKE_CXX_STANDARD}")

include(CTest)
enable_testing()

# IntegralLib, Runge lib
add_subdirectory(mod_lw2_lib)

if(CA_MOD_ENABLE_BUILD_LW_EXES)
    add_subdirectory(mod_lw2_QtGUI)
endif()
    # add_subdirectory(mod_lw2_QtGUI)

add_executable(mainprobe main.cpp)
target_link_libraries(mainprobe PUBLIC KPEqLibs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
