/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:38 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/09 19:26:16 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include<dirent.h>


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
  std::cout << "here\n";
  return(this->header);
}
std::string Response::GetBody()
{
  return(this->body);
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



void Response::run()
{
//   std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
//   std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
  
                std::string test = "/";
                std::string test1 = "/favicon.ico";
                std::string path = "./pages";
                std::string new_path = "./pages";
                std::cout << "----> " << check_cgi << std::endl;
                std::cout << "---->" << url << "<----"<< std::endl;
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
                        std::cout << "jiji"<< url << std::endl;
                        check = false;
                        url = "/Users/rarraji/Desktop/lastWeb/Server/pages/images/rarraji.jpg";
                    }
                    if (url.compare("/") == 0)
                    {
                        std::cout << "jiji"<< url << std::endl;
                        check = false;
                        url = "/Users/rarraji/Desktop/lastWeb/Server/pages/index.html";
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
                std::cout << "url : "<< url << std::endl;
                std::stringstream buffer;
                SendResponse = "HTTP/1.1 200 OK\r\n";
                std::string new_url;
                if(url.find("/Users/rarraji/Desktop/lastWeb/Server/pages") == std::string::npos)
                    new_url = "/Users/rarraji/Desktop/lastWeb/Server/pages" + url;
                else
                    new_url = url;    
                if (url.compare("./ErrorPages") == 0)
                {
                    std::cout << "hiii" << std::endl;
                    SendResponse += "Content-Type: text/html\r\n";      
                    SendResponse += "\r\n";
                    SendResponse += generateHTML(url.c_str());
                }
                else if (url.find(".jpg") != std::string::npos)
                {
                    std::cout << "->new_url : "<< new_url << std::endl;
                    std::ifstream file(new_url.c_str(), std::ios::binary);
                    if (!file.is_open()) 
                    {
                        std::cerr << "[Server] Impossible d'ouvrir image " << "\n";
                    }
                    std::cout << "here" << std::endl;
                    buffer << file.rdbuf();
                    file.close();
                    SendResponse += "Content-Type: image/jpg\r\n";
                }
                else if (url.compare("/images/vedeo.mp4") == 0)
                {
                    std::cout << "new_url : "<< new_url << std::endl;
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
                    SendResponse += "Content-Type: video/mp4\r\n";
                    // exit(1);
                }
                else
                {
                    std::cout << "new_url : "<< new_url << std::endl;
                    std::ifstream file(new_url.c_str());
                    // int check = 1;
                    if (!file.is_open()) 
                    {
                        file.close();
                        std::ifstream file("/Users/rarraji/Desktop/lastWeb/Server/pages/ErrorPages/notFound.html");
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
                    SendResponse += "Content-Type: text/html\r\n";                    
                }
                if (url.compare("/ErrorPages") != 0)
                {
                    // std::cout << "[Server]" << "\n";
                    SendResponse += "\r\n";
                    SendResponse += buffer.str();
                    // std::cout << "---------------------------------"  << std::endl;
                    // std::cout << "SendResponse : " << buffer.str() << std::endl;
                    // std::cout << "---------------------------------"  << std::endl;
                }
                check_cgi = false;
                check = true;
}