/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:36 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/04 08:10:11 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
#include <sys/stat.h>

// class request;

class Response
{
  public:
    
    bool check_body;
    std::map<std::string, std::string> error_pages;
    bool check_cgi;
    bool redur;
    int socket;
    int status;
    int errorpage;
    std::string body;
    std::string root;
    std::string header;
    std::string code;
    std::string methode;
    std::string url;
    std::string SendResponse;
    std::string new_redur;
    std::string reqerror;
    bool directory_listing;
    std::map <std::string , std::string> ContentType;
    Response();
    ~Response();
    std::string generateHTML(const char* path);
    void SetHeader(std::string header);
    void SetBody(std::string body);
    void SetUrl(std::string url_tmp);
    void SetSocket(int tmp_socket);
    std::string GetBody();
    std::string GetHeader();
    void run();
    std::string AddContentType();
    // void RemplirContentType();
    void checkResponse(int socket_fd, std::string host, std::string port);
    std::string get_error_pages(std::string key);

};