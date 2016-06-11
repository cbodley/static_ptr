cmake_minimum_required(VERSION 2.6.2)
project(static_ptr)

# require c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(include)

option(WITH_TESTS "build tests and add 'check' target" ON)
if(WITH_TESTS)
	include_directories(include)
	enable_testing()
	add_subdirectory(test EXCLUDE_FROM_ALL) # build only on 'make check'
endif(WITH_TESTS)
