#include "partition_server.hpp"

partition_server::partition_server()
{
    
}

partition_server::~partition_server()
{
    
}

void partition_server::set_host(std::string Host)
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

std::string partition_server::get_host()
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