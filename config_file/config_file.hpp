#ifndef CONFIG_FILE_HPP
#define CONFIG_FILE_HPP

#include <vector>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <fstream>
#include <sstream>
#include <regex>
#include "partition_server.hpp"
#include "utils.hpp"

class location_param;

class config_file
{
private:
    std::vector<std::string> lines_of_conf;
    std::vector<partition_server> servers;
public:
    config_file();
    void check_if_duplicate_server();
    void check_if_ports_is_duplicated(partition_server new_server);
    int check_location(std::string index, std::string value, location_param &loc);
    void split_by_char_and_store_in_vector(std::string str, char c, std::vector<std::string> &vect);
    int cal_how_many_strings_in_string(std::string str, char c);
    bool only_whitespace(const std::string& str);
    bool first_char_after_whitespace(const std::string &str, char c);
    void is_valid_host(partition_server *new_server ,const std::string& hostname);
    void multiple_srv_with_with_multiple_port(partition_server);
    config_file(const std::string& name_of_file);
    bool delete_file(std::string name_of_file);
    int convert_string_to_int(std::string str);
    int count_alphabetic_and_check_is_digits(char c, std::string str, int number, int min , int max);
    std::vector<partition_server> get_servers();
    std::vector<partition_server> split_servers(std::vector<std::string> lines_of_conf);
    int check_and_store_data(partition_server *new_server, std::vector<std::string>::iterator& it);
    virtual ~config_file();
};

#endif