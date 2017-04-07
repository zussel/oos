#include <matador/orm/persistence.hpp>
#include <matador/orm/session.hpp>

#include "person.hpp"
#include "author.hpp"
#include "student.hpp"
#include "course.hpp"

using namespace matador;

int main()
{
  std::cout << "starting blog\n";

  persistence p("sqlite://mblog.sqlite");

  p.attach<person>("person", true);
  p.attach<author, person>("author");
  p.attach<comment>("comment");
  p.attach<category>("category");
  p.attach<post>("post");
  p.attach<student, person>("student");
  p.attach<course>("course");

  p.create();

  session s(p);

  transaction tr = s.begin();
  try {
    std::cout << "starting transaction\n";
    auto jane = s.insert(new student("jane"));
    auto art = s.insert(new course("art"));

    art->students.push_back(jane);

    auto me = s.insert(new author("sascha", date(29, 4, 1972)));
    auto main = s.insert(new category("Main"));

    auto first = s.insert(new post("First post", me, "My first post content"));
    first->categories.push_back(main);

    tr.commit();
    std::cout << "finished transaction\n";
  } catch(std::exception &ex) {
    std::cout << "exception caught: " << ex.what() << "\n";
    std::cout << "rolling back transaction\n";
    tr.rollback();
  }
  return 0;
}
