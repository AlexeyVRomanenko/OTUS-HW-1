cmake_minimum_required(VERSION 3.28)

project(my_allocator VERSION ${PROJECT_VERSION})

add_library(${PROJECT_NAME} INTERFACE
    my_allocator.h
)
target_include_directories(${PROJECT_NAME} INTERFACE .)

find_package(Boost REQUIRED COMPONENTS filesystem)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)