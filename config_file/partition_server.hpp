#include <iostream>

class partition_server{

    private:
        std::string host;
        std::string port;
        std::string root;
        std::string max_body_size;
        std::string index;
    public:
        void set_host(std::string );
        void set_port(std::string );
        std::string get_host();
        std::string get_port();
    

};