#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

namespace utils{
    std::string delete_all_whitespace_and_set_one_space(std::string str);
    void print_error(std::string str, std::string str1);
    std::string trim_space_from_back(std::string str);
}

#endif
