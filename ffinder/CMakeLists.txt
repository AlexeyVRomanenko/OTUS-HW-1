cmake_minimum_required(VERSION 3.28)

project(ffinder VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME}
    ffinder.cpp
)

 set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        api
)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC
        pch.h
)

find_package(Boost REQUIRED COMPONENTS filesystem)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        ${Boost_LIBRARIES}
)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)