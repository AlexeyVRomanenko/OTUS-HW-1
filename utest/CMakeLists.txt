cmake_minimum_required(VERSION 3.28)

project(utest)

add_executable(${PROJECT_NAME}
    test.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ../ip_parser
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ip_parser
)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Boost_LIBRARIES}
)

message(${Boost_INCLUDE_DIR})
message(${Boost_LIBRARY_DIRS})
message(${Boost_VERSION})