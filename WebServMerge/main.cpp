/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:03:47 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/16 10:35:55 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void fun(int i)
{
    (void)i;
}
int main(int ac, char **av)
{
    // int j = 0;
    signal(SIGPIPE, fun);
    if(ac != 2)
    {
        std::cout << "just add the of the config file to the executable" << std::endl;
        exit(0);
    }
    const std::string name = av[1];
    config_file conf(name);
    Server server;
    // std::vector<partition_server> servers = conf.get_servers();
    server.servers = conf.get_servers();
    // for(std::vector<partition_server>::iterator it = server.servers.begin(); it != server.servers.end(); it++)
    // {
    //     std::cout<<it->get_root()<<std::endl;
    // }   
    server.CreatServers();  
    server.run();
    return  0;
    // std::cout<<servers[0].get_port()<<std::endl;

}