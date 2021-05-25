#include "matador/orm/persistence.hpp"
#include "matador/orm/session.hpp"

#include "matador/object/json_object_mapper.hpp"

#include "matador/http/http_server.hpp"
#include "matador/http/request.hpp"

#include "matador/logger/log_manager.hpp"

#include "../models/person.hpp"
#include "../models/movie.hpp"

#include "movie_service.hpp"

using namespace matador;
using namespace matador::http;

movie_service::movie_service(matador::http::server &s, matador::persistence &p)
  : log_(matador::create_logger("MovieService"))
  , persistence_(p)
{
  s.on_get("/api/movie", [this](const request &req) {
    return list(req);
  });
  s.on_get("/api/movie/{id: \\d+}", [this](const request &req) {
    return get_movie(req);
  });
  s.on_get("/api/movie/init", [this](const request &req) {
    return initialize(req);
  });
  s.on_post("/api/movie", [this](const request &req) {
    return create_movie(req);
  });
}

matador::http::response movie_service::initialize(const matador::http::request &p)
{
  session s(persistence_);

  object_ptr<person> steven;
  transaction tr = s.begin();
  try {
    steven = s.insert(new person("Steven Spielberg", date(18, 12, 1946)));
    s.insert(new person("George Lucas", date(14, 5, 1944)));
    tr.commit();
  } catch (std::exception &ex) {
    log_.error("Couldn't commit transaction: %s", ex.what());
    tr.rollback();
  }

  tr = s.begin();
  try {
    s.insert(new movie("Jaws", 1974, steven));
    s.insert(new movie("Raiders of the lost Arc", 1984, steven));
    tr.commit();
  } catch (std::exception &ex) {
    log_.error("Couldn't commit transaction: %s", ex.what());
    tr.rollback();
  }

  return response::ok("app initialized", mime_types::TYPE_TEXT_PLAIN);
}

matador::http::response movie_service::list(const matador::http::request &p)
{
  session s(persistence_);

  auto result = s.select<movie>();

  for (const auto &m : result) {
    log(log_level::LVL_INFO, "List", "found movie %s (id: %d)", m->title.c_str(), m->id.value());
  }
  json_object_mapper mapper;

  std::string body = mapper.to_string(result, json_format::pretty);

  return response::ok(body, mime_types::TYPE_APPLICATION_JSON);
}

matador::http::response movie_service::get_movie(const matador::http::request &p)
{
  auto id = std::stoul(p.path_params().at("id"));

  session s(persistence_);
  auto result = s.get<movie>(id);

  json_object_mapper mapper;

  std::string body = mapper.to_string(result, json_format::pretty);

  return response::ok(body, mime_types::TYPE_APPLICATION_JSON);
}

matador::http::response movie_service::create_movie(const request &p)
{
  for (const auto &fdata : p.form_data()) {
    log_.info("form data %s: %s", fdata.first.c_str(), fdata.second.c_str());
  }

  
  session s(persistence_);

  auto dview = s.select<person>();
  auto director = dview.begin().optr();

  auto m = new movie;
  m->title = p.form_data().at("title");
  m->director = director;

  auto tr = s.begin();
  try {
    s.insert(m);
    tr.commit();
  } catch (std::exception &ex) {
    tr.rollback();
  }
  return matador::http::response::redirect("/");
}