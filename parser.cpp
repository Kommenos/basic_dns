#include "parser.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdio> 

std::string Parser::get_hostname(std::string &url) {
  size_t host_start = url.find(":");

  std::string hostname;
  if(host_start != std::string::npos) {
    hostname = url.erase(0, host_start + 3);
  }
  size_t host_end = url.find("/");
  hostname = url.substr(0, host_end);
  
  std::cout << "Host: " << hostname << "\n";
  return hostname;
}

std::vector<uint8_t> Parser::encode_hostname(std::string &url) {
  std::string hostname = get_hostname(url);

  std::vector<uint8_t> result;
  std::string label;

  for(char c : hostname + ".") {
    if(c == '.') {
      result.push_back((uint8_t)label.size());
      for(char ch : label) 
        result.push_back((uint8_t)ch);
      label.clear();
    }
    else {
      label += c;
    }
  }
  result.push_back(0x00);
  return result;
}

std::vector<uint8_t> Parser::create_message(std::vector<uint8_t> &label) {
  std::vector<uint8_t> message;

  uint8_t header[12] = {
    0xAA, 0xBB, //ID = 1
    0x01, 0x00, //Flags = 1
    0x00, 0x01, //QDCOUNT = 1
    0x00, 0x00, //ANCOUNT
    0x00, 0x00, //NSCOUNT
    0x00, 0x00, //ARCOUNT
  };
  message.insert(message.begin(), std::begin(header), std::end(header));
  message.insert(message.end(), std::begin(label), std::end(label));

  uint8_t question[4] = {
    0x00, 0x01, //QTYPE = 1
    0x00, 0x01, //QCLASS = 1
  };
  message.insert(message.end(), std::begin(question), std::end(question)); 
  question_size = 4; 

  return message;
}

void Parser::response_parser(const std::vector<uint8_t> &response, size_t label_size, size_t question_size) {
  //NOTE: 12 is the size of header section in dns query.
  std::vector<uint8_t> header_section(response.begin(), response.begin() + 12);

  size_t rr_start = 12 + label_size + question_size;
  std::vector<uint8_t> resource_record(response.begin() + rr_start, response.end()); 
  
  size_t offset = 2;

  uint16_t type = (resource_record[offset] << 8) | resource_record[offset + 1]; offset += 2;
  offset += 2; //skip CLASS 
  offset += 4; //skip TTL
  uint16_t rdlength = (resource_record[offset] << 8) | resource_record[offset + 1]; offset += 2;

  if(type == 1 && rdlength == 4) {
    std::string ip = std::to_string(resource_record[offset]) + "." +
      std::to_string(resource_record[offset + 1]) + "." +
      std::to_string(resource_record[offset + 2]) + "." +
      std::to_string(resource_record[offset + 3]);
    
    std::cout << "IP: " << ip;
  }

}


