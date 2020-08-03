#ifndef MATADOR_PEER_HPP
#define MATADOR_PEER_HPP

#include "matador/net/address.hpp"

#ifdef _WIN32
#include <winsock2.h>
#else
#include <netinet/in.h>
#endif

#include <cstring>

namespace matador {

template < class P >
class peer_base
{
public:
  typedef P protocol_type;
  typedef sockaddr_in address_type;

//  explicit peer_base(const protocol_type &protocol, unsigned short port = 0)
//  {
//    memset(&sockaddr_, 0, sizeof(sockaddr_));
//    sockaddr_.sin_family = protocol.family();
//    sockaddr_.sin_port = htons(port);
//    sockaddr_.sin_addr.s_addr = INADDR_ANY;
//  }

  explicit peer_base(address addr, unsigned short port = 0)
    : addr_(std::move(addr))
  {
    addr_.port(port);
  }

  peer_base(const peer_base &x)
    : addr_(x.addr_)
  {}

  ~peer_base() = default;

  int port() const { return addr_.port(); }
  protocol_type protocol() const
  {
    if (addr_.is_v4()) {
      return protocol_type::v4();
    } else {
      return protocol_type::v6();
    }
  }

  sockaddr* data()
  {
    return addr_.addr();
  }

  const sockaddr* data() const
  {
    return addr_.addr();
  }

  size_t size() const
  {
    return addr_.size();
  }


private:
  address addr_;
};

}
#endif //MATADOR_PEER_HPP
