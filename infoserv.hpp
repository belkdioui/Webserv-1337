/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infoserv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:16:42 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/04 09:59:08 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "map"
#include "request.hpp"
#include "./config_file/location_param.hpp"
#include "./config_file/partition_server.hpp"
#include "./config_file/utils.hpp"

class Request;
// class Response;

class infoserv
{
  public:
    std::vector<partition_server> servers;
    std::string body;
    int host;
    std::string index;
    std::string alias;
    std::string root;
    std::vector<std::string> server_name;
    std::map<std::string, std::string> error_pages;
    std::map<std::string, location_param> location;
    std::string port;
    std::string MaxBodySize;
    Request request;
    void SetContentType();
    location_param get_location(std::string key);
    void set_location(std::string index, location_param value);
    std::string get_location_first(std::string index);
    std::string get_error_pages(std::string key);
    
};