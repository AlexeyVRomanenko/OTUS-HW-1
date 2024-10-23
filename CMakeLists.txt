cmake_minimum_required(VERSION 3.28)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 1.1.${PATCH_VERSION})

project(prop_react VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)

configure_file(version.h.in version.h)

target_include_directories(${PROJECT_NAME} INTERFACE ./include)

find_package(Boost REQUIRED)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        Boost::headers
)

set(CMAKE_FOLDER "${CMAKE_FOLDER}/prop_react")

add_subdirectory(test)

install(TARGETS ${PROJECT_NAME}
    INCLUDES DESTINATION include
)

install(DIRECTORY include/prop_react/
    DESTINATION include/prop_react
)

enable_testing()