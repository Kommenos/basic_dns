#include "network.hpp"

#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

int Network::server_connection() {
  int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd == -1) {
    std::cerr << "Socket creation failed" << std::endl;
    return -1;
  }

  server_address.sin_port = htons(53);
  server_address.sin_family = AF_INET;
  const char *DNS_SERVER_ADDR = "1.1.1.1";  
  server_address.sin_addr.s_addr = inet_addr(DNS_SERVER_ADDR);

  return sock_fd;
}

//DNS query
std::vector<uint8_t> Network::get_response(const std::vector<uint8_t> &dns_message, const int &sock_fd) {
  int send = sendto(sock_fd, dns_message.data(), dns_message.size(), 0, 
    (sockaddr*)&server_address, sizeof(server_address));

  if(send <= 0) {
    std::cerr << "Failed to send Dns Query\n";
    return {};
  }

  std::vector<uint8_t> response(512);
  sockaddr_in from_addr;
  socklen_t addr_len = sizeof(from_addr);

  int receive = recvfrom(sock_fd, response.data(), response.size(), 0, (sockaddr*)&from_addr, &addr_len);
  if(receive <= 0) {
    std::cerr << "Receiving failed\n";
    return {};
  }
  
  response.resize(receive);
  return response;
}


