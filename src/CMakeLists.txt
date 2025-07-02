        # Create libraries for functions in the program
        add_library(getSystemMetrics getSystemMetrics.c
                getSystemMetrics.h)

        # Make header files in current folder available for #include
        target_include_directories(getSystemMetrics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

        # Adding the programs we want to be executable
        add_executable(monitor main.c)

        # Link the executables with the getSystemMetrics library
        target_link_libraries(monitor PRIVATE getSystemMetrics)