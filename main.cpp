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
}