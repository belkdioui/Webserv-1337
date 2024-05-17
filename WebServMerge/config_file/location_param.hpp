#include <map>
#include <iostream>

class location_param{
    private:
        std::string redirect_url;
        std::string methods;
        std::string index;
        std::string directory_listing;
        std::string upload_dir;
    public:
        std::map<std::string, bool> MapMethods;
        location_param();
        ~location_param();
        void set_redirect_url(std::string);
        void set_methods(std::string);
        void set_index(std::string);
        void set_directory_listing(std::string);
        void set_upload_dir(std::string);
        std::string get_redirect_url();
        std::string get_methods();
        std::string get_index();
        std::string get_directory_listing();
        std::string get_upload_dir();
};