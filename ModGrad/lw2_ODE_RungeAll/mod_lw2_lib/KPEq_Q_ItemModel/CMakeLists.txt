cmake_minimum_required(VERSION 3.14)

project(KPEq_Q_ItemModel LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_library(KPEq_Q_ItemModel STATIC
  itemmodel.cpp
  itemmodel.h
)

target_link_libraries(KPEq_Q_ItemModel PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

target_compile_definitions(KPEq_Q_ItemModel PRIVATE KPEQ_Q_ITEMMODEL_LIBRARY)

make_directory(${CMAKE_CURRENT_SOURCE_DIR}/inc/KPEq_Q_Libs)
target_include_directories(KPEq_Q_ItemModel PUBLIC inc)
add_custom_target(
    TARGET KPEq_Q_ItemModel POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        itemmodel.h
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/KPEq_Q_Libs/
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/itemmodel.h
)
#file(COPY itemmodel.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/inc/KPEq_Q_Libs)

# ------------------------------------------------------------------------------
add_subdirectory(KPEq_Q_ItemModel_example/)
