#pragma once

#include <vector>
#include <cstdint>
#include <arpa/inet.h>

class Network {
  private:
    sockaddr_in server_address;

  public:
    Network() {}

    //NOTE: Method declaration must be in sequence, accordingly to network.cpp
    int server_connection();
    std::vector<uint8_t> get_response(const std::vector<uint8_t> &dns_message, const int &sock_fd);
};
