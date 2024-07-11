/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:24 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/03 18:25:46 by rarraji          ###   ########.fr       */
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
#include "cgi.hpp"
#include "response.hpp"
#include "./config_file/location_param.hpp"

class Request{
  private:
    std::string request;
    std::string l3mara;
    std::string new_3mara;
    std::map<std::string, std::string> error_pages;
    char buffer[1000000];
    int body_lenght;
    bool chunked;
    bool Get;
    bool check;
    bool last;
    bool ContentLength;
    int compareLenBody;
    int header_len;
    int start;
    int check_l3mara;
    int upload_nor;
    
    // bool cgi;
  public:
  
    std::string header;
    std::string reqerror;
    std::string body;
    std::string new_body;
    std::string url;
    std::string methode;
    std::string host;
    std::string port;
    std::string root;
    std::string querystingcgi;
    std::string MaxBodySize;
    std::map<std::string, location_param> location;
    bool check_cgi;
    bool cgi;
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
    int check_req_valid();
    void checkQuery();
    void UploadFiles();
    void SaveHost_Port(std::string tmp_host);
    void CreatFiles(std::string NameFile, std::string buf, bool check);
    std::string generateErrorPage(int statusCode, const std::string& errorMessage);
    location_param get_location(std::string key);
    std::string get_error_pages(std::string key);
    void set_location(std::string index, location_param value);
    Response response;
};

#endif