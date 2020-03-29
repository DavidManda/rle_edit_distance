edit_dist: rle_edit_distance.cpp
	g++ -std=c++11 -Ofast run_edit_dist.cpp rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp helper.cpp rle.cpp -o run_edit_distance.o
example:
	g++ rbtree_example.cpp avl_tree.cpp segment.cpp point.cpp -o rbtree_example.o
tree:
	g++ avl_tree.cpp segment.cpp point.cpp -o avl_tree.o
test:
	g++ -std=c++11 test.cpp test_rle_edit_distance.cpp test_avl_tree.cpp rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp -o test.o 
find_test:
	g++ -std=c++11 find_failing_test.cpp rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp helper.cpp rle.cpp -o find_test.o
gen_input:
	g++ -std=c++11 generate_inputs.cpp -o gen_input.o
read_string:
	g++ -std=c++11 read_string.cpp -o read_string.o
measure_runtime:
	g++ -std=c++11 measure_runtime.cpp rle_edit_distance.cpp segment.cpp point.cpp avl_tree.cpp helper.cpp rle.cpp -o measure_runtime.o