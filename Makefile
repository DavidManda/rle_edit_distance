edit_dist: rle_edit_distance.cpp
	g++ rle_edit_distance.cpp segment.cpp point.cpp -o rle_edit_distance.o -I boost_1_72_0
example:
	g++ rbtree_example.cpp -o rbtree_example.o -I boost_1_72_0