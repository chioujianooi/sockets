option(BUILD_SHARED_LIBS "Build shared libraries" ON)



# Create a shared library (DLL on Windows)
add_library(mysocket SHARED
    src/winsocket.cpp
)

set_target_properties(mysocket PROPERTIES
    VERSION 1.0
    SOVERSION 1
)

# Include headers
target_include_directories(mysocket
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(mysocket PRIVATE ws2_32)

target_compile_definitions(mysocket
    PRIVATE MYSOCKET_EXPORTS
)