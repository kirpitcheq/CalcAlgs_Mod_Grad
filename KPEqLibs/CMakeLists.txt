add_subdirectory(QtLibs)

project(KPEqLibs VERSION 1.0.0 LANGUAGES CXX)
add_library(${PROJECT_NAME} INTERFACE)
# add_library(${PROJECT_NAME} main.cpp)
add_dependencies(${PROJECT_NAME} KPEq_Integral) 
target_link_libraries(${PROJECT_NAME} INTERFACE
    MsgBoxQKPEq ChartKPEq 
    KPEq_NewtonPolynom 
    KPEq_Integral 
    KPEq_RungeODE
    KPEq_Q_ItemModel
)



if(BUILD_WITH_EXTERNAL )
        target_link_libraries(${PROJECT_NAME} INTERFACE
            KPEq_ODE
        )
endif()
