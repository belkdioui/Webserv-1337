#ifndef PARTITION_SERVER_HPP
#define PARTITION_SERVER_HPP
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "location_param.hpp"

class partition_server{

    private:
        long int host;
        std::string port;
        std::vector<std::string> ports;
        std::string root;
        std::string max_body_size;
        std::string index;
    public:
        std::vector<std::string> server_name;
        std::map<std::string, location_param> location;
        std::map<std::string, std::string> error_pages;
        partition_server();
        ~partition_server();
        std::string get_location_first(std::string index);
        void set_host(long int );
        void set_port(std::string );
        void set_ports(std::vector<std::string>& Ports);
        void set_root(std::string );
        void set_max_body_size(std::string );
        void set_index(std::string );
        void set_error_pages(std::string, std::string);
        void set_location(std::string, location_param);
        void set_server_name(std::vector<std::string>& Server_name);
        long int get_host();
        std::vector <std::string>& get_server_name();
        std::vector <std::string>& get_ports();
        std::string get_port();
        std::string get_root();
        std::string get_max_body_size();
        std::string get_index();
        std::string get_error_pages(std::string);
        location_param get_location(std::string);
};

#endif