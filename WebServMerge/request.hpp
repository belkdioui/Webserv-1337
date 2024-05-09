/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:24 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/08 10:34:41 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

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
#include <vector> 
#include <map>   
#include <fstream> 
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <thread>
#include "cgi.hpp"
#include "response.hpp"


class Request{
  private:
    std::string request;
    char buffer[2048];
    int body_lenght;
    bool chunked;
    bool Get;
    bool check;
    bool last;
    bool ContentLength;
    int compareLenBody;
    int header_len;
    int start;
    
    // bool cgi;
  public:
    std::string header;
    std::string body;
    std::string new_body;
    std::string url;
    std::string methode;
    std::string host;
    bool check_cgi;
    long long s;
    size_t se;
    Request();
    int read_socket(int socket);
    void Check_read(int socket, fd_set &read_fds, fd_set &write_fds);
    void remplirMyMap(int socket);
    void addSocketModeWrite();
    void CheckMethodeReq(int socket);
    void calBodylenght();
    void AddHeaderReq(int valread);
    void AddHeaderBody();
    void CheckChunked();
    void RegContent(int nb);
    void check_req_valid();
    void checkQuery();
    void UploadFiles();
    void CreatFiles(std::string NameFile, std::string buf, bool check);
    Response response;
};

#endif