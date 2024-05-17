#include <iostream>
#include <map>
#include "location_param.hpp"

class partition_server{

    private:
        std::string host;
        std::string port;
        std::string root;
        std::string max_body_size;
        std::string index;
        std::map<std::string, std::string> error_pages;
        std::map<std::string, location_param> location;
    public:
        partition_server();
        ~partition_server();
        void set_host(std::string );
        void set_port(std::string );
        void set_root(std::string );
        void set_max_body_size(std::string );
        void set_index(std::string );
        void set_error_pages(std::string, std::string);
        void set_location(std::string, location_param);
        std::string get_host();
        std::string get_port();
        std::string get_root();
        std::string get_max_body_size();
        std::string get_index();
        std::string get_error_pages(std::string);
        location_param get_location(std::string);
        std::string get_location_first(std::string index);
};