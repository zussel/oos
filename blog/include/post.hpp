#ifndef POST_HPP
#define POST_HPP

#include "category.hpp"
#include "comment.hpp"
#include "author.hpp"

#include "matador/utils/identifier.hpp"
#include "matador/utils/varchar.hpp"
#include "matador/utils/time.hpp"

#include "matador/object/has_many.hpp"
#include "matador/object/belongs_to.hpp"

struct author;

struct post
{
  matador::identifier<unsigned long> id;
  matador::varchar<255> title;
  matador::belongs_to<author> writer;
  matador::time created_at;
  matador::has_many<category> categories;
  matador::has_many<comment> comments;
  matador::has_many<matador::varchar<255>> tags;
  std::string content;

  post() {}

  post(const std::string &title,
       const matador::object_ptr<author> &autr,
       const std::string &cntnt
  );

  template < class Serializer >
  void serialize(Serializer &serializer)
  {
    serializer.serialize("id", id);
    serializer.serialize("title", title);
    serializer.serialize("author", writer, matador::cascade_type::NONE);
    serializer.serialize("created_at", created_at);
    serializer.serialize("post_category", categories, "category_id", "post_id");
    serializer.serialize("comment", comments, "post", "id");
    serializer.serialize("post_tag", tags, "post_id", "tag");
    serializer.serialize("content", content);
  }
};

#endif /* POST_HPP */
