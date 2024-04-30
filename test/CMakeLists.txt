cmake_minimum_required(VERSION 3.28)

project(test1)

add_executable(${PROJECT_NAME}
    test.cpp
)

target_include_directories(test1
    PRIVATE
        ../lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        library1
)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        #Boost::regex
        Boost::unit_test_framework
)

message(${Boost_INCLUDE_DIR})
message(${Boost_LIBRARY_DIRS})
message(${Boost_VERSION})

add_test(${PROJECT_NAME} ${PROJECT_NAME})