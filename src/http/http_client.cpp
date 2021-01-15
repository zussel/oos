#include "matador/http/http_client.hpp"
#include "matador/http/http_client_connection.hpp"
#include "matador/http/request.hpp"
#include "matador/http/request_header.hpp"

#include "matador/logger/log_manager.hpp"

#include <chrono>

namespace matador {
namespace http {

client::client(const std::string &host)
  : host_(host)
  , connector_(std::make_shared<connector>())
  , log_(matador::create_logger("HttpClient"))
{
  // split host from port (default port is 80)
  std::vector<std::string> parts;
  auto count = matador::split(host, ':', parts);

  if (count > 2 || count == 0) {
    throw_logic_error("invalid host string");
  }

  if (count == 2) {
    host_ = parts.at(0);
    port_ = parts.at(1);
  }
}

response client::get(const std::string &route)
{
  request_ = request(http::GET, route, host_);

  execute();

  return response_;
}

response client::post(const std::string &route, const std::string &body)
{
  return response();
}

response client::put(const std::string &route, const std::string &body)
{
  return response();
}

response client::remove(const std::string &route)
{
  return response();
}

void client::execute()
{
  service_.connect(connector_, port_, [this](tcp::peer ep, io_stream &stream) {
    // create echo server connection
    auto conn = std::make_shared<http_client_connection>(/*request_, response_, */stream, std::move(ep));
    conn->execute(request_);
  });

  service_.run();
}

}
}