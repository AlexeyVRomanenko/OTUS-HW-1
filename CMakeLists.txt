cmake_minimum_required(VERSION 3.28)

project(1409 VERSION ${PROJECT_VERSION})

add_executable(${PROJECT_NAME}
    main.cpp
)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(TBB REQUIRED)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        TBB::tbb
        ${Boost_LIBRARIES}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)