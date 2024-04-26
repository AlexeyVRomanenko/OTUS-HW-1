cmake_minimum_required(VERSION 3.28)

project(library1 VERSION ${PROJECT_VERSION})

add_library(${PROJECT_NAME}
    lib.h
    lib.cpp
)