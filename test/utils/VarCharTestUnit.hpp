#ifndef VARCHARTESTUNIT_HPP
#define VARCHARTESTUNIT_HPP

#include "oos/unit/unit_test.hpp"

class VarCharTestUnit : public oos::unit_test
{
public:
  VarCharTestUnit();

  void test_create();
  void test_copy();
  void test_assign();
  void test_init();
  void test_modify();
  void test_access();
};

#endif /* VARCHARTESTUNIT_HPP */
