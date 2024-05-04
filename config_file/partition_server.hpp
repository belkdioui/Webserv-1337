#include <iostream>
#include <map>

class partition_server{

    private:
        std::string host;
        std::string port;
        std::string root;
        std::string max_body_size;
        std::string index;
        std::map<int, std::string> error_pages;
    public:
        partition_server();
        ~partition_server();
        void set_host(std::string );
        void set_port(std::string );
        void set_root(std::string );
        void set_max_body_size(std::string );
        void set_index(std::string );
        void set_error_pages(int, std::string);
        std::string get_host();
        std::string get_port();
        std::string get_root();
        std::string get_max_body_size();
        std::string get_index();
        std::string get_error_pages(int);
};