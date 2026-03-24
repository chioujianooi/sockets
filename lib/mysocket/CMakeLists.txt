option(BUILD_SHARED_LIBS "Build shared libraries" ON)


add_library(mysocket SHARED
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/windows/winsocket.cpp>
    $<$<PLATFORM_ID:LINUX>:${CMAKE_CURRENT_SOURCE_DIR}/src/linux/posixsocket.cpp>
)

set_target_properties(mysocket PROPERTIES
    VERSION 1.0
    SOVERSION 1
)



# Include headers
target_include_directories(mysocket
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/include/windows>
    $<$<NOT:$<PLATFORM_ID:LINUX>>:${CMAKE_CURRENT_SOURCE_DIR}/include/linux>
)

target_link_libraries(mysocket PRIVATE $<$<PLATFORM_ID:Windows>:ws2_32>)

target_compile_definitions(mysocket
    PRIVATE MYSOCKET_EXPORTS
)