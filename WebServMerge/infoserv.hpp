/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infoserv.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:16:42 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/12 14:57:47 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "map"
#include "request.hpp"
#include "location.hpp"

class Request;
// class Response;

class infoserv
{
  public:
    std::string body;
    std::map<int, std::string> ErrorPage;
    std::string host;
    std::string index;
    std::string root;
    int port;
    std::string MaxBodySize;
    location location;
    Request request;
    void SetContentType();
};