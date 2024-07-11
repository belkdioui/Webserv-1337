/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infoserv.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:16:45 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/03 12:53:06 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "infoserv.hpp"

void infoserv::set_location(std::string index, location_param value)
{
    location[index] = value;
}

location_param infoserv::get_location(std::string key)
{
    std::map<std::string, location_param>::iterator it = location.find(key);
     if (it == location.end())
     {
        std::string path;
        std::string save;
        int i = 0;
        for(it = location.begin(); it != location.end(); it++)
        {
            if(!key.find(it->first))
            {
                if(i == 0)
                    save = it->first;
                else
                {
                    if(save.size() <= it->first.size())
                        save = it->first;
                }
            }
            i++;
        }
        it = location.find(save);
        if (save.empty())
        {
            location_param loc;
            loc.set_methods("GET");
            loc.set_index(utils::big_index);
            set_location("/", loc);
            return loc;
        }
        return it->second;
     }
    return it->second;
}

std::string infoserv::get_error_pages(std::string key)
{
    std::map<std::string, std::string>::iterator it = error_pages.find(key);
    if (it == error_pages.end())
        return "";
    return it->second;
}

std::string infoserv::get_location_first(std::string index)
{
    location_param tmp = get_location(index);
    std::map<std::string, location_param>::iterator it = location.begin();
    std::map<std::string, location_param>::iterator ite = location.end();
    for (; it != ite; it++)
    {
        if((*it).second == tmp)
        {
            return (it->first);
        }
    }
    return "";
}