#include "post.hpp"

post::post(const std::string &ttle,
     const matador::object_ptr<author> &autr,
     const std::string &cntnt
)
  : title(ttle), writer(autr), content(cntnt)
{}
