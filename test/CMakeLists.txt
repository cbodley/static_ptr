add_subdirectory(googletest)
include_directories(googletest/googletest/include)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

set(tests
	test_conversion
	test_derived_ptr
	test_move_copy
	test_string_ptr
	test_virtual_ptr
	)

foreach(test IN LISTS tests)
	add_executable(${test} ${test}.cc)
	target_link_libraries(${test} gtest_main)
	add_test(${test} ${test})
	add_dependencies(check ${test})
endforeach()
