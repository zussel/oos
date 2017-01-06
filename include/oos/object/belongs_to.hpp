#ifndef OOS_BELONGS_TO_HPP
#define OOS_BELONGS_TO_HPP

#include "oos/object/object_ptr.hpp"

namespace oos {

template < class T >
using belongs_to = object_pointer<T, object_holder_type::BELONGS_TO>;

}

#endif //OOS_BELONGS_TO_HPP