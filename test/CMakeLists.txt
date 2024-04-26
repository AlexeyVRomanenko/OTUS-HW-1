cmake_minimum_required(VERSION 3.29)

project(hw-1_ut VERSION ${PROJECT_VERSION})

add_executable(${PROJECT_NAME}
    test.cpp
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}; ${CMAKE_MODULE_PATH}")

find_package(Boost REQUIRED)

#if (Boost_FOUND)
 #   include_directories(${Boost_INCLUDE_DIRS})
 #   link_directories(${Boost_LIBRARY_DIRS})
#endif ()

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        hw-1_lib
)