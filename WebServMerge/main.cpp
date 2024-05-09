/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:03:47 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/09 10:03:23 by rarraji          ###   ########.fr       */
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
    std::vector<partition_server> servers = conf.get_servers();
    server.servers = conf.get_servers();
    server.CreatServers();  
    server.run();
    return  0;
    // std::cout<<servers[0].get_port()<<std::endl;

}