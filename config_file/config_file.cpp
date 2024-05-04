#include "config_file.hpp"
#include <fstream>
#include <sstream>

int cal_num_of_server(std::vector<std::string> lines_of_conf)
{
    int num = 0;
    for (std::vector<std::string>::iterator it= lines_of_conf.begin(); it != lines_of_conf.end(); ++it)
    {
            if (*it == "server:")
                num++;
    }
    return (num);
}



int config_file::check_and_store_data(partition_server *new_server, std::vector<std::string>::iterator it)
{
    std::stringstream ss(*it);
    std::string index;
    std::string value;

    ss>>index>>value;
    if (index == "host:")
        new_server->set_host(value);
    else if (index == "port:")
        new_server->set_port(value);
    else if (index == "root:")
        new_server->set_root(value);
    else if (index == "max_body_size:")
        new_server->set_max_body_size(value);
    else if (index == "index:")
        new_server->set_index(value);
    else if (index == "error_pages:")
    {
        if(value.empty())
        {
            it++;
            std::cout << "after: " <<  *it << std::endl;
            std::stringstream sl(*it);
            sl>>index>>value;
            std::cout<<value<<std::endl;
            std::cout<<"****"<<index<<"****"<<std::endl;
            while(index == "400:" || index == "403:" || index == "404:" || index == "405:" || index == "413:" || index == "\n")
            {
                std::cout<<"wwwwwwwwwwwwwwwwwwwwww"<<value<<std::endl;
            }
        }
    }
    return(0);
}

std::vector<partition_server> config_file::split_servers(std::vector<std::string> lines_of_conf)
{
    int num_of_server = cal_num_of_server(lines_of_conf);
    if (num_of_server > 0)
    {
        for (std::vector<std::string>::iterator it= lines_of_conf.begin(); it != lines_of_conf.end(); ++it)
        {
            if (*it == "server:")
            {
                partition_server new_server;
                it++;
                while(*it != "server:" && it != lines_of_conf.end())
                {
                    check_and_store_data(&new_server, it);
                    it++;
                }
                if (new_server.get_max_body_size().empty())
                    new_server.set_max_body_size("100000");
                servers.push_back(new_server);
                if(it == lines_of_conf.end())
                    exit(0);
            }
        }
    }
    return servers;
}

config_file::config_file(const std::string& name_of_file)
{
    std::string line;
    std::ifstream file(name_of_file);

    if(!file.is_open())
    {
        throw std::runtime_error("file doesn't exist");
    }
    while (std::getline(file, line))
        lines_of_conf.push_back(line);
    split_servers(lines_of_conf);

}

config_file::~config_file()
{
}
