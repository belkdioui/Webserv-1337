#include "partition_server.hpp"

void partition_server::set_host(std::string Host)
{
    host = Host;
}

void partition_server::set_port(std::string Port)
{
    port = Port;
}

std::string partition_server::get_host()
{
    return host;
}

std::string partition_server::get_port()
{
    return port;
}