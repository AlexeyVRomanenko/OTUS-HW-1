cmake_minimum_required(VERSION 3.28)

project(tests)

add_executable(${PROJECT_NAME}
    test.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ../my_lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
           my_lib
)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
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