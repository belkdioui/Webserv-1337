/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:36 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/11 09:44:14 by rarraji          ###   ########.fr       */
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
#include <thread>
#include <sys/stat.h>

// class request;

class Response
{
  public:
    bool check_body;
    bool check_cgi;
    int socket;
    std::string body;
    std::string header;
    std::string code;
    std::string methode;
    std::string url;
    std::string SendResponse;
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
    void RemplirContentType();

};