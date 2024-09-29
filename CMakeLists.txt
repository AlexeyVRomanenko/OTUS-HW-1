project(prop_react CXX)
set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE .\include)

find_package(Boost REQUIRED)

target_link_libraries(${PROJECT_NAME}
    INTERFACE
        Boost::headers
)

set(CMAKE_FOLDER "${CMAKE_FOLDER}/prop_react")

add_subdirectory(test)