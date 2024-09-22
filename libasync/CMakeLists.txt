cmake_minimum_required(VERSION 3.28)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(libasync VERSION ${PROJECT_VERSION} LANGUAGES CXX)

configure_file(version.h.in version.h)

add_library(${PROJECT_NAME} SHARED
    libasync.cpp
)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#set_target_properties(${PROJECT_NAME}
 #   PROPERTIES
#        CXX_VISIBILITY_PRESET hidden
#)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        api
    PRIVATE
        .
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        LIBASYNC
)

add_subdirectory(tests)
add_subdirectory(front)
add_subdirectory(back)
add_subdirectory(tasks_thread)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        front
        back
        tasks_thread
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        back
        front
        tasks_thread
)

#boost
find_package(Boost REQUIRED COMPONENTS system)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)