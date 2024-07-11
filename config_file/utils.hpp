#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>

namespace utils{
    std::string delete_all_whitespace_and_set_one_space(std::string str);
    void print_error(std::string str, std::string str1);
    std::string trim_space_from_back(std::string str);
    int deletePath(const char* path);
    int removeDirectoryRecursively(const char *path);
    extern std::string big_index;
}

#endif
