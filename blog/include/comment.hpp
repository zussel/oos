#ifndef COMMENT_HPP
#define COMMENT_HPP

#include "matador/utils/identifier.hpp"
#include "matador/utils/varchar.hpp"
#include "matador/utils/time.hpp"

#include "matador/object/belongs_to.hpp"

struct comment
{
  matador::identifier<unsigned long> id;
  matador::varchar<255> email;
  matador::belongs_to<post> blog_post;
  std::string content;
  matador::time created_at;  

  template < class Serializer >
  void serialize(Serializer &serializer)
  {
    serializer.serialize("id", id);
    serializer.serialize("email", email);
    serializer.serialize("post", blog_post, matador::cascade_type::NONE);
    serializer.serialize("content", content);
    serializer.serialize("created_at", created_at);
  }
};

#endif /* COMMENT_HPP */
