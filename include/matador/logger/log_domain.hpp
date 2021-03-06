#ifndef MATADOR_LOG_DOMAIN_HPP
#define MATADOR_LOG_DOMAIN_HPP

#ifdef _MSC_VER
#ifdef matador_logger_EXPORTS
#define OOS_LOGGER_API __declspec(dllexport)
#define EXPIMP_LOGGER_TEMPLATE
#else
#define OOS_LOGGER_API __declspec(dllimport)
#define EXPIMP_LOGGER_TEMPLATE extern
#endif
#pragma warning(disable: 4251)
#else
#define OOS_LOGGER_API
#endif

#include "matador/logger/log_sink.hpp"
#include "matador/logger/log_level.hpp"

#include <string>
#include <list>
#include <map>

namespace matador {

/**
 * @brief Connection to a set of log sinks
 *
 * A log domain is the connection point between
 * a set of log sinks and the logger objects
 * in the user code.
 *
 * A domain consists of a unique name and a
 * list of sinks
 */
class OOS_LOGGER_API log_domain
{
public:
  /**
   * The timeformat for each log line
   */
  static constexpr const char *TIMESTAMP_FORMAT = "%Y-%m-%d %H:%M:%S.%f";

  /**
   * Creates a log_domain with the given name
   *
   * @param name The name of the log domain
   */
  explicit log_domain(std::string name);

  /**
   * Returns the name of the domain
   *
   * @return The name of the domain
   */
  std::string name() const;

  /**
   * Add a sink to the domain.
   *
   * The sink must be packed into a std::shared_ptr
   * because it can be shared among other domains
   *
   * @param sink The sink to add
   */
  void add_sink(sink_ptr sink);

  /**
   *
   * @param lvl
   * @param source
   * @param message
   */
  void log(log_level lvl, const std::string &source, const char *message);

  /**
   * Clears the list of log sinks
   */
  void clear();

private:

  static std::map<log_level, std::string> level_strings;

  std::string name_;
  std::list<sink_ptr> sinks;
};

}
#endif //MATADOR_LOG_DOMAIN_HPP
