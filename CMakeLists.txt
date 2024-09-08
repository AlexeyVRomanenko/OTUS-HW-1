cmake_minimum_required(VERSION 3.28)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(bulk_server VERSION ${PROJECT_VERSION} LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
)

configure_file(version.h.in version.h)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)

add_subdirectory(libasync)
add_subdirectory(bulk_client)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
       libasync
)

#boost
find_package(Boost REQUIRED COMPONENTS date_time asio)
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)

enable_testing()