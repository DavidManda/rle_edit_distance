run_edit: edit_distance.cpp
	g++ run_edit_dist.cpp edit_distance.cpp helper.cpp point.cpp rle_string.cpp -o run_edit.o
gen_input: generate_inputs.cpp
	g++ generate_inputs.cpp -o generate_inputs.o
test: find_failing_test.cpp
	g++ find_failing_test.cpp edit_distance.cpp helper.cpp point.cpp rle_string.cpp -o find_failing_test.o