# Copyright (c) 2020 Stella Authors. All rights reserved.

cmake_minimum_required(VERSION 3.17)
project(dy-googletest-download)

include(ExternalProject)
ExternalProject_Add(googletest
	GIT_REPOSITORY    https://github.com/google/googletest.git
	GIT_TAG           master
	SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googletest-src"
	BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/googletest-build"
	CONFIGURE_COMMAND ""
	BUILD_COMMAND     ""
	INSTALL_COMMAND   ""
	TEST_COMMAND      ""
)
