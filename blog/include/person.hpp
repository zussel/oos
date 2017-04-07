#ifndef PERSON_HPP
#define PERSON_HPP

#include <matador/utils/identifier.hpp>
#include <matador/utils/varchar.hpp>
#include <matador/utils/date.hpp>

struct person
{
  matador::identifier<unsigned long> id;
  matador::varchar<255> name;
  matador::date birthday;

  person();
  person(const std::string &n, const matador::date &bd);

  template < class Serializer >
  void serialize(Serializer &serializer)
  {
    serializer.serialize("id", id);
    serializer.serialize("name", name);
    serializer.serialize("birthday", birthday);
  }
};

#endif /* PERSON_HPP */
