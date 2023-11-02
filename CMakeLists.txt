cmake_minimum_required(VERSION 3.0.0)

# set(ENABLE_BUILD_LW_EXES OFF CACHE BOOL "Enable build executable laboratory work programs")
set(CA_MOD_ENABLE_BUILD_LW_EXES ON)

include(FetchContent)
FetchContent_Declare(
    SaDT_n_STaD
    GIT_REPOSITORY https://github.com/kirpitcheq/SaDT_n_STaD.git
    GIT_TAG        RPN_Fix
)
FetchContent_MakeAvailable(SaDT_n_STaD)

add_subdirectory(CalcAlgs) 
add_subdirectory(ModGrad) 
add_subdirectory(KPEqLibs)

