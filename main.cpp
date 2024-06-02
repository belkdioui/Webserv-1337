#include "./config_file/config_file.hpp"
#include "./config_file/location_param.hpp"
#include "./config_file/utils.hpp"

int main(int ac, char **av)
{
    if(ac != 2)
    {
        std::cout<<"just add the of the config file to the executable"<<std::endl;
        exit(0);
    }
    const std::string name = av[1];
    config_file conf(name);

    std::vector<partition_server> servers = conf.get_servers();
    utils::trim_space_from_back("  sdfdsfds   sdf   ");
    for(std::vector<partition_server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        std::cout<<"**"<<it->get_location("/site_1/").get_methods("GET")<<"**"<<std::endl;
    }
     
}

// #include <iostream>
// #include <cstdio> 

// bool removeFile(const std::string &filePath) {
//     if (std::remove(filePath.c_str()) == 0) {
//         return true;
//     } else {
//         return false;
//     }
// }

// int main() {
//     std::string path = "/Users/bel-kdio/Desktop/Webserv-1337";
//     if (removeFile(path)) {
//         std::cout << "File successfully deleted." << std::endl;
//     } else {
//         std::cout << "File could not be deleted." << std::endl;
//     }

//     return 0;
// }