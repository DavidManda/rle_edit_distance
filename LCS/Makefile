edit: edit_distance.cpp
	g++ edit_distance.cpp rle_string.cpp -o edit_distance.o
clean:
	rm *.o
run_lcs: run_lcs.cpp
	g++ run_lcs.cpp LCS.cpp binary_tree.cpp rle_string.cpp -o run_lcs.o
gen_input: generate_inputs.cpp
	g++ generate_inputs.cpp -o generate_inputs.o
test: LCS_test.cpp
	g++ -std=c++11 LCS_test.cpp LCS.cpp rle_string.cpp -o test.o
binary_tree:
	g++ -std=c++11 binary_tree.cpp -o binary_tree.o