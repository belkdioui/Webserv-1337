#include "config_file.hpp"
#include <fstream>
#include <sstream>

std::vector<partition_server> config_file::get_servers()
{
    return servers;
}

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

int check_location(std::string index, std::string value, location_param &loc)
{
    if (index == "redirect_URL:")
        loc.set_redirect_url(value);
    else if (index == "index:")
        loc.set_index(value);
    else if (index == "methods:")
        loc.set_methods(value);
    else if (index == "upload_dir:")
        loc.set_upload_dir(value);
    else if (index == "directory_listing:")
        loc.set_directory_listing(value);
    else
        return(1);
    return(0);
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
            std::stringstream se(*it);
            se>>index>>value;
            new_server->set_error_pages(index, value);
            while(index == "400:" || index == "403:" || index == "404:" || index == "405:" || index == "413:" || index == "\n")
            {
                it++;
                se.clear();
                se<<(*it);
                se>>index>>value;
                new_server->set_error_pages(index, value);
            }
        }
    }
    else if (index == "location:")
    {
        if(!value.empty())
        {
            std::string save;
            save = value;
            it++;
            std::stringstream sl(*it);
            sl>>index>>value;
            location_param loc;
            check_location(index, value, loc);
            while (index == "redirect_URL:" || index == "index:" || index == "methods:" || index == "directory_listing:" || index == "upload_dir:" || index == "directory_listing:")
            {
                it++;
                std::stringstream sn(*it);
                sn<<(*it);
                sn>>index>>value;
                check_location(index, value, loc);
            }
            new_server->set_location(save, loc);
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
                while(*it != "server:" && it != lines_of_conf.end() && *(it+1) != "server:")
                {
                    check_and_store_data(&new_server, it);                    
                        ++it;
                }
                if (new_server.get_max_body_size().empty())
                    new_server.set_max_body_size("100000");
                servers.push_back(new_server);
                if(it == lines_of_conf.end())
                    return servers;
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
