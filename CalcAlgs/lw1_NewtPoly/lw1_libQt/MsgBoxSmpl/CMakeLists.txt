set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Charts)

add_library(MsgBoxSmpl
        msgboxsmpl.h msgboxsmpl.cpp
)
target_link_libraries(MsgBoxSmpl PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
)
