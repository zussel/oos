#include "LoggerTest.hpp"
#include "file.hpp"

#include "matador/logger/file_sink.hpp"
#include "matador/logger/log_manager.hpp"

#include <cstdio>
#include <fstream>
#include <regex>

#ifdef _MSC_VER
#include <io.h>
#else
#include <unistd.h>
#endif

LoggerTest::LoggerTest()
  : unit_test("logger", "logger test")
{
  add_test("file_sink", std::bind(&LoggerTest::test_file_sink, this), "logger file sink test");
  add_test("logger", std::bind(&LoggerTest::test_logger, this), "logger test");
  add_test("logging", std::bind(&LoggerTest::test_logging, this), "logger logging test");
  add_test("stdout", std::bind(&LoggerTest::test_stdout, this), "logger stdout logging test");
  add_test("stderr", std::bind(&LoggerTest::test_stderr, this), "logger stderr logging test");
}

void LoggerTest::test_file_sink()
{
  matador::file_sink test("test.txt");

  UNIT_ASSERT_EQUAL("test.txt", test.path());

  test.close();

  if (::remove("test.txt") == -1) {
#ifdef _MSC_VER
    char buf[1024];
    strerror_s(buf, 1024, errno);
    UNIT_FAIL(buf);
#else
    UNIT_FAIL(strerror(errno));
#endif
  }

  UNIT_ASSERT_FALSE(filehelper::file::exists("test.txt"));
}

void LoggerTest::test_logger()
{
  auto logger = matador::create_logger("test");

  UNIT_ASSERT_EQUAL("test", logger.source());
  UNIT_ASSERT_EQUAL("default", logger.domain());

  auto logsink = std::make_shared<matador::file_sink>("log.txt");

  UNIT_ASSERT_TRUE(filehelper::file::is_readable("log.txt"));
  UNIT_ASSERT_TRUE(filehelper::file::is_writable("log.txt"));

  logsink->close();

  if (::remove("log.txt") == -1) {
#ifdef _MSC_VER
    char buf[1024];
    strerror_s(buf, 1024, errno);
    UNIT_FAIL(buf);
#else
    UNIT_FAIL(strerror(errno));
#endif
  }

  UNIT_ASSERT_FALSE(filehelper::file::exists("log.txt"));

  logger = matador::create_logger("net", "system");

  UNIT_ASSERT_EQUAL("net", logger.source());
  UNIT_ASSERT_EQUAL("system", logger.domain());

  logsink = std::make_shared<matador::file_sink>("net.txt");

  UNIT_ASSERT_TRUE(filehelper::file::is_readable("net.txt"));
  UNIT_ASSERT_TRUE(filehelper::file::is_writable("net.txt"));

  logsink->close();

  ::remove("net.txt");

  UNIT_ASSERT_FALSE(filehelper::file::exists("net.txt"));

  matador::log_manager::instance().clear();
}

void LoggerTest::test_logging()
{
  auto logger = matador::create_logger("test");

  UNIT_ASSERT_EQUAL("test", logger.source());
  UNIT_ASSERT_EQUAL("default", logger.domain());

  auto logsink = std::make_shared<matador::file_sink>("log.txt");

  UNIT_ASSERT_TRUE(filehelper::file::is_readable("log.txt"));
  UNIT_ASSERT_TRUE(filehelper::file::is_writable("log.txt"));

  matador::add_log_sink(logsink);

  logger.info("information");
  logger.warn("warning");
  logger.debug("debugging");
  logger.trace("tracing something");
  logger.error("big error");

  logsink->close();

  validate_log_file_line("log.txt", 0, "INFO", "test", "information");
  validate_log_file_line("log.txt", 1, "WARN", "test", "warning");
  validate_log_file_line("log.txt", 2, "DEBUG", "test", "debugging");
  validate_log_file_line("log.txt", 3, "TRACE", "test", "tracing something");
  validate_log_file_line("log.txt", 4, "ERROR", "test", "big error");


  ::remove("log.txt");

  UNIT_ASSERT_FALSE(filehelper::file::exists("log.txt"));

  matador::log_manager::instance().clear();
}

void LoggerTest::test_stdout()
{
  auto logger = matador::create_logger("test");

  UNIT_ASSERT_EQUAL("test", logger.source());
  UNIT_ASSERT_EQUAL("default", logger.domain());

  auto logsink = std::make_shared<matador::stdout_sink>();

  // Redirect stdout
  {
    filehelper::std_stream_switcher stdout_switcher(stdout, "stdout.txt");
    matador::add_log_sink(logsink);
    logger.info("information");
  }

  validate_log_file_line("stdout.txt", 0, "INFO", "test", "information");

  ::remove("stdout.txt");

  UNIT_ASSERT_FALSE(filehelper::file::exists("stdout.txt"));

  matador::log_manager::instance().clear();
}

void LoggerTest::test_stderr()
{
  auto logger = matador::create_logger("test");

  UNIT_ASSERT_EQUAL("test", logger.source());
  UNIT_ASSERT_EQUAL("default", logger.domain());

  auto logsink = std::make_shared<matador::stderr_sink>();

  // Redirect stdout
  {
    filehelper::std_stream_switcher stdout_switcher(stderr, "stderr.txt");
    matador::add_log_sink(logsink);
    logger.info("information");
  }

  validate_log_file_line("stderr.txt", 0, "INFO", "test", "information");

  ::remove("stderr.txt");

  UNIT_ASSERT_FALSE(filehelper::file::exists("stderr.txt"));

  matador::log_manager::instance().clear();
}

void LoggerTest::validate_log_file_line(const std::string &filename, int line_index, const std::string &level, const std::string &src, const std::string &msg)
{
  std::ifstream logfile(filename);

  UNIT_ASSERT_TRUE(logfile.good());

  std::string line;
  int line_count = 0;
  do {
    if (!std::getline(logfile, line).good()) {
      UNIT_FAIL("couldn't find line");
    }
  } while (line_count++ < line_index);

  const std::regex logline_regex(R"(^\d{4}-\d\d-\d\d \d\d:\d\d:\d\d\.\d{3}.*\[([A-Z]+)\s*\]\[(.*)\]: (.*)$)");
  std::smatch match;

  UNIT_ASSERT_TRUE(std::regex_match(line, match, logline_regex));
  UNIT_ASSERT_EQUAL(4UL, match.size());

  UNIT_ASSERT_EQUAL(level, match[1].str());
  UNIT_ASSERT_EQUAL(src, match[2].str());
  UNIT_ASSERT_EQUAL(msg, match[3].str());
}