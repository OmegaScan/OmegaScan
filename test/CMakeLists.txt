macro (add_test_exec exec_name)
	add_executable("${exec_name}" "${exec_name}.cc")
	target_include_directories("${exec_name}" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	target_link_libraries("${exec_name}" omegalib)
endmacro (add_test_exec)

add_test_exec(test)
