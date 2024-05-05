cmake_minimum_required(VERSION 3.28)

project(ip_parser VERSION ${PROJECT_VERSION})

add_library(${PROJECT_NAME}
    ip_parser.h
    ip_parser.cpp
)

find_package(Boost REQUIRED COMPONENTS filesystem)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${Boost_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${Boost_LIBRARIES}
)

include(CPack)