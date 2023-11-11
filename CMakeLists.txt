cmake_minimum_required(VERSION 3.0.0)

# set(ENABLE_BUILD_LW_EXES OFF CACHE BOOL "Enable build executable laboratory work programs")
set(CA_MOD_ENABLE_BUILD_LW_EXES ON)


option(BUILD_WITH_EXTERNAL
    "Download project SaDT_n_STaD,
    get from it revpolnot lib, and connect several libs to it
    and to KPEqLib intermidiate from other libs"
    OFF
)

if(BUILD_WITH_EXTERNAL)
        include(FetchContent)
        FetchContent_Declare(
            SaDT_n_STaD
            GIT_REPOSITORY https://github.com/kirpitcheq/SaDT_n_STaD.git
            GIT_TAG        RPN_Fix
        )
        FetchContent_MakeAvailable(SaDT_n_STaD)
endif()


option(BUILD_TESTS_KPEq_CAModGrad
    "Build tests using gtest (may be conflicts with External Projects which including gtest framework too)"
    OFF
)

if(BUILD_TESTS_KPEq_CAModGrad)
        include(FetchContent)
        FetchContent_Declare(
            gtest
            GIT_REPOSITORY  https://github.com/google/googletest.git
            GIT_TAG         728ec52
        )
        FetchContent_MakeAvailable(gtest)
endif()

add_subdirectory(CalcAlgs)
add_subdirectory(ModGrad) 
add_subdirectory(KPEqLibs)

