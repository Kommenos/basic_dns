#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <unistd.h> 

#include "parser.hpp" 
#include "network.hpp" 

int main(int argc, char *argv[]) {
  if(argc < 2) {
    std::cerr << "No url provided" << std::endl;
    return 1;
  }

  std::string url = argv[1];

  Parser parser;
  Network network;
  
  std::vector<uint8_t> encoded_hostname = parser.encode_hostname(url);
  std::vector<uint8_t> dns_message = parser.create_message(encoded_hostname);

  int sock_fd = network.server_connection();
  std::vector<uint8_t> response = network.get_response(dns_message, sock_fd);
  /*for(uint8_t parts : response) {
    printf("%02x ", parts);
  }*/
  parser.response_parser(response, encoded_hostname.size(), parser.get_question_size());
  close(sock_fd);
  
  return 0;
}
