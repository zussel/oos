#ifndef MATADOR_RESPONSEPARSERTEST_HPP
#define MATADOR_RESPONSEPARSERTEST_HPP

#include "matador/unit/unit_test.hpp"

class ResponseParserTest : public matador::unit_test
{
public:
  ResponseParserTest();

  void test_empty_response();
  void test_body_response();
  void test_from_file_response();
};


#endif //MATADOR_RESPONSEPARSERTEST_HPP