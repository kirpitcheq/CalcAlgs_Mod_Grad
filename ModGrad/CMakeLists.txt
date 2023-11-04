
if(BUILD_WITH_EXTERNAL )
        add_subdirectory(lw1_ODE) # set build vars for SaTDnSTaD
endif()

add_subdirectory(lw2_ODE_RungeAll)
