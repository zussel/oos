#include "PrototypeTreeTest.hpp"
#include "../Item.hpp"

#include "object/prototype_tree.hpp"

#include <iostream>

using namespace oos;
using namespace std;

PrototypeTreeTestUnit::PrototypeTreeTestUnit()
  : unit_test("tree", "Prototype Tree Test Unit")
{
  add_test("empty", std::tr1::bind(&PrototypeTreeTestUnit::test_empty, this), "test empty prototype tree");
  add_test("insert", std::tr1::bind(&PrototypeTreeTestUnit::test_insert, this), "test insert element");
  add_test("insert_template", std::tr1::bind(&PrototypeTreeTestUnit::test_insert_by_template, this), "test insert element by template arguments");
  add_test("find", std::tr1::bind(&PrototypeTreeTestUnit::test_find, this), "test find element");
  add_test("remove", std::tr1::bind(&PrototypeTreeTestUnit::test_remove, this), "test remove element");
  add_test("decrement", std::tr1::bind(&PrototypeTreeTestUnit::test_decrement, this), "test decrement iterator");
  add_test("count", std::tr1::bind(&PrototypeTreeTestUnit::test_count, this), "test count of prototypes");
  add_test("child_of", std::tr1::bind(&PrototypeTreeTestUnit::test_child_of, this), "test child of element");
  add_test("traverse", std::tr1::bind(&PrototypeTreeTestUnit::test_traverse, this), "test traversing the prototype tree");
}

PrototypeTreeTestUnit::~PrototypeTreeTestUnit()
{}

void PrototypeTreeTestUnit::initialize()
{}

void PrototypeTreeTestUnit::finalize()
{}

void PrototypeTreeTestUnit::test_empty()
{
  prototype_tree ptree;
  UNIT_ASSERT_TRUE(ptree.empty(), "prototype must be empty");
  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)0, "prototype size must be zero (0)");
}

void PrototypeTreeTestUnit::test_insert()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");
}


void PrototypeTreeTestUnit::test_insert_by_template()
{
  prototype_tree ptree;
  ptree.insert<Item>("item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");

  ptree.insert<ItemA, Item>("item_a", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)2, "prototype size must be one (2)");
}

void PrototypeTreeTestUnit::test_find()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");

  prototype_iterator elem = ptree.find("item");

  UNIT_ASSERT_TRUE(elem != ptree.end(), "couldn't find prototype");
  UNIT_ASSERT_EQUAL(elem->type, "item", "type must be 'item'");

  elem = ptree.find<Item>();

  UNIT_ASSERT_TRUE(elem != ptree.end(), "couldn't find prototype");
  UNIT_ASSERT_EQUAL(elem->type, "item", "type must be 'item'");
}


void PrototypeTreeTestUnit::test_remove()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");

}

void PrototypeTreeTestUnit::test_decrement()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");

  prototype_iterator i = ptree.end();

  --i;
  --i;

  UNIT_ASSERT_TRUE(i == ptree.begin(), "iterator must be begin");
}

void PrototypeTreeTestUnit::test_count() {
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);
  ptree.insert(new object_producer<ItemA>, "item_a", false, "item");
  ptree.insert(new object_producer<ItemB>, "item_b", false, "item");
  ptree.insert(new object_producer<ItemC>, "item_c", false, "item");

  UNIT_ASSERT_EQUAL(ptree.prototype_count(), (size_t)5, "prototype size must be one (5)");
}

void PrototypeTreeTestUnit::test_child_of()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);

  UNIT_ASSERT_EQUAL(ptree.size(), (size_t)1, "prototype size must be one (1)");

  prototype_iterator root = ptree.begin();

  prototype_iterator item = ptree.find<Item>();

  UNIT_ASSERT_FALSE(root->is_child_of(item.get()), "root must not be child of node");

  UNIT_ASSERT_TRUE(item->is_child_of(root.get()), "node must be child of root");
}

void PrototypeTreeTestUnit::test_traverse()
{
  prototype_tree ptree;
  ptree.insert(new object_producer<Item>, "item", false);
  ptree.insert(new object_producer<ItemA>, "item_a", false, "item");
  ptree.insert(new object_producer<ItemB>, "item_b", false, "item");
  ptree.insert(new object_producer<ItemC>, "item_c", false, "item");

  prototype_iterator first = ptree.begin();
  prototype_iterator last = ptree.end();
  int count(0);

  while (first != last) {
    UNIT_ASSERT_LESS(count, 5, "prototype count isn't valid");
    ++first;
    ++count;
  }

  UNIT_ASSERT_EQUAL(count, 5, "expected prototype size isn't 4");
}
