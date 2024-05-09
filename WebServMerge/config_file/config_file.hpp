#include <vector>
#include <iostream>
#include "partition_server.hpp"

class config_file
{
private:
    std::vector<std::string> lines_of_conf;
    std::vector<partition_server> servers;
public:
    config_file(const std::string& name_of_file);
    std::vector<partition_server> get_servers();
    std::vector<partition_server> split_servers(std::vector<std::string> lines_of_conf);
    int check_and_store_data(partition_server *new_server, std::vector<std::string>::iterator it);
    ~config_file();
};
