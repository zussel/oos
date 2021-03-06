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

#include "matador/db/mssql/mssql_result.hpp"

#include "matador/utils/date.hpp"
#include "matador/utils/time.hpp"
#include "matador/utils/basic_identifier.hpp"

#include <cstring>

namespace matador {

namespace mssql {

mssql_result::mssql_result(SQLHANDLE stmt)
  : affected_rows_(0)
  , rows(0)
  , fields_(0)
  , stmt_(stmt)
{
  // get row and column information
  SQLLEN r(0);
  SQLRETURN ret = SQLRowCount(stmt, &r);
  throw_database_error(ret, SQL_HANDLE_STMT, stmt, "mssql");

  if (r != SQL_ERROR && r >= 0) {
    rows = (unsigned long) r;
  }

  SQLSMALLINT columns = 0;
  ret = SQLNumResultCols(stmt, &columns);
  throw_database_error(ret, SQL_HANDLE_STMT, stmt, "mssql");
}

mssql_result::~mssql_result()
{
  SQLCloseCursor(stmt_);
  SQLFreeStmt(stmt_, SQL_CLOSE);
}

const char* mssql_result::column(size_type) const
{
  return nullptr;
}

bool mssql_result::fetch()
{
//  SQLRETURN ret = SQLSetStmtAttr(stmt_, SQL_ROWSET_SIZE, (void *) 2, SQL_NTS);
//  throw_error(ret, SQL_HANDLE_DBC, stmt_, "mssql", "error on creating sql statement");
//  ret = SQLSetStmtAttr(stmt_, SQL_ATTR_CURSOR_TYPE, (void *) SQL_CURSOR_KEYSET_DRIVEN, SQL_NTS);
//  throw_error(ret, SQL_HANDLE_DBC, stmt_, "mssql", "error on creating sql statement");
//  ret = SQLSetStmtAttr(stmt_, SQL_ATTR_CONCURRENCY, (void *) SQL_CONCUR_LOCK, SQL_NTS);
//  throw_error(ret, SQL_HANDLE_DBC, stmt_, "mssql", "error on creating sql statement");

  SQLRETURN ret = SQLFetch(stmt_);
  if (SQL_SUCCEEDED(ret)) {
    return true;
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
    return false;
  }
}

mssql_result::size_type mssql_result::affected_rows() const
{
  return affected_rows_;
}

mssql_result::size_type mssql_result::result_rows() const
{
  return rows;
}

mssql_result::size_type mssql_result::fields() const
{
  return fields_;
}

int mssql_result::transform_index(int index) const
{
  return ++index;
}

void mssql_result::serialize(const char *id, char &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, short &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, int &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, long &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, long long &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, unsigned char &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, unsigned short &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, unsigned int &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, unsigned long &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, unsigned long long &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, bool &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, float &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, double &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *, char *x, size_t s)
{
  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, result_index_++, SQL_C_CHAR, x, s, &info);
  if (ret == SQL_SUCCESS) {
    return;
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

void mssql_result::serialize(const char *id, std::string &x, size_t s)
{
  read_column(id, x, s);
}

void mssql_result::serialize(const char *id, std::string &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, matador::date &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, matador::time &x)
{
  read_column(id, x);
}

void mssql_result::serialize(const char *id, identifiable_holder &x, cascade_type)
{
  read_foreign_object(id, x);
}

void mssql_result::serialize(const char *id, basic_identifier &x)
{
  x.serialize(id, *this);
}

void mssql_result::read_column(const char *, std::string &val)
{
  char buf[1024];
  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, result_index_++, SQL_C_CHAR, buf, 1024, &info);
  if (SQL_SUCCEEDED(ret)) {
    val.assign(buf, info);
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

void mssql_result::read_column(const char *, std::string &val, size_t s)
{
  std::vector<char> buf(s, 0);
  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, result_index_++, SQL_C_CHAR, buf.data(), s, &info);
  if (SQL_SUCCEEDED(ret)) {
    val.assign(buf.data(), info);
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }

}

void mssql_result::read_column(const char *, char &val)
{
  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, (SQLUSMALLINT)(result_index_++), SQL_C_CHAR, &val, 0, &info);
  if (SQL_SUCCEEDED(ret)) {
    return;
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

void mssql_result::read_column(const char *, unsigned char &val)
{
  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, (SQLUSMALLINT)(result_index_++), SQL_C_CHAR, &val, 0, &info);
  if (SQL_SUCCEEDED(ret)) {
    return;
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

void mssql_result::read_column(char const *, date &x)
{
  SQL_DATE_STRUCT ds;

  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, static_cast<SQLUSMALLINT>(result_index_++), SQL_C_TYPE_DATE, &ds, 0, &info);
  if (SQL_SUCCEEDED(ret)) {
    x.set(ds.day, ds.month, ds.year);
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

void mssql_result::read_column(char const *, time &x)
{
  SQL_TIMESTAMP_STRUCT ts;

  SQLLEN info = 0;
  SQLRETURN ret = SQLGetData(stmt_, static_cast<SQLUSMALLINT>(result_index_++), SQL_C_TYPE_TIMESTAMP, &ts, 0, &info);
  if (SQL_SUCCEEDED(ret)) {
    x.set(ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second, ts.fraction / 1000 / 1000);
  } else {
    throw_database_error(ret, SQL_HANDLE_STMT, stmt_, "mssql");
  }
}

bool mssql_result::prepare_fetch()
{
  if (!fetch()) {
    return false;
  }

  result_index_ = 0;
  return true;
}

bool mssql_result::finalize_fetch()
{
  return true;
}

}

}
