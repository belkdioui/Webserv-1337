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

// #include <dirent.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <limits.h>
// #include <unistd.h>
// #include <cstdio>
// #include <cstring>
// #include <iostream>

// int deletePath(const char* path) {
//     if (std::remove(path) == 0) {
//         std::cout << "Deleted successfully" << std::endl;
//         return 0;
//     } else {
//         std::cerr << "Unable to delete the file or directory" << std::endl;
//         return -1;
//     }
// }

// void removeDirectoryRecursively(const char *path) {
//     DIR *dir;
//     struct dirent *entry;
//     char fullPath[PATH_MAX];
//     if ((dir = opendir(path)) == NULL) {
//         perror("opendir");
//         return;
//     }
//     while ((entry = readdir(dir))!= NULL) {
//         if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
//             snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);
//             if (entry->d_type == DT_DIR) {
//                 removeDirectoryRecursively(fullPath);
//             } else {
//                 unlink(fullPath);
//             }
//         }
//     }
//     closedir(dir);
//     rmdir(path);
// }

// int main() {
//     const char *directoryPath = "/Users/bel-kdio/Desktop/Webserv-1337/solix";
//     if(deletePath(directoryPath)==-1)
//         removeDirectoryRecursively(directoryPath);
//     return 0;
// }