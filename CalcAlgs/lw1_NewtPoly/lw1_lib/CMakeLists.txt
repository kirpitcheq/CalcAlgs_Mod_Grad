cmake_minimum_required(VERSION 3.25.0)
project(lw1_NewtonPolynom VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

# add_subdirectory(modules)

# include(FetchContent)
# FetchContent_Declare(
#     kpeqlib
#     GIT_REPOSITORY  https://github.com/kirpitcheq/SaDT_n_STaD.git
#     GIT_TAG         93bead5

# )
# FetchContent_MakeAvailable(kpeqlib)

add_executable(${PROJECT_NAME} main.cpp)
# target_link_libraries(${PROJECT_NAME} PUBLIC NewtonPolynom)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
