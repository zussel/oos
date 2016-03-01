//
// Created by sascha on 2/26/16.
//

#include "sql/token.hpp"

namespace oos {
namespace detail {

token::token(basic_dialect::t_token tok)
  : type(tok)
{}

std::string select::compile(basic_dialect &d, token::t_compile_type compile_type)
{
  return d.compile(*this);
}

std::string drop::compile(basic_dialect &d, token::t_compile_type compile_type)
{
  return d.compile(*this);
}

create::create(const std::string &t)
  : token(basic_dialect::CREATE_TABLE), table(t)
{}

std::string create::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += " " + table + " ";
//  return result;
}

insert::insert(const std::string &t)
  : token(basic_dialect::INSERT), table(t)
{}

std::string insert::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += table + " ";
//  return result;
}

update::update(const std::string &t)
  : token(basic_dialect::UPDATE)
{}

std::string update::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += table + " ";
//  return result;
}

remove::remove(const std::string &t)
  : token(basic_dialect::DELETE)
{}

std::string remove::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += table + " ";
//  return result;
}

std::string distinct::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

std::string set::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

std::string asc::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

std::string desc::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

from::from(const std::string &t)
  : token(basic_dialect::FROM), table(t)
{}

std::string from::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += table + " ";
//  return result;
}

limit::limit(size_t lmt)
  : token(basic_dialect::LIMIT), limit_(lmt)
{}

std::string limit::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::stringstream str;
//  str << token::compile(d, compile_type);
//  str << " (" << limit_ << ")";
//  return str.str();
}

order_by::order_by(const std::string &col)
  : token(basic_dialect::ORDER_BY), column(col)
{}

std::string order_by::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

group_by::group_by(const std::string &col)
  : token(basic_dialect::GROUP_BY), column(col)
{}

std::string group_by::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
//  std::string result = token::compile(d, compile_type);
//  result += column + " ";
//  return result;
}

std::string basic_where::compile(basic_dialect &d, t_compile_type compile_type)
{
  d.compile(*this);
}

columns::columns(t_brackets with_brackets)
  : token(basic_dialect::COLUMNS)
  , with_brackets_(with_brackets)
{}

column::column(const std::string &col)
  : token(basic_dialect::COLUMN)
  , name(col)
{}

std::string column::compile(basic_dialect &d, t_compile_type compile_type)
{
  return d.compile(*this);
}

typed_column::typed_column(const std::string &col, data_type_t t, std::size_t idx, bool host)
  : column(col)
  , type(t), index(idx), is_host(host)
{}

std::string typed_column::compile(basic_dialect &d, t_compile_type compile_type)
  {
  return d.compile(*this);
}

std::string basic_value::compile(basic_dialect &d, token::t_compile_type type)
{
  return d.compile(*this);
}

}
}