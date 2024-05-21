/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/18 14:34:13 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <thread>
#define PATH "/Users/rarraji/Desktop/prj/WebServMerge/pages"

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
    // RemplirContentType();
    // exit(1);
    
}

// void Server::RemplirContentType()
// {
//     std::ifstream ss("./Content.type");
//     std::string buffer;
//     std::string data;
//     while (getline(ss, buffer))
//     {
//         data += buffer;
//         data += "\n";
//     }
    
//     std::stringstream sss(data);
//     int i = 0;
//     std::string first;
//     std::string second;
//     while (sss >> buffer)
//     { 
//         if (i % 2 == 0)
//             first = buffer;
//         else
//         {
//             second = buffer;
//             ContentType.insert(std::make_pair(first, second));
//         }
//         i++;    
//     }
//     for (std::map<std::string, std::string>::iterator it = ContentType.begin(); it != ContentType.end(); ++it);
//         // std::cout << ContentType[it->first] << std::endl; 
// }


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
        // std::cout << "here\n";
        int server_socket_1 = create_server_socket(convertStringToInt(this->servers[i].get_port()), this->servers[i].get_host(), this->servers[i].get_root(), this->servers[i].get_host());
        Sockets.push_back(server_socket_1);
    }
}




void Server::RemplirInfo(int socket_fd, std::string root, std::string host)
{
    infoserv info;
    // std::cout << "root: " << root << std::endl;
    // std::cout << "host: " << host << std::endl;
    info.root = root;
    info.host = host;
    // info.errorPage = this->servers[socket_fd].get_error_pages()
    // info.MaxBodySize = this->servers[socket_fd].get_max_body_size();
    // set host
    // set port
    // set location
    // set error page
    // set index
    // set maxBodySize
    mapinfo.insert(std::make_pair(socket_fd, info));
}

// add ip addresse;
int Server::create_server_socket(int port, std::string ip, std::string root, std::string host) 
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
    RemplirInfo(socket_fd, root, host);
    // mapinfo.insert(std::make_pair(socket_fd, info));
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Created server socket fd: " << socket_fd << std::endl;
    int on = 1;
    setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR,(const char *)&on, sizeof(int));
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status !=   0) 
    {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Bound socket to port " << "\033[0;34m" << port << "\033[0m" << std::endl;

    status = listen(socket_fd,   10);
    if (status !=   0) 
    {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Listening on port "  << "\033[0;34m" << port << "\033[0m" << std::endl;
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
    std::cout <<"\033[0;31m" " [Server] " << "\033[0m" << "Accepted new connection on client socket " << client_fd << ".\n";
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    mapinfo[socket].request.Check_read(socket, read_fds, write_fds);
}

int Server::CheckIsMyServer(int nb)
{
  for (size_t i = 0; i < Sockets.size(); i++)
  {
    // std::cout << Sockets[i] << std::endl;
    if (nb == Sockets[i])
        return(1);
  }
  return(0);
//   exit(1);
}

void Server::checkResponse(int socket, std::string host , std::string port)
{
    bool check_red = false;
    std::cout << "host: " << host  << std::endl;
    std::cout << "port: " << port  << std::endl;
    std::cout << "Socket: " << socket  << std::endl;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        // check man server 
        if(this->servers[i].get_host().compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
        {
            std::cout << "url : " << mapinfo[socket].request.response.url << std::endl;
            std::cout << "root + url : " <<  this->servers[i].get_root() + mapinfo[socket].request.response.url << std::endl;

            //check is url is location or not
            if(this->servers[i].get_location_first(mapinfo[socket].request.response.url).compare(mapinfo[socket].request.response.url) == 0) 
            {
              check_red = true;
              std::cout << "DONE IS LOCATION" << std::endl;
            }
            else
              std::cout << "ISN'T LOCATION" << std::endl;
            //check url is valide in location;
        }
        // check is dir or file 
        struct stat fileStat;
        std::string tmp1;
        if (mapinfo[socket].request.response.url.find(this->servers[i].get_root()) == std::string::npos) // cheak url isgisila root
            tmp1 = this->servers[i].get_root() + mapinfo[socket].request.response.url;
        else
            tmp1 = mapinfo[socket].request.response.url;
            
        std::cout << "check_red : " << check_red << std::endl;
        
        if (stat(tmp1.c_str(), &fileStat) == 0) // checkdir
        {
            int i = 0;
            if (S_ISDIR(fileStat.st_mode))
            {
                std::cout << "dossier\n";
                std::string tmp  = tmp1;
                std::cout << "tmp : " << tmp  << std::endl;

                //check_if_location_end "/"
                if (tmp[tmp.length() - 1] == '/' && check_red == true)
                    std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
                if (tmp[tmp.length() - 1] != '/')
                {
                    mapinfo[socket].request.response.url += '/';
                    if(check_red == true)
                      std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.response.url).get_redirect_url() << std::endl;
                    mapinfo[socket].request.response.redur = true;
                    mapinfo[socket].request.response.new_redur = mapinfo[socket].request.response.url;
                    std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
                    i = 1;
                    break;
                    
                }
                //check red url 
                else if (i == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url().length() >= 1)
                {
                    std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url() << std::endl;
                    mapinfo[socket].request.response.redur = true;
                    mapinfo[socket].request.response.new_redur = this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url();
                    std::cout << "new_url_to_red : " << mapinfo[socket].request.response.new_redur << std::endl;
                    i = 1;
                    break;
                    //check_redirect_URL
                }
                else
                    mapinfo[i].request.response.redur = false;
                // for (std::map<std::string, bool>::iterator it = this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.begin(); it != this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.end(); ++it)
                // {
                //     //check methodes
                // }
                if (i == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_index().length() > 1)
                {
                //  mapinfo[socket].request.response.root = this->servers[i].get_root() + mapinfo[socket].request.url;
                    mapinfo[socket].request.response.url = this->servers[i].get_root() + this->servers[i].get_location_first(mapinfo[socket].request.url) + this->servers[i].get_location(mapinfo[socket].request.url).get_index();
                    mapinfo[socket].request.response.redur = true;
                    mapinfo[socket].request.response.new_redur = mapinfo[socket].request.response.url;
                    // this->servers[i].get_location(mapinfo[socket].request.url)
                    std::cout << "index To serve : " << mapinfo[socket].request.response.url << std::endl;
                    i = 1;
                    break;
                }
                std::cout << "directory_listing :" << this->servers[i].get_location(mapinfo[socket].request.url).get_directory_listing() << std::endl;
                if (i == 0 && this->servers[i].get_location(mapinfo[socket].request.url).get_directory_listing().compare("true") == 0)
                {
                  std::cout << "DONE directory_listing" << std::endl;  
                  mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.response.url;
                  mapinfo[socket].request.response.directory_listing = true;
                  break;
                }
                break;
            }
            else
            {
                mapinfo[socket].request.response.url = this->servers[i].get_root() + mapinfo[socket].request.url;
                std::cout << "FILE : " << mapinfo[socket].request.response.url << std::endl;
                std::cout << "File Done" << std::endl;
                break;    
            }
        }
        
    }
    // exit(1);
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
        // std::cout << Sockets[i] << std::endl;
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
    // std::cout << "Sockets.size =  : "<< Sockets.size() << std::endl;   
    // std::cout << "fdmax : "<< fd_max << std::endl;   
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Set up select fd sets\n";

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
            // struct stat sb;
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (CheckIsMyServer(i) == 1)
                {
                    // std::cout << "i = "<<i<<std::endl;
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
                        // std::cout << "------------------------------------\n";
                        // std::cout << mapinfo[i].request.url << std::endl;
                        // std::cout << "------------------------------------\n";
                        break;
                    }
                }
                std::cout << "************************************\n";
                checkResponse(i, mapinfo[i].request.host, mapinfo[i].request.port);
                // exit(1);
                mapinfo[i].request.response.run();
                // std::cout << "************************************\n";
                // std::cout << "i == " << i << std::endl;
                // std::cout << "hna fach taykhrj " << std::endl;
                mapinfo[i].request.s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se, mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se, 0);
                mapinfo[i].request.se += mapinfo[i].request.s;
                // std::cout << "s == " << mapinfo[i].request.s << std::endl;
                // std::cout << "se == " << mapinfo[i].request.se << std::endl;
                // std::cout << "size == " << mapinfo[i].request.response.SendResponse.size() << std::endl;
                // std::string newsend = mapinfo[i].request.response.SendResponse.c_str() + s;
                if(mapinfo[i].request.se == mapinfo[i].request.response.SendResponse.size())
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    // if (stat("./output.txt", &sb) == 0)
                    //     std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    mapinfo[i].request.s = 0;
                    mapinfo[i].request.se = 0;
                }
                if(mapinfo[i].request.s == -1)
                {
                    // s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + s, mapinfo[i].request.response.SendResponse.size(), 0);
                    FD_CLR(i,&write_fds);
                    close(i);
                    // if (stat("./output.txt", &sb) == 0)
                    //     std::remove("./output.txt");
                    std::cout << "\033[0;35m" << "---------->>>>>CLOSE-SOCKET<<<<<-------- : " << i << "\033[0m" << std::endl;
                    // s = 0;
                    mapinfo[i].request.se = 0;
                    mapinfo[i].request.s = 0;
                }
            }
            
        }
    }
}

