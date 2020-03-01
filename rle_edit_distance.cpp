#include <boost/intrusive/rbtree.hpp>

namespace bi = boost::intrusive;

class Node : public bi::set_base_hook<>{

public:
  int s;
  Node *left;
  Node *right;

  Node(int _s){
    s = _s;
  }

  friend bool operator< (const Node &a, const Node &b)
    {  return a.s < b.s;  }
  friend bool operator> (const Node &a, const Node &b)
    {  return a.s > b.s;  }
  friend bool operator== (const Node &a, const Node &b)
    {  return a.s == b.s;  }

};

typedef bi::rbtree<Node> FooRbtree;

int main(){
  Node foo_object(1);
  FooRbtree rbtree;

  rbtree.insert_unique(foo_object);

  // assert(&list.front() == &foo_object);
  return 0;
}