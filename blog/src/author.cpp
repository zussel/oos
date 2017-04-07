#include "author.hpp"

author::author() {}

author::author(const std::string &name, const matador::date &birthday)
  : person(name, birthday)
{}
