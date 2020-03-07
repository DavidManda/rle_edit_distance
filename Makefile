edit_dist: rle_edit_distance.cpp
	g++ rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp -o rle_edit_distance.o
example:
	g++ rbtree_example.cpp avl_tree.cpp segment.cpp point.cpp -o rbtree_example.o
tree:
	g++ avl_tree.cpp segment.cpp point.cpp -o avl_tree.o
test:
	g++ test.cpp test_rle_edit_distance.cpp test_avl_tree.cpp rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp -o test.o 