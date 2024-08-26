cmake_minimum_required(VERSION 3.28)

project(back VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_library(${PROJECT_NAME}
    back.cpp
)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        api
    PRIVATE
        ../front/api
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        tasks_thread
)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC
        pch.h
)
target_link_libraries(${PROJECT_NAME} PRIVATE tasks_thread)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)