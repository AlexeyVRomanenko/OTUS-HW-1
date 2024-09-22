cmake_minimum_required(VERSION 3.28)

project(bulk_client LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
)

#boost
find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)