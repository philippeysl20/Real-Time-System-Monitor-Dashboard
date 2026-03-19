# Create libraries for functions in the program
add_library(getSystemMetrics
        getSystemMetrics.c
        getSystemMetrics.h
        GetCPUModelName.h
        GetCPUModelName.c
)

add_library(webSocketEventHandling
        webSocketEventHandling.c
        webSocketEventHandling.h
)

add_library(buildJSON
        buildJSON.c
        BuildJSON.h
)

# Set include directories for each library
target_include_directories(getSystemMetrics PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibraries
)

target_include_directories(webSocketEventHandling PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibraries
)

target_include_directories(buildJSON PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibraries
)

# Link internal dependencies between your own libraries
target_link_libraries(getSystemMetrics PRIVATE buildJSON)
target_link_libraries(webSocketEventHandling PRIVATE getSystemMetrics)

# Link getSystemMetrics directly with PDH (Windows performance counters)
if (WIN32)
    target_link_libraries(getSystemMetrics PRIVATE pdh advapi32)
endif()

# Define the final executable and source files
add_executable(monitor
        main.c
        ExternalLibraries/mongoose.c
        ExternalLibraries/cJSON.c
        GetCPUModelName.h
        GetCPUModelName.c
)

target_include_directories(monitor PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ExternalLibraries
)

# Link libraries to final executable
target_link_libraries(monitor PRIVATE
        getSystemMetrics
        buildJSON
        webSocketEventHandling
)

# Link Windows system libraries
if (WIN32)
    target_link_libraries(monitor PRIVATE ws2_32)
endif()
