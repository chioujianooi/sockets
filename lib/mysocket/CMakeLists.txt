cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
project(mysocket VERSION 1.0)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)




add_library(mysocket SHARED
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/windows/winsocket.cpp>
    $<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/src/linux/posixsocket.cpp>
)

set_target_properties(mysocket PROPERTIES
    VERSION 1.0
    SOVERSION 1
    DEBUG_POSTFIX _d
)



# Include headers
target_include_directories(mysocket
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:$<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/include/windows>>
    $<BUILD_INTERFACE:$<$<PLATFORM_ID:Linux>:${CMAKE_CURRENT_SOURCE_DIR}/include/linux>>
    $<INSTALL_INTERFACE:include>
    $<$<PLATFORM_ID:Windows>:$<INSTALL_INTERFACE:include/windows>>
    $<$<PLATFORM_ID:Linux>:$<INSTALL_INTERFACE:include/linux>>
)

target_link_libraries(mysocket PRIVATE $<$<PLATFORM_ID:Windows>:ws2_32>)

target_compile_definitions(mysocket
    PRIVATE MYSOCKET_EXPORTS
)


install(TARGETS mysocket
    EXPORT mysocketTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    INCLUDES DESTINATION include
)

install(DIRECTORY include/
    DESTINATION include
)

install(EXPORT mysocketTargets
    FILE mysocketTargets.cmake
    NAMESPACE mysocket::
    DESTINATION lib/cmake/mysocket
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/mysocketConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/mysocketConfig.cmake
    INSTALL_DESTINATION lib/cmake/mysocket
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/mysocketConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/mysocketConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/mysocketConfigVersion.cmake
    DESTINATION lib/cmake/mysocket
)

install(FILES
    $<TARGET_PDB_FILE:mysocket>
    DESTINATION bin
    OPTIONAL
)