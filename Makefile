edit: edit_distance.cpp
	g++ edit_distance.cpp rle_string.cpp -o edit_distance.out
clean:
	rm edit_distance
rle_string: rle_string.cpp
	g++ rle_string.cpp -o rle_string.out
run_lcs: run_lcs.cpp
	g++ run_lcs.cpp LCS.cpp rle_string.cpp -o run_lcs.out
gen_input: generate_inputs.cpp
	g++ generate_inputs.cpp -o generate_inputs.out
test: LCS_test.cpp
	g++ -std=c++11 LCS_test.cpp LCS.cpp rle_string.cpp -o test.out