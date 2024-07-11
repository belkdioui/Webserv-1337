/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 15:53:48 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/04 10:23:09 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() 
{
    
}

void Server::RemplirContentType()
{
    std::ifstream ss("./Content.type");
    std::string buffer;
    std::string data;
    while (getline(ss, buffer))
    {
        data += buffer;
        data += "\n";
    }
    
    std::stringstream sss(data);
    int i = 0;
    std::string first;
    std::string second;
    while (sss >> buffer)
    { 
        if (i % 2 == 0)
            first = buffer;
        else
        {
            second = buffer;
            ContentType.insert(std::make_pair(first, second));
        }
        i++;    
    }
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
    RemplirContentType();
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        int server_socket_1 = create_server_socket1(i);
        if(server_socket_1 != -2)
            Sockets.push_back(server_socket_1);
    }
}

void Server::RemplirInfo1(int socket_fd, int sock)
{
    infoserv info;
    info.servers = this->servers;
    info.port = this->servers[sock].get_port();
    info.root = this->servers[sock].get_root();
    info.host = this->servers[sock].get_host();
    info.error_pages = this->servers[sock].error_pages;
    info.location = this->servers[sock].location;
    info.MaxBodySize = this->servers[sock].get_max_body_size();
    info.index = this->servers[sock].get_index();
    info.server_name = this->servers[sock].server_name;
    mapinfo.insert(std::make_pair(socket_fd, info));
}

int Server::create_server_socket1(int sock) 
{
    struct sockaddr_in sa;
    int socket_fd;
    int status;

    std::memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(this->servers[sock].get_host());

    for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); ++it)
    {
        if(it->second.host == servers[sock].get_host() && it->second.port == servers[sock].get_port())
            return(-2);
    }
    

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1)
    {
        std::cerr << "[Server] Socket error: " << strerror(errno) << std::endl;
        return (-1);
    }
    RemplirInfo1(socket_fd, sock);
    sa.sin_port = htons(convertStringToInt(this->servers[sock].get_port()));

    int on = 1;
    setsockopt(socket_fd,SOL_SOCKET, SO_REUSEADDR,(const char *)&on, sizeof(int));
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof sa);
    if (status !=   0) 
    {
        std::cerr << "[Server] Bind error: " << strerror(errno) << std::endl;
    }

    status = listen(socket_fd, 100000);
    if (status !=   0) 
    {
        std::cerr << "[Server] Listen error: " << strerror(errno) << std::endl;
        return (-1);
    }
    std::cout << "\033[0;31m" " [Server] " << "\033[0m" << "Listening on port "  << "\033[0;34m" << this->servers[sock].get_port() << "\033[0m" << std::endl;
    return (socket_fd);
}

void Server::accept_new_connection(int listener_socket, fd_set &read_fds, int *fd_max)  
{
    int client_fd;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);


    
    client_fd = accept(listener_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    std::map <int, infoserv> ::iterator it_bg = mapinfo.find(listener_socket);
    mapinfo[client_fd] = it_bg->second;
    if (client_fd == -1) 
    {
        std::cerr << "[Server] Accept error: " << strerror(errno) << std::endl;
        return;
    }
    FD_SET(client_fd, &read_fds);
    if (client_fd > *fd_max) 
        *fd_max = client_fd;
}

void Server::read_data_from_socket(int socket, fd_set &read_fds, fd_set &write_fds) 
{
    mapinfo[socket].request.root =  mapinfo[socket].root;
    mapinfo[socket].request.response.ContentType =  ContentType;
    mapinfo[socket].request.location = mapinfo[socket].location;
    mapinfo[socket].request.MaxBodySize =  mapinfo[socket].MaxBodySize;
    mapinfo[socket].request.Check_read(socket, read_fds, write_fds);
}

int Server::CheckIsMyServer(int nb)
{
  for (size_t i = 0; i < Sockets.size(); i++)
  {
    if (nb == Sockets[i])
        return(1); 
  }
  return(0);
}


int Server::CheckServerNmae(int i, std::string servername)
{
    for (size_t j = 0; j < this->servers[i].get_server_name().size(); j++)
    {
        if(this->servers[i].get_server_name()[j].compare(servername) == 0)
            return(1);
    }
    return(0);
}


std::string generateErrorPage2(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}

std::string convertIntToString2(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}




void Server::checkResponse1(int sock, std::string host)
{
    (void) host;

    if(mapinfo[sock].request.response.errorpage == 1)
        return;
    for (size_t i = 0; i < this->servers.size(); i++)
    {
        if(this->servers[i].get_host() == mapinfo[sock].host && this->servers[i].get_port() == mapinfo[sock].port)
        {
            std::vector<std::string> server_names = this->servers[i].get_server_name();
            
            for(std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); it++)
            {
                if((*it) == host)
                {
                    mapinfo[sock].error_pages = this->servers[i].error_pages;
                    mapinfo[sock].location = this->servers[i].location;
                    mapinfo[sock].MaxBodySize = this->servers[i].get_max_body_size();
                    mapinfo[sock].index = this->servers[i].get_index();
                    mapinfo[sock].server_name = this->servers[i].server_name;
                    mapinfo[sock].root = this->servers[i].get_root();
                }
            }
        }
    }
    mapinfo[sock].request.response.errorpage = 0;

                struct stat fileStat;
                std::string tmp1;
                mapinfo[sock].request.response.redur = false;
                if (mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)
                {
                    if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() > 1)
                        tmp1 = mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias();
                    else    
                        tmp1 = this->mapinfo[sock].root;
                    tmp1.append(mapinfo[sock].request.response.url);
                }
                else
                    tmp1 = mapinfo[sock].request.response.url;
                if (mapinfo[sock].request.response.url.compare("/favicon.ico") == 0)
                {
                    mapinfo[sock].request.response.url = "/Users/rarraji/Desktop/prj/WS_BETTA/pages/images/me.png";
                    return;
                }
                
                if (mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url().length() >= 1)
                {
                    mapinfo[sock].request.response.redur = true;
                    mapinfo[sock].request.response.new_redur = mapinfo[sock].get_location(mapinfo[sock].request.url).get_redirect_url();
                    return;
                }
                // std::cout << "len: " << mapinfo[sock].get_location_first(mapinfo[sock].request.url) << std::endl;
                if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() > 1)
                {
                    size_t len = mapinfo[sock].get_location_first(mapinfo[sock].request.url).size();
                    // size_t pos = mapinfo[sock].request.url.find(mapinfo[sock].get_location_first(mapinfo[sock].request.url));
                    mapinfo[sock].request.response.url = mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias() + mapinfo[sock].request.url.substr(len);
                    // std::cout << "alias " << mapinfo[sock].request.response.url << std::endl;
                    tmp1 = mapinfo[sock].request.response.url;

                    // mapinfo[sock].request.url = mapinfo[sock].request.response.url;
                }
                if (stat(tmp1.c_str(), &fileStat) == 0) 
                {
                    int j = 0;
                    if (S_ISDIR(fileStat.st_mode))
                    {
                        std::string tmp  = tmp1;

                        if(tmp1.find(".py") != std::string::npos)
                            j = 1;
                        if(mapinfo[sock].request.methode != "DELETE")
                        {
                            if(mapinfo[sock].request.url[mapinfo[sock].request.url.length() - 1] != '/')
                            {
                                mapinfo[sock].request.url += '/';
                                mapinfo[sock].request.response.url = mapinfo[sock].request.url;
                                mapinfo[sock].request.response.redur = true;
                                mapinfo[sock].request.response.new_redur = mapinfo[sock].request.response.url;
                                return;
                            }

                            // if(this->mapinfo[sock].get_location_first(mapinfo[sock].request.url ).compare((mapinfo[sock].request.url)) == 0) 
                            // {
                            //     // check_red = true;
                            //     std::cout << "DONE IS LOCATION" << std::endl;
                            // }
                            // else
                            // {
                            //     std::cout << "ISN'T LOCATION" << std::endl;
                            //     j = 1;
                            // }
                            // std::cout << "alias lenght : " << mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() << std::endl;   
                            if (j == 0 && mapinfo[sock].get_location(mapinfo[sock].request.url).get_index().length() > 1)
                            {
                                // std::cout << "d5alna l check dyal index: " << mapinfo[sock].get_location(mapinfo[sock].request.url).get_index() << std::endl;   
                                if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() < 1)
                                {

                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.url ;
                                }
                                    // mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location_first(mapinfo[sock].request.url);
                                    mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location(mapinfo[sock].request.url).get_index();
                                
                                // if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() < 1)
                                // {
                                //     mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.url ;
                                //     // mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location_first(mapinfo[sock].request.url);
                                //     mapinfo[sock].request.response.url = mapinfo[sock].request.response.url + mapinfo[sock].get_location(mapinfo[sock].request.url).get_index();
                                // }
                                if(mapinfo[sock].request.response.url.find(".py") != std::string::npos)
                                {
                                    mapinfo[sock].request.response.check_cgi = true;
                                    Cgi cgi;
                                    cgi.SetHeader(mapinfo[sock].request.header);
                                    cgi.SetBody(mapinfo[sock].request.new_body);
                                    cgi.url = mapinfo[sock].request.response.url;
                                    cgi.root = mapinfo[sock].request.root;
                                    cgi.querystingcgi = mapinfo[sock].request.querystingcgi;
                                    cgi.methode = mapinfo[sock].request.methode;
                                    cgi.run();
                                    if (cgi.time_out == 1)
                                    {
                                        mapinfo[sock].request.response.check_cgi = false;
                                        mapinfo[sock].request.response.status = 500;
                                        if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                        {
                                            mapinfo[sock].request.response.errorpage = 1;
                                            mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                        }
                                        else
                                            mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    }
                                    if (cgi.status_code_error == 1)
                                    {
                                    mapinfo[sock].request.response.check_cgi = false;
                                    mapinfo[sock].request.response.status = 504;
                                        if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                        {
                                            mapinfo[sock].request.response.errorpage = 1;
                                            mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                        }
                                        else
                                            mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    }
                                    else
                                        mapinfo[sock].request.response.url = mapinfo[sock].root + "/output.txt";
                                }
                                // std::cout << "access: " << mapinfo[sock].request.response.url.c_str() <<std::endl;
                                if(access(mapinfo[sock].request.response.url.c_str(), F_OK) == 0)
                                {
                                    j = 1;
                                    return;
                                }
                            }
                            // std::cout << "j : " << j << std::endl;
                            if (j == 0 && mapinfo[sock].get_location(mapinfo[sock].request.url).get_directory_listing() == "true")
                            {
                                // if(mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)
                                // {
                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.url;
                                    mapinfo[sock].request.response.directory_listing = true;
                                // }
                                return;
                            }
                            else
                            {
                                mapinfo[sock].request.response.status = 403;
                                if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                {
                                    mapinfo[sock].request.response.errorpage = 1;
                                    mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "FORBIDEN");
                                }
                                else
                                {
                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                                    if(stat(mapinfo[sock].request.response.url.c_str(), &fileStat) != 0)
                                    {
                                        mapinfo[sock].request.response.errorpage = 1;
                                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "FORBIDEN");
                                    }

                                }
                                
                                return;
                            }
                            
                            return;
                        }
                        else
                        {
                            if (mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos)
                                mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].request.response.url;
                            
                            return;    
                        }

                    }
                    // if(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() > 1)
                    // {
                    //     std::cout << "alias" << std::endl;
                    //     if(mapinfo[sock].request.response.url.find(mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias()) == std::string::npos)
                    //         mapinfo[sock].request.response.url = mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias() +  mapinfo[sock].request.response.url;
                    //         return;
                    // }
                    else
                    {
                        if(mapinfo[sock].request.response.url.find(mapinfo[sock].root) == std::string::npos && mapinfo[sock].get_location(mapinfo[sock].request.url).get_alias().length() < 1)
                            mapinfo[sock].request.response.url = mapinfo[sock].root +  mapinfo[sock].request.response.url;
                        // std::cout << "file :" << mapinfo[sock].request.response.url << std::endl;
                        
                        
                        if(mapinfo[sock].request.response.url.find(".py") != std::string::npos)
                        {
                            mapinfo[sock].request.response.check_cgi = true;
                            Cgi cgi;
                            cgi.SetHeader(mapinfo[sock].request.header);
                            cgi.SetBody(mapinfo[sock].request.new_body);
                            cgi.url = mapinfo[sock].request.response.url;
                            cgi.root = mapinfo[sock].request.root;
                            cgi.querystingcgi = mapinfo[sock].request.querystingcgi;
                            cgi.methode = mapinfo[sock].request.methode;
                            cgi.run();
                            if (cgi.time_out == 1)
                            {
                                mapinfo[sock].request.response.check_cgi = false;
                                mapinfo[sock].request.response.status = 500;
                                if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                {
                                    mapinfo[sock].request.response.errorpage = 1;
                                    mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                }
                                else
                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                            }
                            if (cgi.status_code_error == 1)
                            {
                            mapinfo[sock].request.response.check_cgi = false;
                            mapinfo[sock].request.response.status = 504;
                                if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                                {
                                    mapinfo[sock].request.response.errorpage = 1;
                                    mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "notImplemented");
                                }
                                else
                                    mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                            }
                            else
                                mapinfo[sock].request.response.url = mapinfo[sock].root + "/output.txt";
                        }
                        
                        return;
                    }
                }
                   
                else if (stat(tmp1.c_str(), &fileStat) != 0 && mapinfo[sock].request.methode != "DELETE")
                {
                    mapinfo[sock].request.response.status = 404;
                    if(mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status)).empty())
                    {
                        mapinfo[sock].request.response.errorpage = 1;
                        mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "NOT-FOUND");
                    }
                    else
                    {
                        mapinfo[sock].request.response.url = mapinfo[sock].root + mapinfo[sock].get_error_pages(convertIntToString2(mapinfo[sock].request.response.status));
                        if(stat(mapinfo[sock].request.response.url.c_str(), &fileStat) != 0)
                        {
                            mapinfo[sock].request.response.errorpage = 1;
                            mapinfo[sock].request.response.url = generateErrorPage2(mapinfo[sock].request.response.status, "NOT-FOUND");
                        }
                        
                        
                    }
                    return;
                }
}



void Server::run() 
{
    fd_set read_fds;
    fd_set write_fds;
    int fd_max =  0;
    

    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    for (size_t i = 0; i < Sockets.size(); i++)
    {
        FD_SET(Sockets[i], &read_fds);
    }

    for (size_t i = 0; i < Sockets.size(); ++i)
    {
        fd_max = std::max(fd_max, Sockets[i]);
    }


    while (true) 
    {
        fd_set copy_read_fds = read_fds;
        fd_set copy_write_fds = write_fds;

        int status = select(fd_max + 1, &copy_read_fds, &copy_write_fds, NULL, NULL);

        if (status == -1) 
        {
            std::cerr << "[Server] Select error: " << strerror(errno) << std::endl;
            return;
        } 
        for (int i = 3; i <= fd_max; i++) 
        {
            struct stat sb;
            if (FD_ISSET(i, &copy_read_fds)) 
            {
                if (CheckIsMyServer(i) == 1)
                {
                    accept_new_connection(i, read_fds, &fd_max);
                } 
                else 
                    read_data_from_socket(i, read_fds, write_fds);
            }
            if (FD_ISSET(i, &copy_write_fds)) 
            {
                for (std::map <int, infoserv>::iterator it = mapinfo.begin() ; it != mapinfo.end(); it++)
                {
                    if(it->first == i)
                    {
                        break;
                    }
                }
                    
                    checkResponse1(i, mapinfo[i].request.host);
                    mapinfo[i].request.response.run();

                    mapinfo[i].request.s = send(i,  mapinfo[i].request.response.SendResponse.c_str() + mapinfo[i].request.se, mapinfo[i].request.response.SendResponse.size() - mapinfo[i].request.se, 0);

                    mapinfo[i].request.se += mapinfo[i].request.s;

                    if (mapinfo[i].request.se == mapinfo[i].request.response.SendResponse.size())
                    {
                        FD_CLR(i,&write_fds);
                        mapinfo[i].request.s = 0;
                        mapinfo[i].request.se = 0;
                        if (stat("./output.txt", &sb) == 0 && stat("./input.txt", &sb) == 0)
                        {
                            std::remove("./output.txt");
                            std::remove("./input.txt");
                        }
                        mapinfo[i].request.response.SendResponse = "";
                        mapinfo[i].request.header = "";
                        mapinfo[i].request.body = "";
                        close(i);

                    }
                    if (mapinfo[i].request.s == -1)
                    {
                        FD_CLR(i,&write_fds);
                        mapinfo[i].request.s = 0;
                        mapinfo[i].request.se = 0;
                        mapinfo[i].request.response.SendResponse = "";
                        mapinfo[i].request.header = "";
                        mapinfo[i].request.body = "";
                        close(i);
                        if (stat("./output.txt", &sb) == 0 && stat("./input.txt", &sb) == 0)
                        {
                            std::remove("./output.txt");
                            std::remove("./input.txt");
                        }
                    }
            }
            
        }
    }
}


