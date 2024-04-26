cmake_minimum_required(VERSION 3.29)

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.1.${PATCH_VERSION})

project(hw-01_exe VERSION ${PROJECT_VERSION})

add_executable(${PROJECT_NAME}
    main.cpp
)

configure_file(version.h.in version.h)

set_target_properties(${PROJECT_NAME}
PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_BINARY_DIR}"
)

add_subdirectory(lib)
add_subdirectory(test)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        lib
)

target_include_directories(hw-01_ut
    PRIVATE
        lib
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        hw-1_lib
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()