# Copyright (c) 2020 Stella Authors. All rights reserved.

cmake_minimum_required(VERSION 3.17)
project(dy
    VERSION 1.0.0
    DESCRIPTION "JSON-like type library for C"
    HOMEPAGE_URL "https://github.com/stelo-stella/dy"
)
enable_testing()

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Basic settings
set(DY_SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")
set(DY_PUBLIC_DIR "${PROJECT_SOURCE_DIR}/public")
set(DY_PRIVATE_DIR "${PROJECT_SOURCE_DIR}/private")
set(DY_TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")

add_library(dy SHARED
    ${DY_SOURCE_DIR}/dy.cc
)

target_compile_definitions(dy
    PRIVATE -DDY_EXPORT
)

target_include_directories(dy
    PUBLIC  ${DY_PUBLIC_DIR}
    PRIVATE ${DY_PRIVATE_DIR}
)

set(DY_HEADERS
    ${DY_PUBLIC_DIR}/dy.h
)

install(TARGETS dy EXPORT dy DESTINATION "bin")
install(FILES ${DY_HEADERS} DESTINATION "include")

if (DY_TESTS)
    # Download and build googletest
    configure_file(
        cmake/DownloadGoogleTest.cmake
        googletest-download/CMakeLists.txt
    )

    # Download googletest
    execute_process(
        COMMAND ${CMAKE_COMMAND} -G
                "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE GOOGLETEST_RESULT
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
	)
    if (GOOGLETEST_RESULT)
        message(FATAL_ERROR " Failed to download googletest")
    endif()
    message(STATUS " Complete downloading googletest")

    # Build googletest
    execute_process(
        COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE GOOGLETEST_RESULT
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download
	)
    if (GOOGLETEST_RESULT)
        message(FATAL_ERROR " Failed to build googletest")
    endif()
    message(STATUS " Complete building googletest")

    unset(GOOGLETEST_RESULT)

    # Prevent overriding the parent project's compiler/linker settings
    # on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest to dy
    add_subdirectory(
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL
    )

    add_library(dll SHARED ${DY_TESTS_DIR}/dll.c)
    target_link_libraries(dll dy)
    target_compile_definitions(dll
        PRIVATE -DDLL_EXPORT
    )

    function(dy_add_test TEST_NAME)
        set(EXE_NAME dy-test-${TEST_NAME})
        add_executable(${EXE_NAME} ${DY_TESTS_DIR}/${TEST_NAME}.cc)
        target_link_libraries(${EXE_NAME} dll dy gtest_main)
        add_test(NAME ${TEST_NAME} COMMAND ${EXE_NAME})
    endfunction()

    dy_add_test(arrays)
    dy_add_test(generic_arrays)
    dy_add_test(generic_maps)
endif()