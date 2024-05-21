#ifndef LOCATION_PARAM_HPP
#define LOCATION_PARAM_HPP

#include <map>
#include <iostream>
#include <sstream>
// #include "config_file.hpp"

// class config_file;

class location_param
{
    private:
        std::string redirect_url;
        std::map<std::string, bool> methods;
        std::string index;
        std::string directory_listing;
        std::string upload_dir;
        std::string root;
    public:
        location_param();
        ~location_param();
        void set_redirect_url(std::string);
        void set_methods(std::string);
        void set_index(std::string);
        void set_directory_listing(std::string);
        void set_upload_dir(std::string);
        void set_root(std::string);
        std::string get_redirect_url();
        bool get_methods(std::string key);
        std::string get_index();
        std::string get_root();
        std::string get_directory_listing();
        std::string get_upload_dir();
};

#endif