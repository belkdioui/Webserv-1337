/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 10:17:38 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/17 10:42:09 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "response.hpp"
#include<dirent.h>

#define PATH "/Users/rarraji/Desktop/prj/WebServMerge/pages"

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
        
    // std::cout << "HNA : ---> "<< part << std::endl;
    // exit(1);
    return(part);   
    // for (size_t i = 0; i < ; i++)
    // {
    //     /* code */
    // }
}







// void Response::checkResponse(int socket, std::string host , std::string port)
// {
//     (void) socket;
//     std::cout << "host: " << host  << std::endl;
//     std::cout << "port: " << port  << std::endl;
//     for (size_t i = 0; i < this->servers.size(); i++)
//     {
//         if(this->servers[i].get_host().compare(host) == 0 && this->servers[i].get_port().compare(port) == 0)
//         {
//             std::cout << "url : " << mapinfo[socket].request.url << std::endl;
//             std::cout << "root + url : " <<  this->servers[i].get_root() + mapinfo[socket].request.url << std::endl;
//             if(this->servers[i].get_location_first(mapinfo[socket].request.url).compare(mapinfo[socket].request.url) == 0)        
//                 std::cout << "DONE" << std::endl;
//             //check url is valide in location;
//         }
//         struct stat fileStat;
//         if (stat((this->servers[i].get_root() + mapinfo[socket].request.url).c_str(), &fileStat) == 0)
//         {
//             if (S_ISDIR(fileStat.st_mode))
//             {
//                 // this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url();
//                 if (this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url().length() > 1)
//                 {
//                     std::cout << "redirect_URL : " << this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url() << std::endl;
//                     mapinfo[socket].request.response.redur = true;
//                     mapinfo[socket].request.response.new_redur = this->servers[i].get_location(mapinfo[socket].request.url).get_redirect_url();
//                     //check_redirect_URL
//                 }
//                 else
//                     mapinfo[i].request.response.redur = false;
//                 // for (std::map<std::string, bool>::iterator it = this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.begin(); it != this->servers[i].get_location(mapinfo[socket].request.url).MapMethods.end(); ++it)
//                 // {
//                 //     //check methodes
//                 // }
//                 // if (this->servers[i].get_location(mapinfo[socket].request.url).get_index().length() > 1)
//                 // {
                    
//                 // }
                


                
//                 std::cout << "methods : " << this->servers[i].get_location(mapinfo[socket].request.url).get_methods() << std::endl;
//                 std::cout << "index : " << this->servers[i].get_location(mapinfo[socket].request.url).get_index() << std::endl;
//                 // check is directory_listing && alias
//                 // check methode allowed
//                 // check index 
//                 // check auto indexing
                 
//                 // std::cout << "DONE DIR" << std::endl;
//             }
//             else
//                 std::cout << "File" << std::endl;
//         }
//     }
//     // exit(1);
// }







void Response::run()
{
    RemplirContentType();
//   std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
//   std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
//   std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
  
                // std::string test = "/";
                // std::string test1 = "/favicon.ico";
                // std::string path = "./pages";
                // std::string new_path = "./pages";
                // std::cout << "----> " << check_cgi << std::endl;
                if(redur == true)
                    std::cout << "---->" << new_redur << "<----"<< std::endl;
                else
                    std::cout << "---->" << url << "<----"<< std::endl;
                std::cout << "redur : " << redur << std::endl;
                int check = true;
                // struct stat fileStat;
                // stat(url.c_str(), &fileStat);
                // if (S_ISDIR(fileStat.st_mode))
                // {
                    
                // }
                if (redur == true)
                {

                    std::cout << "redur is gonna to serve" << std::endl;
                    SendResponse = "HTTP/1.1 301 Moved Permanently\r\n";
                    SendResponse += "Content-Type: text/html\r\n";
                    SendResponse += "Location: " + new_redur;
                }
                else
                {
                    
                    if (check_cgi == false)
                    {
                        if (url.compare("/favicon.ico") == 0)
                        {
                            check = false;
                            url = PATH;
                            url += "/images/me.png";
                        }
                        // if (url.compare("/") == 0)
                        // {
                        //     check = false;
                        //     url = PATH;
                        //     url += "/index.html";
                        // }
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
                    // if(url.find(url) == std::string::npos)
                    // {
                    //     // new_url = root; 
                    //     new_url = url;   
                    // }
                    // else
                    new_url = url;    
                    if (directory_listing == 1)
                    {
                        // std::string tmp;
                        // tmp = root;
                        // tmp += url;
                        // // url = tmp; 
                        std::cout << "-->new_path directory_listing : "<< url << std::endl;
                        SendResponse += AddContentType();
                        SendResponse += "\r\n";
                        SendResponse += generateHTML(url.c_str());
                        dir = true;
                        // std::cout << "->new_url : "<< tmp << std::endl;
                        // std::ifstream file(tmp.c_str());
                        // if (!file.is_open()) 
                        // {
                        //     std::cerr << "[Server] Impossible d'ouvrir image " << "\n";
                        // }
                        // // std::cout << "here" << std::endl;
                        // buffer << file.rdbuf();
                        // file.close();
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
                        SendResponse += AddContentType();
                        // SendResponse += "Content-Type: video/mp4\r\n";
                    }
                    else
                    {
                        std::cout << "new_url : "<< new_url << std::endl;
                        if(check_cgi == true)
                            new_url = url;
                        std::ifstream file(new_url.c_str());
                        if (!file.is_open()) 
                        {
                            file.close();
                            std::string tmp;
                            tmp = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/notFound.html";
                            std::ifstream file(tmp);
                            // check = 0;
                            if (!file.is_open())
                            {
                            std::cerr << "[Server 404] Impossible d'ouvrir le fichier here" << "\n";
                            }
                            // std::cerr << "[Server 1] Impossible d'ouvrir le fichier here" << "\n";
                            buffer << file.rdbuf();
                            file.close();
                        }
                        else
                        {
                            buffer << file.rdbuf();
                            file.close();
                            SendResponse += AddContentType();
                        }
                        // SendResponse += "Content-Type: text/html\r\n";                    
                    }
                    // if (dir == false)
                    // {
                    //     SendResponse = AddContentType();
                    // }
                    // if (url.compare("/ErrorPages") != 0)
                    // {
                    //     SendResponse += "\r\n";
                    //     SendResponse += buffer.str();
                    // }
                    
                if(directory_listing == false)
                {
                    SendResponse += "\r\n";
                    SendResponse += buffer.str();
                }
                // if(new_url.find(".html") != std::string::npos)
                // {
                //     std::cout << "---------------------------------"  << std::endl;
                //     std::cout << "SendResponse : " << buffer.str() << std::endl;
                //     std::cout << "---------------------------------"  << std::endl;
                // }
                }
                check_cgi = false;
                check = true;
                redur = false;
                directory_listing = false;
}