#pragma once
#include <string>
#include <vector>
#include <cstdint>

class Parser {
  private:
    size_t question_size;
  public:
    Parser() {}
    
    //NOTE: Method initialization must be in sequence, accordingly to parser.cpp
    std::string get_hostname(std::string &url);
    std::vector<uint8_t> encode_hostname(std::string &url);
    std::vector<uint8_t> create_message(std::vector<uint8_t> &label);
    void response_parser(const std::vector<uint8_t> &response, size_t label_size, size_t question_size);

    //getters
    size_t get_question_size() {return question_size;}
};
