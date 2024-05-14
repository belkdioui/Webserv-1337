/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/09 09:47:00 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <thread>

Server::Server() 
{
    // server_socket_1 = create_server_socket(PORT_1);
    // server_socket_2 = create_server_socket(PORT_2);
    // server_socket_3 = create_server_socket(PORT_3);
    // int NumberServerSocker = 3;
    // for (size_t i = 0; i < NumberServerSocker; i++)
    // {
    //     int server_socket_1 = create_server_socket(serverSockets[i].port);
    // }
    
}


int convertStringToInt(const std::string& str) 
{
  std::stringstream ss(str);
  int result;
  
  if (ss >> result) 
    return result;
  return(0);  
}

void Server::CreatServers() 
{
    // this->servers.size() = 3;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        int server_socket_1 = create_server_socket(convertStringToInt(this->servers[i].get_port()), (this->servers[i].get_host()));
        Sockets.push_back(server_socket_1);
    }
}




void Server::RemplirInfo(int socket_fd)
{
    infoserv info;
    // set root
    // set host
    // set port
    // set location
    // set error page
    // set index
    // set maxBodySize
    mapinfo.insert(std::make_pair(socket_fd, info));
}

// add ip addresse;
int Server::create_server_socket(int port, std::string ip) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;
    (void) ip;

    memset(&sa, 0, sizeof sa);
    // sa.sin_family = AF_INET; 
    // sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);   //ip server
    // // sa.sin_addr.s_addr = inet_addr(ip.c_str()); //change ip 0.0.0.0 to myip
    // sa.sin_port = htons(port); // port server
    sa.sin_family = AF_INET;
    if (inet_pton(AF_INET, ip.c_str(), &sa.sin_addr) <= 0) 
    {
        // Handle error
        return -1;
    }
    sa.sin_port = htons(port);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
    {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return (-1);
    }
    RemplirInfo(socket_fd);
    // mapinfo.insert(std::make_pair(socket_fd, info));
    std::cout << "[Server] Created server socket fd: " << socket_fd << std::endl;

    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status !=   0) 
    {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "[Server] Bound socket to localhost port " << port << std::endl;

    status = listen(socket_fd,   10);
    if (status !=   0) 
    {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "[Server] Listening on port " << port << std::endl;
    return (socket_fd);
}

void Server::accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max) 
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd == -1) 
    {
        std::cerr << "[Server] Accept error: " << strerror(errno) << std::endl;
        return;
    }
    FD_SET(client_fd, &read_fds);
    if (client_fd > *fd_max) 
        *fd_max = client_fd;
    std::cout << "[Server] Accepted new connection on client socket " << client_fd << ".\n";
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    mapinfo[socket].request.Check_read(socket, read_fds, write_fds);
}

int Server::CheckIsMyServer(int nb)
{
  for (size_t i = 0; i < Sockets.size(); i++)
  {
    std::cout << Sockets[i] << std::endl;
    if (nb == Sockets[i])
        return(1);
  }
  return(0);
//   exit(1);
}

void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);


    // boucler sur les socket
    for (size_t i = 0; i < Sockets.size(); i++)
    {
        std::cout << Sockets[i] << std::endl;
        FD_SET(Sockets[i], &read_fds);
    }
    
    
    // FD_SET(server_socket_1, &read_fds);
    // FD_SET(server_socket_2, &read_fds);
    // FD_SET(server_socket_3, &read_fds);

    // fd_max = std::max(server_socket_1, std::max(server_socket_2, server_socket_3));

    //fd_max les socket 
    for (size_t i = 0; i < Sockets.size(); ++i)
    {
        fd_max = std::max(fd_max, Sockets[i]);
        // std::cout << "fdmax : "<< fd_max << std::endl;
    }
    std::cout << "Sockets.size =  : "<< Sockets.size() << std::endl;   
    std::cout << "fdmax : "<< fd_max << std::endl;   
    std::cout << "[Server] Set up select fd sets\n";

    while (true) 
    {
        fd_set copy_read_fds = read_fds;
        fd_set copy_write_fds = write_fds;

        int status = select(fd_max + 1, &copy_read_fds, &copy_write_fds, NULL, NULL);
        // std::cout << "test====>: " << status << std::endl;

        if (status == -1) 
        {
            std::cerr << "[Server] Select error: " << strerror(errno) << std::endl;
            return;
        } 
        // else if (status ==  0) 
        // {
        //     std::cout << "[Server] Waiting...\n";
        //     continue;
        // }

        // boucler number of socket 
        for (int i = 3; i <= fd_max; i++) 
        {
            struct stat sb;
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (CheckIsMyServer(i) == 1)
                {
                    std::cout << "i = "<<i<<std::endl;
                    accept_new_connection(i, read_fds, &fd_max);
                } 
                else 
                    read_data_from_socket(i, read_fds, write_fds);
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                std::cout << "\033[32m" << "+++++++++++++++++++++++++++++RESPONSE++++++++++++++++++++++++++++++++++\033[0m" << std::endl;
                for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); it++)
                {
                    if(it->first == i)
                    {
                        std::cout << "------------------------------------\n";
                        std::cout << mapinfo[i].request.url << std::endl;
                        std::cout << "------------------------------------\n";
                        break;
                    }
                }
                std::cout << "************************************\n";
                mapinfo[i].request.response.run();
                std::cout << "************************************\n";
                std::cout << "i == " << i << std::endl;
                std::cout << "hna fach taykhrj " << std::endl;
                mapinfo[i].request.s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se, mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se, 0);
                mapinfo[i].request.se += mapinfo[i].request.s;
                std::cout << "s == " << mapinfo[i].request.s << std::endl;
                std::cout << "se == " << mapinfo[i].request.se << std::endl;
                std::cout << "size == " << mapinfo[i].request.response.SendResponse.size() << std::endl;
                // std::string newsend = mapinfo[i].request.response.SendResponse.c_str() + s;
                if(mapinfo[i].request.se == mapinfo[i].request.response.SendResponse.size())
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    if (stat("./output.txt", &sb) == 0)
                        std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    mapinfo[i].request.s = 0;
                    mapinfo[i].request.se = 0;
                }
                if(mapinfo[i].request.s == -1)
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    if (stat("./output.txt", &sb) == 0)
                        std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    // s = 0;
                    mapinfo[i].request.se = 0;
                    mapinfo[i].request.s = 0;
                }
            }
            
        }
    }
}

