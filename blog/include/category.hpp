#ifndef CATEGORY_HPP
#define CATEGORY_HPP

#include "post.hpp"

#include "matador/utils/identifier.hpp"
#include "matador/utils/varchar.hpp"

#include "matador/object/has_many.hpp"

struct post;

struct category
{
  matador::identifier<unsigned long> id;
  matador::varchar<255> name;
  std::string description;
  matador::has_many<post> posts;

  category() {}
  category(const std::string &n) : name(n) {}

  template < class Serializer >
  void serialize(Serializer &serializer)
  {
    serializer.serialize("id", id);
    serializer.serialize("name", name);
    serializer.serialize("description", description);
    serializer.serialize("post_category", posts, "category_id", "post_id");
  }
};

#endif /* CATEGORY_HPP */
