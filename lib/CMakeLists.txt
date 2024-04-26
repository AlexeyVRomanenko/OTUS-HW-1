cmake_minimum_required(VERSION 3.29)

project(hw-1_lib VERSION ${PROJECT_VERSION})

add_library(${PROJECT_NAME} STATIC
    lib.h
    lib.cpp
)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()