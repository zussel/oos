//
// Created by sascha on 4/7/17.
//

#ifndef MATADOR_STUDENT_HPP
#define MATADOR_STUDENT_HPP

#include "person.hpp"

#include <matador/object/has_many.hpp>

struct course;

struct student : public person
{
  student() {}
  student(const std::string &name, const matador::date &bdate = matador::date()) : person(name, bdate) {}

  template < class SERIALIZER >
  void serialize(SERIALIZER &serializer)
  {
    serializer.serialize(*matador::base_class<person>(this));
    serializer.serialize("student_course", courses, "student_id", "course_id");
  }

  matador::has_many<course> courses;
};

#endif //MATADOR_STUDENT_HPP
