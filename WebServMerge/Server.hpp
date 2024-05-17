/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:14 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/16 10:31:47 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <utility>
#include <cstdio>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <vector> // Pour gérer plusieurs sockets
#include <map>   
#include <fstream> 
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "infoserv.hpp"
#include "ParseServer.hpp"
#include "./config_file/config_file.hpp"

#define PORT_1   8002
#define PORT_2   8004 
#define PORT_3   8005

class infoserv;

// class response;
class Server 
{
  private:
    // int server_socket_1;
    // int server_socket_2;
    // int server_socket_3;
    std::vector<ParseServer> serverSockets;
    int create_server_socket(int port, std::string ip, std::string root, std::string host);
    void accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max);
    void read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds);
    void parse_req(std::string buffer);
    // void readLine(int socket, fd_set &read_fds, fd_set &write_fds);
    // void readBody(int socket, std::string& body);
    
  public:
    std::vector<partition_server> servers;
    std::vector<int> Sockets;
    Server();
    void CreatServers();
    // void RemplirContentType();
    int CheckIsMyServer(int nb);
    void run();
    void RemplirInfo(int socket_fd, std::string root, std::string host);
    void checkResponse(int socket_fd, std::string host, std::string port);
    std::map <int, infoserv> mapinfo;
    
};

#endif