//
// Created by sascha on 19.09.19.
//

#include "HasManyUnitTest.hpp"
#include "../entities.hpp"

#include "matador/object/has_one_to_many_item.hpp"

using namespace matador;

HasManyUnitTest::HasManyUnitTest()
: unit_test("has_many", "has many base unit tests")
{
  add_test("one_to_many", std::bind(&HasManyUnitTest::test_one_to_many, this), "test one to many item");
}

using many_list_varchars = many_builtins<varchar<255>, std::list>;

void HasManyUnitTest::test_one_to_many()
{
  has_one_to_many_item<many_list_varchars, varchar<255>> item;

  UNIT_ASSERT_EQUAL("", item.right());
}
