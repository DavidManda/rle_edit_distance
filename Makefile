edit: edit_distance.cpp
	g++ edit_distance.cpp rle_string.cpp -o edit_distance.out
clean:
	rm edit_distance
rle_string: rle_string.cpp
	g++ rle_string.cpp -o rle_string.out
lcs: LCS.cpp
	g++ LCS.cpp -o LCS.out