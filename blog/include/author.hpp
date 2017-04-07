#ifndef AUTHOR_HPP
#define AUTHOR_HPP

#include "person.hpp"
#include "post.hpp"

#include <matador/object/has_many.hpp>

#include <matador/utils/base_class.hpp>

struct author : public person
{
  // many to one
  matador::has_many<post> posts;

  author();
  author(const std::string &name, const matador::date &birthday);

  template < class Serializer >
  void serialize(Serializer &serializer)
  {
    serializer.serialize(*matador::base_class<person>(this));
    serializer.serialize("post", posts, "author", "id");
  }
};

#endif /* AUTHOR_HPP */