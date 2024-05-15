/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:38 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/11 10:02:45 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include<dirent.h>

#define PATH "/Users/rarraji/Desktop/prj/WebServMerge/pages/"

Response::Response()
{
//   check_cgi = false;
}
Response::~Response()
{
  
}
void Response::SetHeader(std::string header_tmp)
{
  this->header = header_tmp;
}
void Response::SetBody(std::string body_tmp)
{
  this->body = body_tmp;
}
void Response::SetUrl(std::string url_tmp)
{
  this->url = url_tmp;
}
void Response::SetSocket(int tmp_SetSocket)
{
  this->url = tmp_SetSocket;
}

std::string Response::GetHeader()
{
//   std::cout << "here\n";
  return(this->header);
}
std::string Response::GetBody()
{
  return(this->body);
}


void Response::RemplirContentType()
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
    for (std::map<std::string, std::string>::iterator it = ContentType.begin(); it != ContentType.end(); ++it);
        // std::cout << ContentType[it->first] << std::endl; 
}

std::string Response::generateHTML(const char* path) 
{
                // SendResponse = "HTTP/1.1 200 OK\r\n";
    std::ostringstream ss;
    DIR *dir = opendir(path);
    if (!dir) {
        return "";
    }

    ss << "<html><head><title>Directory Listing</title><style>h1 {text-align:center;}</style></head><body><h1>Directory Listing</h1><ul><br>";

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] != '.') {
            ss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
        }
    }

    ss << "</ul></body></html>";
    closedir(dir);
    return ss.str();
}


std::string Response::AddContentType()
{
    std::string part = "Content-Type: ";
    std::string check;
    size_t pos = url.find('.');
    if (pos != std::string::npos)
    {
        check = url.substr(pos + 1 , url.length());
        for (std::map<std::string, std::string>::iterator it = ContentType.begin(); it != ContentType.end(); ++it)
        {
            if(check.compare(it->first) == 0)
            {
                part += it->second;
                part += "\r\n";
                break;
            }
        }
        
    }
    if(part.compare("Content-Type: ") == 0)
    {
        part += "text/html";
        part += "\r\n";   
    }
        
    std::cout << "HNA : ---> "<< part << std::endl;
    // exit(1);
    return(part);   
    // for (size_t i = 0; i < ; i++)
    // {
    //     /* code */
    // }
}



void Response::run()
{
    RemplirContentType();
  std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
  std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
  
                std::string test = "/";
                std::string test1 = "/favicon.ico";
                std::string path = "./pages";
                std::string new_path = "./pages";
                // std::cout << "----> " << check_cgi << std::endl;
                // std::cout << "---->" << url << "<----"<< std::endl;
                int check = true;

                if (check_cgi == false)
                {
                    // if (url.compare("/ErrorPages") == 0)
                    // {
                    //     std::cout << "i am here 1" << std::endl;
                    //     url = "./ErrorPages";
                    // }
                    // if (url.compare("/home") == 0)
                    //     url = "/home";
                    if (url.compare("/favicon.ico") == 0)
                    {
                        // std::cout << "jiji"<< url << std::endl;
                        check = false;
                        url = PATH;
                        url += "images/solix.jpg";
                    }
                    if (url.compare("/") == 0)
                    {
                        // std::cout << "jiji"<< url << std::endl;
                        check = false;
                        url = PATH;
                        url += "index.html";
                    }
                    // if (url.compare("/upload") == 0)
                    //     url = "/upload";
                    // if (url.compare("/images/rarraji.jpg") == 0)
                    //     url = "./images/rarraji.jpg";
                    // if (url.compare("/images/bel-kdio.jpg") == 0)
                    //     url = "./images/bel-kdio.jpg";
                    // if (url.compare("/images/maxresdefault.jpg") == 0)
                    //     url = "./images/maxresdefault.jpg";
                    // if (url.compare("/images/vedeo.mp4") == 0)
                    //     url = "./images/vedeo.mp4"; 
                        
                    // if(url.compare("./ErrorPages") == 0)
                    // {
                    //     std::cout << "i am here\n";
                    //     new_path = url;        
                    // }
                    // else if (url.compare("./images/rarraji.jpg") != 0 && url.compare("./images/bel-kdio.jpg") != 0 && url.compare("./images/maxresdefault.jpg") && url.compare("./images/vedeo.mp4") && url.compare("../fichier.txt") != 0)
                    // {
                    //     new_path += url + ".html";
                    //     std::cout << new_path << std::endl;
                    //     std::cout << "HERE2\n";
                    // }   
                    // else
                    // {
                    //     new_path = url;
                    //     // std::cout << "HERE1\n";
                    // }
                }
                else
                    url = "./output.txt";
                // std::cout << "url : "<< url << std::endl;
                std::stringstream buffer;
                SendResponse = "HTTP/1.1 200 OK\r\n";
                std::string new_url;
                bool dir = false;
                if(url.find(PATH) == std::string::npos)
                {
                    new_url = PATH; 
                    new_url += url;
                    
                }
                else
                    new_url = url;    
                if (url.compare("/ErrorPages") == 0)
                {
                    // std::cout << "hiii" << std::endl;
                    SendResponse += "Content-Type: text/html\r\n";      
                    SendResponse += "\r\n";
                    std::string tmp;
                    tmp = PATH;
                    tmp += "ErrorPages";
                    SendResponse += generateHTML(tmp.c_str());
                    dir = true;
                }
                else if (url.find(".jpg") != std::string::npos)
                {
                    std::cout << "->new_url : "<< new_url << std::endl;
                    std::ifstream file(new_url.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir image " << "\n";
                    }
                    // std::cout << "here" << std::endl;
                    buffer << file.rdbuf();
                    file.close();
                    SendResponse += AddContentType();
                    // SendResponse += "Content-Type: image/jpg\r\n";
                }
                else if (url.compare("/images/vedeo.mp4") == 0)
                {
                    // std::cout << "new_url : "<< new_url << std::endl;
                    std::ifstream file(new_url.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                    }
                    buffer << file.rdbuf();
                    file.close();
                    // std::stringstream fi;
                    std::ofstream fi("vedeo.mp4", std::ios::binary);
                    if (!fi.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir le vedeo " << "\n";
                    }
                    fi << buffer.rdbuf();
                    SendResponse += AddContentType();
                    // SendResponse += "Content-Type: video/mp4\r\n";
                }
                else
                {
                    if(check_cgi == true)
                       new_url = url;
                    std::ifstream file(new_url.c_str());
                    // int check = 1;
                    if (!file.is_open()) 
                    {
                        file.close();
                        std::string tmp;
                        tmp = PATH;
                        tmp += "ErrorPages/notFound.html";
                        std::ifstream file(tmp);
                        // check = 0;
                        if (!file.is_open())
                        {
                         std::cerr << "[Server 404] Impossible d'ouvrir le fichier here" << "\n";
                        }
                        std::cerr << "[Server] Impossible d'ouvrir le fichier here" << "\n";
                        buffer << file.rdbuf();
                        file.close();
                    }
                    buffer << file.rdbuf();
                    file.close();
                    SendResponse += AddContentType();
                    // SendResponse += "Content-Type: text/html\r\n";                    
                }
                // if (dir == false)
                //    SendResponse = AddContentType();
                if (url.compare("/ErrorPages") != 0)
                {
                    SendResponse += "\r\n";
                    SendResponse += buffer.str();
                }
                    // std::cout << "---------------------------------"  << std::endl;
                    // std::cout << "SendResponse : " << buffer.str() << std::endl;
                    // std::cout << "---------------------------------"  << std::endl;
                check_cgi = false;
                check = true;
}