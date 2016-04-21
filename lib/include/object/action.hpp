/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ACTION_HPP
#define ACTION_HPP

#ifdef _MSC_VER
  #ifdef oos_EXPORTS
    #define OOS_API __declspec(dllexport)
    #define EXPIMP_TEMPLATE
  #else
    #define OOS_API __declspec(dllimport)
    #define EXPIMP_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
  #define OOS_API
#endif

#include <string>
#include <list>
#include <memory>

namespace oos {

class action_visitor;
class byte_buffer;
class object_store;
class object_proxy;
class object_serializer;

/**
 * @internal
 * @class action
 * @brief Base class for all sql relevant actions
 *
 * The action class is the base class for all sql
 * relevant actions like:
 *  - insert
 *  - update
 *  - delete
 *
 * All these actions are accepted via the action_visitor
 * class, which implements the visitor pattern.
 */
class OOS_API action
{
public:
  typedef void (*t_backup_func)(byte_buffer&, action*, object_serializer &serializer);

public:
  action();
  virtual ~action();
  
  /**
   * Interface to accept the action.
   * 
   * @param av The action_visitor
   */
  virtual void accept(action_visitor *av) = 0;

  virtual void backup(byte_buffer &to) = 0;
  virtual void restore(byte_buffer &from, object_store *store) = 0;

protected:
  static void remove_proxy(object_proxy *proxy, object_store *store);
  static object_proxy* find_proxy(object_store *store, unsigned long id);
  static void insert_proxy(object_store *store, object_proxy *proxy);

protected:
  object_serializer *serializer_;
};

/// @endcond

}

#endif /* ACTION_HPP */
