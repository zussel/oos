//
// Created by sascha on 4/7/17.
//

#ifndef MATADOR_COURSE_HPP
#define MATADOR_COURSE_HPP

#include "student.hpp"

#include <matador/utils/identifier.hpp>

#include <matador/object/has_many.hpp>

struct course
{
  course() {}
  course(const std::string &t) : title(t) {}

  template < class SERIALIZER >
  void serialize(SERIALIZER &serializer)
  {
    serializer.serialize("id", id);
    serializer.serialize("title", title);
    serializer.serialize("student_course", students, "student_id", "course_id");
  }

  matador::identifier<unsigned long> id;
  std::string title;
  matador::has_many<student> students;
};

#endif //MATADOR_COURSE_HPP
