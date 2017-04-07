#include "person.hpp"

person::person() {}

person::person(const std::string &n, const matador::date &bd)
  : name(n), birthday(bd)
{}
