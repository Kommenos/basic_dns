#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <unistd.h> 
#include <regex>

#include "parser.hpp" 
#include "network.hpp" 

bool is_url_valid(const std::string &url) {
  const std::regex domain_pattern(
    //NOTE: check for valid domain name
    "^([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+[a-zA-Z]{2,}$"
  );

  const std::regex url_pattern(
      "^(https?:\\/\\/)?"                              
      "([a-zA-Z0-9]([a-zA-Z0-9\\-]{0,61}[a-zA-Z0-9])?\\.)+" 
      "[a-zA-Z]{2,}"                        
      "(:\\d{1,5})?"                        
      "(\\/[-a-zA-Z0-9@:%_\\+.~#?&\\/=]*)?"  
      "$"
  );
  
  if(std::regex_match(url, domain_pattern)) {
    return true;
  }
  else if(std::regex_match(url, url_pattern)) {
    return true;
  }
  else {
    return false;
  }
  
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    std::cerr << "No url provided" << std::endl;
    return 1;
  }

  std::string url = argv[1];

  Parser parser;
  Network network;

  std::vector<uint8_t> encoded_hostname;
  if(is_url_valid(url)) {
    encoded_hostname = parser.encode_hostname(url);
  }
  else {
    std::cerr << "This is not a valid url or domain name" << std::endl;
    return 1;
  }
  std::vector<uint8_t> dns_message = parser.create_message(encoded_hostname);

  int sock_fd = network.server_connection();
  std::vector<uint8_t> response = network.get_response(dns_message, sock_fd);
  /*for(uint8_t parts : response) {
    printf("%02x ", parts);
  }*/

  if(response.empty()) {
    std::cerr << "No response received, check your internet connection\n";
    close(sock_fd);
    return -1;
  }
  parser.response_parser(response, encoded_hostname.size(), parser.get_question_size());
  close(sock_fd);
  
  return 0;
}
