cmake_minimum_required(VERSION 3.28)

project(tests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    test.cpp
)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        matrix
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

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)