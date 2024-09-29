project(prop_react_ut CXX)
set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME}
    property_test.cpp
)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        prop_react
        Boost::unit_test_framework
)

enable_testing()
add_test(${PROJECT_NAME} ${PROJECT_NAME})