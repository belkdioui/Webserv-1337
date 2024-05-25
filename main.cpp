#include "./config_file/config_file.hpp"

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
    for(std::vector<partition_server>::iterator it = servers.begin(); it != servers.end(); it++)
    {
        std::cout<<"here"<<std::endl;
        std::cout<<it->get_port()<<std::endl;
        std::cout<<"21"<<std::endl;
    }
     
}