/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 16:03:47 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/01 21:02:49 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// void fun(int i)
// {
//     (void)i;
// }

int main(int ac, char **av)
{
    std::string name;
    // signal(SIGPIPE, fun);
    if(ac > 2)
        exit(1);
    if(ac == 1)
        name = "./config_file/config_file.badre";
    else
        name = av[1];
    config_file conf(name);
    Server server;
    server.servers = conf.get_servers();
    server.CreatServers();  
    server.run();
    return  0;
}