#ifndef MATADOR_URL_HPP
#define MATADOR_URL_HPP

#include <string>
#include <unordered_map>

namespace matador {
namespace http {

class url
{
public:
  static std::string encode(const std::string &str);
  static bool decode(const std::string &str, std::string &decoded);

private:
  static std::unordered_map<char, const char *> char_to_enc_map_;
};

}
}
#endif //MATADOR_URL_HPP