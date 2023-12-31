cmake_minimum_required(VERSION 3.25.1)

function(add_lib LIB_NAME SRCS INC_DIR TARGET_LIBS)
# set(LIB_NAME "KPEq_RungeOD")
# add_library(${LIB_NAME} inc/KPEq/rungeode.hpp)
    project(${LIB_NAME} VERSION 0.1.0 LANGUAGES C CXX)
    set (CMAKE_CXX_STANDARD 20)
    set (CMAKE_CXX_STANDARD_REQUIRED)
    set (CMAKE_CXX_EXTENSIONS OFF)
    message("sources ${SRCS}")
    add_library(${PROJECT_NAME} ${SRCS})
    target_include_directories(${PROJECT_NAME} PUBLIC ${INC_DIR})
    if(TARGET_LIBS GREATER 0)
        target_link_libraries(${PROJECT_NAME} PUBLIC ${TARGET_LIBS})
    endif()
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

# CMAKE_SOURCE_DIR
find_package(fmt)
# add_lib("KPEq_Integral" "integral.cpp inc/KPEq/integral.hpp" "inc" "fmt::fmt")


#set (CMAKE_CXX_STANDARD 20)
#set (CMAKE_CXX_STANDARD_REQUIRED)
#set (CMAKE_CXX_EXTENSIONS OFF)

project(KPEq_Integral VERSION 0.1.0 LANGUAGES C CXX)
add_library(KPEq_Integral integral.cpp inc/KPEq/integral.hpp)
target_include_directories(KPEq_Integral PUBLIC inc)
target_link_libraries(KPEq_Integral PUBLIC fmt::fmt)
set_target_properties(KPEq_Integral PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
# tests for it
add_subdirectory(tests_integral_kpeq)

project(KPEq_RungeODE VERSION 0.1.0 LANGUAGES C CXX)
add_library(${PROJECT_NAME} inc/KPEq/rungeode.hpp rungeode.cpp)
target_include_directories(KPEq_RungeODE PUBLIC inc)
set_target_properties(KPEq_RungeODE PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

# --------------------------------------------------------------------------------

add_subdirectory(KPEq_Q_ItemModel)
