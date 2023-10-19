cmake_minimum_required(VERSION 3.5)

set (CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_subdirectory(libsQt)
add_subdirectory(lib)
add_subdirectory(QtGUI)
add_subdirectory(lib_tests)
