# Create libraries for functions in the program
add_library(getSystemMetrics getSystemMetrics.c getSystemMetrics.h)
add_library(webSocketEventHandling webSocketEventHandling.h
        webSocketEventHandling.c)
add_library(buildJSON BuildJSON.h
        buildJSON.c)


# Make header files in current folder and mongoose folder available for #include
target_include_directories(getSystemMetrics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/mongoose
)

# Add the executable and include mongoose.c as part of the build
add_executable(monitor
        main.c
        mongoose/mongoose.c
)

# Link the executable with the getSystemMetrics library
target_link_libraries(monitor PRIVATE getSystemMetrics)
