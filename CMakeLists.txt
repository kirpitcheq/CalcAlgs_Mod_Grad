cmake_minimum_required(VERSION 3.0.0)

# set(ENABLE_BUILD_LW_EXES OFF CACHE BOOL "Enable build executable laboratory work programs")
set(CA_MOD_ENABLE_BUILD_LW_EXES ON)

add_subdirectory(CalcAlgs) 
add_subdirectory(ModGrad) 
add_subdirectory(KPEqLibs)

