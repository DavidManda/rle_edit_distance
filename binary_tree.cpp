#include "binary_tree.hpp"
#include <cstddef>
#include <iostream>

#define COUNT 10
treeNode::treeNode(int _key, int _val, int _x, int _y){
    key = _key;
    val = _val;
    x = _x;
    y = _y;
    left = NULL;
    right = NULL;
}

void treeNode::update_value(int _val, int _x, int _y){
    this->val = _val;
    this->x = _x;
    this->y = _y;
}

void treeNode::insert(int _key, int _val, int _x, int _y){
    if(this->key == _key){
        this->update_value(_val, _x, _y);
        return;
    }
    if(this->key > _key){
        if(this->left == NULL){
            this->left = new treeNode(_key, _val, _x, _y);
            return;
        }
        this->left->insert(_key, _val, _x, _y);
    }
    else{
        if(this->right == NULL){
            this->right = new treeNode(_key, _val, _x, _y);
            return; 
        }
        this->right->insert(_key, _val, _x, _y); 
    }
}

binarySearchTree::binarySearchTree(){
    this->root = NULL;
}

void binarySearchTree::insert(int _key, int _val, int _x, int _y){
    if(this->root == NULL){
        this->root = new treeNode(_key, _val, _x, _y);
        return;
    }
    this->root->insert(_key, _val, _x, _y);
}

// Function to print binary tree in 2D  
// It does reverse inorder traversal  
void print_2D_util(treeNode *root, int space)  
{  
    // Base case  
    if (root == NULL)  
        return;  
  
    // Increase distance between levels  
    space += COUNT;  
  
    // Process right child first  
    print_2D_util(root->right, space);  
  
    // Print current node after space  
    // count  
    std::cout<<"\n";  
    for (int i = COUNT; i < space; i++)  
        std::cout<<" ";  
    std::cout<<root->key<<"\n";  
  
    // Process left child  
    print_2D_util(root->left, space);  
}  

void print_2D(treeNode *root){
    print_2D_util(root, 0);
}

int main(){
    binarySearchTree my_bst;
    my_bst.insert(2,0,0,0);
    my_bst.insert(1,0,0,0);
    my_bst.insert(0,0,0,0);
    my_bst.insert(3,0,0,0);
    print_2D(my_bst.root);
    return 0;
}