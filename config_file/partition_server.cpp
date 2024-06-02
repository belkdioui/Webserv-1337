#include "partition_server.hpp"

partition_server::partition_server()
{
    host = -1;
}

partition_server::~partition_server()
{
    
}

void partition_server::set_host(long int Host)
{
    host = Host;
}

void partition_server::set_port(std::string Port)
{
    port = Port;
}

void partition_server::set_root(std::string Root)
{
    root = Root;
}

void partition_server::set_max_body_size(std::string Max_body_size)
{
    max_body_size = Max_body_size;
}

void partition_server::set_index(std::string Index)
{
    index = Index;
}

void partition_server::set_error_pages(std::string index, std::string value)
{
    error_pages[index] = value;
}


void partition_server::set_server_name(std::vector<std::string>& Server_name)
{
    server_name = Server_name;
}

void partition_server::set_ports(std::vector<std::string>& Ports)
{
    ports = Ports;
}


void partition_server::set_location(std::string index, location_param value)
{
    if(location.find(index) != location.end())
        utils::print_error("duplicate location : ", index);
    location[index] = value;
}

long int partition_server::get_host()
{
    return host;
}

std::string partition_server::get_port()
{
    return port;
}

std::string partition_server::get_root()
{
    return root;
}

std::string partition_server::get_max_body_size()
{
    return max_body_size;
}

std::string partition_server::get_index()
{
    return index;
}

std::vector<std::string>& partition_server::get_server_name()
{
    // std::cout<<server_name[1]<<std::endl;
    return server_name;
}

std::vector<std::string>& partition_server::get_ports()
{
    // std::cout<<server_name[1]<<std::endl;
    return ports;
}

std::string partition_server::get_error_pages(std::string key)
{
    std::map<std::string, std::string>::iterator it = error_pages.find(key);
    if (it == error_pages.end())
        return "";
    return it->second;
}

location_param partition_server::get_location(std::string key)
{
    std::map<std::string, location_param>::iterator it = location.find(key);
     if (it == location.end())
     {
        std::string path;
        std::string save;
        int i = 0;
        for(it = location.begin(); it != location.end(); it++)
        {
            if(!key.find(it->first))
            {
                if(i == 0)
                    save = it->first;
                else
                {
                    if(save.size() <= it->first.size())
                        save = it->first;
                }
            }
            i++;
        }
        it = location.find(save);
        if (save.empty())
        {
            location_param loc;
            loc.set_methods("GET");
            set_location("/", loc);
            return loc;
        }
        return it->second;
     }
    return it->second;
}

std::string partition_server::get_location_first(std::string index)
{
    std::map<std::string, location_param>::iterator it = location.find(index);
    return it->first;
}