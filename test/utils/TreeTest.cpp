#include "matador/utils/tree.hpp"
#include "TreeTest.hpp"


#include <string>
#include <list>
#include <iostream>
#include <algorithm>
#include <utility>

using namespace std;
using namespace matador;

typedef tree<string> t_stringtree;
typedef list<string> t_stringlist;

//struct equal_node_name : public unary_function<string, bool> {
template <typename T>
class equal_node_name : public unary_function<T, bool> {
public:
    explicit equal_node_name(T n = "") : name_(std::move(n)) {}
    bool operator() (const T& x) const {
        return x == name_;
    }
private:
    T name_;
};

template < typename T >
struct less_name : public binary_function<typename tree<T>::node_type, typename tree<T>::node_type, bool> {
public:
  bool operator()(const typename tree<T>::node_type &a, const typename tree<T>::node_type &b) const {
    return (a.data < b.data);
  }
};

TreeTest::TreeTest()
  : unit_test("tree", "tree test")
{
  add_test("tree", std::bind(&TreeTest::test_tree, this), "tree test");
}

void TreeTest::test_tree()
{
  t_stringtree stringtree;
  t_stringtree::iterator j, l, m, o, p;

  // add first node
  j = stringtree.insert(stringtree.end(), "hallo");

  l = j;

  cout << "added node <" << *j << ">\n";

  // build up a tree
  stringtree.push_back_child(j, "welt");
  stringtree.push_back_child(j, "mars");
  o = stringtree.push_back_child(j, "mond");
  stringtree.push_back_child(j, "mond");
  stringtree.push_back_child(j, "mond");
  stringtree.push_back_child(j, "mond");
  m = stringtree.push_back_child(j, "mond");
  j = stringtree.push_back_child(j, "saturn");
  stringtree.push_back_child(j, "ring 1");
  p = stringtree.push_back_child(j, "ring 2");
  stringtree.push_back_child(j, "ring 3");

  cout << "size of tree: " << distance(stringtree.begin(), stringtree.end()) << endl;

  // depth counter
  int d = 0;

  for (auto i = stringtree.begin(); i != stringtree.end(); ++i)	{
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  for (t_stringtree::reverse_iterator i = stringtree.rbegin(); i != stringtree.rend(); ++i)	{
    d = stringtree.depth(i.base());
    for (int k = 0; k < d; ++k) cerr << "  ";
    //cout << "node: " << *i << endl;
    cout << "node (depth " << d << "): " << *i << endl;
  }

  cout << "siblings of node <" << *j << ">\n";
  for (auto i = stringtree.begin(j); i != stringtree.end(j); ++i) {
    cout << "sibling node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  // remove last "mond" node
  cout << "erasing node <" << *m << "> ... ";
  stringtree.erase(m);
  cout << "done.\n";

  for (auto i = stringtree.begin(); i != stringtree.end(); ++i)	{
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  // moving node "mond" before node "ring 2"
  cout << "moving node <" << *o << "> ... ";
  stringtree.move(o, p);
  cout << "done.\n";

  for (auto i = stringtree.begin(); i != stringtree.end(); ++i)	{
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  auto i = find_if(stringtree.begin(j), stringtree.end(j), equal_node_name<string>("ring 2"));

  cout << "found node <" << *i << ">\n";

  for (t_stringtree::subtree_iterator i = stringtree.begin_subtree(l); i != stringtree.end_subtree(l); ++i) {
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "subtree node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  t_stringlist sl;
  sl.push_back("hallo");
  sl.push_back("saturn");
  sl.push_back("ring 2");

  cout << "searching for node <";
  copy(sl.begin(), sl.end(), ostream_iterator<string>(cout, "."));
  cout << ">\n";

  j = stringtree.find_in_path(sl.begin(), sl.end(), equal_node_name<string>());

  if (j == stringtree.end())
    cout << "node not found\n";
  else
    cout << "found node <" << *j << ">\n";

  auto c = stringtree.begin();

  j = stringtree.begin();

  cout << "nodes before sorting\n";
  copy(stringtree.begin(j), stringtree.end(j), ostream_iterator<string>(cout, " "));
  cout << "\n";
  stringtree.sort_children(j);
  //  stringtree.sort_children(j, less_name<string>());
  cout << "nodes after sorting\n";
  copy(stringtree.begin(j), stringtree.end(j), ostream_iterator<string>(cout, " "));
  cout << "\n";

  t_stringtree::range_pair rp = stringtree.equal_range(j, equal_node_name<string>("mond"));
  cout << "retrieving equal range for node <mond>\n";
  copy(rp.first, rp.second, ostream_iterator<string>(cout, " "));
  cout << "\n";

  cout << "Sorting the whole tree ... ";
  stringtree.sort();
  cout << "done.\n";

  for (auto i = stringtree.begin(); i != stringtree.end(); ++i)	{
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "node (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  // clear tree
  stringtree.clear();
  // insert root
  j = stringtree.insert(stringtree.begin(), "root 1");

  // build up tree
  auto k = stringtree.push_back_child(j, "branch 1.1");
  m = stringtree.push_back_child(k, "branch 1.1.1");
  stringtree.push_back_child(m, "leaf 1.1.1.1");
  stringtree.push_back_child(m, "leaf 1.1.1.2");
  stringtree.push_back_child(k, "leaf 1.1.2");
  k = stringtree.push_back_child(j, "branch 1.2");
  stringtree.push_back_child(k, "leaf 1.2.1");
  m = stringtree.push_back_child(k, "branch 1.2.2");
  stringtree.push_back_child(m, "leaf 1.2.2.1");
  o = stringtree.push_back_child(m, "branch 1.2.2.2");
  stringtree.push_back_child(o, "leaf 1.2.2.2.1");
  stringtree.push_back_child(o, "leaf 1.2.2.2.2");
  stringtree.push_back_child(k, "leaf 1.2.3");
  k = stringtree.push_back_child(j, "branch 1.3");
  m = stringtree.push_back_child(k, "branch 1.3.1");
  stringtree.push_back_child(m, "leaf 1.3.1.1");
  m = stringtree.push_back_child(k, "branch 1.3.2");
  o = stringtree.push_back_child(m, "branch 1.3.2.1");
  stringtree.push_back_child(o, "leaf 1.3.2.1.1");
  p = stringtree.push_back_child(o, "branch 1.3.2.1.2");
  stringtree.push_back_child(p, "leaf 1.3.2.1.2.1");
  stringtree.push_back_child(m, "leaf 1.3.2.2");

  // print tree
  cout << "New tree:\n";
  for (auto i = stringtree.begin(); i != stringtree.end(); ++i)	{
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "node " << &*i << " (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  cout << "\nIterating over leafs:\n";
  for (auto i = stringtree.begin_leaf(); i != stringtree.end_leaf(); ++i) {
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "leaf node " << &*i << " (depth " << stringtree.depth(i) << "): " << *i << endl;
  }

  cout << "\nIterating backwards over leafs:\n";
  for (auto i = stringtree.end_leaf(); i != stringtree.begin_leaf(); --i) {
    d = stringtree.depth(i);
    for (int k = 0; k < d; ++k) cerr << "  ";
    cout << "leaf node " << &*i << " (depth " << stringtree.depth(i) << "): " << *i << endl;
  }
}
