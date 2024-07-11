/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:27 by rarraji           #+#    #+#             */
/*   Updated: 2024/06/04 10:37:12 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"
#include "./config_file/utils.hpp"

Request::Request()
{
  compareLenBody = 0;
  check_l3mara = 0;
  Get = true;
  last = true;
  start = 0;
  s = 0;
  se = 0;
}

std::string convertIntToString1(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}

std::string generateErrorPage3(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}

std::string Request::get_error_pages(std::string key)
{
    std::map<std::string, std::string>::iterator it = error_pages.find(key);
    if (it == error_pages.end())
        return "";
    return it->second;
}

int Request::read_socket(int socket)
{
    int valread;
    fcntl(socket, F_SETFL, O_NONBLOCK , FD_CLOEXEC);
    valread = read(socket, buffer, sizeof(buffer) - 1);  
    return (valread);
}

void Request::AddHeaderReq(int valread)
{
  size_t pos;
  if ((pos = request.find("\r\n\r\n")) != std::string::npos && !check)
  {
    check = true;
    compareLenBody = valread - pos + 4;
    header = request;
  }
  else
    compareLenBody += valread;
}

void Request::CheckChunked()
{
  size_t pos;
  if (header.find("chunked") != std::string::npos)
  {
    chunked = true;
  }
  if ((pos = body.find("\r\n\r\n")) != std::string::npos)
  {
    body = body.substr(pos + 4 , body.length());
  }
}

int hexStringToDecimal(const std::string& hexString) 
{
    std::stringstream ss;
    ss << std::hex << hexString;
    int decimalValue;
    ss >> decimalValue;
    return decimalValue;
}
int convertStringToInt1(const std::string& str) 
{
  std::stringstream ss(str);
  int result;
  
  if (ss >> result) 
    return result;
  return(0);  
}


void Request::RegContent(int nb)
{
  std::string newb;
  body = body.substr(nb + 2 , body.length());
}

void Request::AddHeaderBody()
{
  size_t pos;
  std::string num_one;
  std::string num_one_2;
  if ((pos = header.find("\r\n\r\n")) != std::string::npos)
  {
    header = header.substr(0, pos + 2);
  }
  if(!Get)
  {
    CheckChunked();
    int nb = 0;
    int i = 0;
    while(true && chunked)
    {
      std::stringstream ss(body);
      getline(ss, num_one, '\n');
      if(hexStringToDecimal(num_one) == 0)
        break;
      nb =  hexStringToDecimal(num_one);
      body = body.substr(body.find("\n") + 1, body.length());
      new_body += body.substr(start, nb);
      body = body.substr(nb + 2, body.length());
      start = 0;
      i++;
    }
  }
  if (!Get && !chunked)
  {
      new_body = body;
  }
}
void Request::CreatFiles(std::string NameFile, std::string buf, bool check)
{
    if (check)
    {
      std::string tmp = root + get_location(url).get_upload_dir().c_str();
      chdir(tmp.c_str());
      std::ofstream outputFile(NameFile.c_str() , std::ios::binary);
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
    else
    {
      std::string tmp = root + get_location(url).get_upload_dir().c_str();
      chdir(tmp.c_str());
      std::ofstream outputFile(NameFile.c_str());
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
}

int checkImgOrText(std::string buf)
{
  if (buf.find("image") != std::string::npos)
    return(1);
  return (0);  
}


std::string Request::generateErrorPage(int statusCode, const std::string& errorMessage) 
{
  std::ostringstream ss;
  ss << "<html><head><title>Error " << statusCode << "</title></head>"
     "<body><h1>Error: " << statusCode << "</h1>"
     "<p>" << errorMessage << "</p>"
     "</body></html>";
  return ss.str();
}
void Request::UploadFiles()
{
  std::stringstream ss(new_body);
  std::string buf;
  bool check = false;
  getline(ss,buf);
  std::string Myboundary = buf.substr(0 , buf.length() - 1);
  while (getline(ss,buf))
  {
    size_t pos;
    if ((pos = buf.find("filename=")) != std::string::npos || (pos = buf.find("name=")) != std::string::npos)
    {
      std::string content;
      std::string NameFile;
      if ((pos = buf.find("filename=")) != std::string::npos)
      {
        NameFile += buf.substr(pos + 10 , buf.length());
      }
      else if ((pos = buf.find("name=")) != std::string::npos)
      {
        NameFile += buf.substr(pos + 6 , buf.length()); 
      }
      NameFile = NameFile.substr(0 , NameFile.length() - 2);
      NameFile += '\0';
      if ((pos = buf.find("filename=")) != std::string::npos)
        getline(ss,buf);
      if(checkImgOrText(buf))
        check = true;
      getline(ss,buf);
      while (getline(ss,buf))
      {
        if(buf.find(Myboundary) != std::string::npos)
          break;
        content += buf;
        content += "\n";
      }
      CreatFiles(NameFile, content, check);
    }
  }
}

void Request::checkQuery()
{
  std::string str;
  std::string tmp;
  std::vector<std::string> MyQeury;
  size_t pos;
  if((pos = url.find("?")) != std::string::npos)
  {
    str = url.substr(pos+1, url.length());
    querystingcgi = str;    
    url = url.substr(0, pos);
  }
  while ((pos = str.find("&")) != std::string::npos)
  {
    tmp = str.substr(0, pos);
    str = str.substr(pos + 1, str.length());
    MyQeury.push_back(tmp);
  }
  tmp = str.substr(0, str.length());
  MyQeury.push_back(tmp);
}

void Request::SaveHost_Port(std::string tmp_host)
{
  host = tmp_host.substr(6, tmp_host.length());
}
int Request::check_req_valid()
{
  std::stringstream ss(header);
  std::string buf;
  int i = 0;
  while(getline(ss, buf, '\n'))
  {
    if (i == 0)
    {
      std::stringstream ss(buf);
      std::string buff;
      int j = 0;
      while (getline(ss, buff, ' '))
      {
        if (j == 0)
        {
          methode = buff;

          if(methode.compare("GET") != 0 && methode.compare("POST") != 0 && methode.compare("DELETE") != 0)
          {
            response.status = 501;
            if(get_error_pages(convertIntToString1(501)).empty())
            {
                response.errorpage = 1;
                response.url = generateErrorPage3(501, "Not Implemented");
            }
            else
            response.url = get_error_pages(convertIntToString1(501)); 
            return (0);
          }
        }
        if (j == 1)
        {
          url = buff;
          checkQuery();
        if(get_location(url).get_methods(methode) == false)
        {
            response.status = 405;
            if(get_error_pages(convertIntToString1(4)).empty())
            {
                response.errorpage = 1;
                response.url = generateErrorPage3(405, "Not allowed");
            }
            else
            response.url = get_error_pages(convertIntToString1(405)); 
            return (0);
        }
        }
        if (j == 2)
        {
          buf += '\0';
          if (buff.compare("HTTP/1.1") != 0)
          {
            
            // std::cout << "-->"<< buff  << "-->" << std::endl;
            // std::string reqerror = "/Users/rarraji/Desktop/prj/WebServMerge/pages/ErrorPages/HTTP_version.html";
            // return(0);
            // response with 505
            // error
          }
        }  
        j++;
      }
    }
    host = header.substr(header.find("Host:") , header.length());
    host = host.substr(0 , host.find("\r\n"));
    SaveHost_Port(host);
    i++;
  }
  return(1);
}

void Request::set_location(std::string index, location_param value)
{
    if(location.find(index) != location.end())
        utils::print_error("duplicate location : ", index);
    location[index] = value;
}

location_param Request::get_location(std::string key)
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


void Request::Check_read(int socket, fd_set &read_fds, fd_set &write_fds)
{
  int valread;
  valread = read_socket(socket);
  size_t pos = 0;
  if (valread == 0 || valread == -1)
  {
    FD_CLR(socket, &read_fds);
    close(socket);
    return;
  }
  if (valread > 0)
  {
      buffer[valread] = '\0';
      request.append(buffer, valread);
      AddHeaderReq(valread);
      if ((pos = request.find("Content-Length: ")) != std::string::npos)
      {
        Get = false;
        body_lenght = std::atoi(request.substr(pos + 16,  request.find("\r\n", pos + 16) - pos + 16).c_str());
        header_len = pos + 16;
      }
  }

    size_t poss;
    if((poss = request.find("boundary")) != std::string::npos && check_l3mara == 0)
    {
      l3mara = "--";
      l3mara += request.substr(poss + 9, request.length());
      l3mara = l3mara.substr(0, l3mara.find('\r'));
      new_3mara = l3mara + "--";
      check_l3mara = 1;
    }
  if ((request.find("\r\n\r\n") != std::string::npos && Get)  || (request.find("\r\n\r\n0") != std::string::npos && !Get) || (request.find(new_3mara) != std::string::npos && !Get))
  {
    response.errorpage = 0;
      int tmp = 0;
      if(convertStringToInt1(MaxBodySize) < body_lenght)
      {
        tmp = 1;
        response.status = 413;
        if(get_error_pages(convertIntToString1(413)).empty())
        {
            response.errorpage = 1;
            response.url = generateErrorPage3(413, "Content_Too_Large");
        }
        else
         response.url = get_error_pages(convertIntToString1(413)); 
        
      }
    
      if(check_req_valid() && tmp == 0)
      {
        if(methode == "DELETE")
        {
          std::string del = root + url;
          response.status = 204;
          response.methode = "DELETE";
          if(access(del.c_str(), F_OK) !=0)
          {
              response.status = 404;
                if(get_error_pages(convertIntToString1(404)).empty())
                {
                    response.errorpage = 1;
                    response.url = generateErrorPage3(404, "Not Found");
                }
                else
                response.url = get_error_pages(convertIntToString1(404));

          }
          else if(utils::deletePath(del.c_str())==-1)
          {
            int res = utils::removeDirectoryRecursively(del.c_str());
            if(res != 0)
            {
                response.status = 500;
                if(get_error_pages(convertIntToString1(500)).empty())
                {
                    response.errorpage = 1;
                    response.url = generateErrorPage3(500, "Internal Server Error");
                }
                else
                response.url = get_error_pages(convertIntToString1(500));
            }

          }
          // response.url = generateErrorPage3(204, "No Content");
        }
        else
        {
          
          if(!Get)
            body = request.substr(header_len, request.length());
          AddHeaderBody();
          if ((pos = header.find("\r\n\r\n")) != std::string::npos)
            header = header.substr(0, pos + 2);
          if (get_location(url).get_upload_dir().length() > 0 && methode == "POST")
          {
            if(get_location(url).get_methods("POST") == false)
            {
              response.status = 405;
                if(get_error_pages(convertIntToString1(405)).empty())
                {
                    response.errorpage = 1;
                    response.url = generateErrorPage3(405, "Method Not Allowed");
                }
                else
                response.url = get_error_pages(convertIntToString1(404));
            }
            else
            {
              cgi = true;
              UploadFiles();
            }
          }
          if(response.errorpage == 0)
          {  
            response.SetHeader(header);
            response.SetBody(body);
            response.SetUrl(url);
            response.check_body = false; 
          }
          if (response.url.find(".py") != std::string::npos && cgi == false && methode == "POST")
          {
              struct stat sb;
              std::string tmp_url = root + url;
              if(stat(tmp_url.c_str(), &sb) != 0)
              {
                response.status = 404;
                if(get_error_pages(convertIntToString1(404)).empty())
                {
                    response.errorpage = 1;
                    response.url = generateErrorPage3(404, "notfound");
                }
                else
                response.url = get_error_pages(convertIntToString1(404));
              }
              else
              {
                response.check_cgi = true;
                Cgi cgi;
                cgi.SetHeader(header);
                cgi.SetBody(new_body);
                cgi.url = root + response.url;
                cgi.root = root;
                cgi.querystingcgi = querystingcgi;
                cgi.methode = methode;
                cgi.run();
                if (cgi.time_out == 1)
                {
                  response.check_cgi = false;
                  response.status = 504;
                  if(get_error_pages(convertIntToString1(504)).empty())
                  {
                      response.errorpage = 1;
                      response.url = generateErrorPage3(504, "getwayTimeout");
                  }
                  else
                  response.url = get_error_pages(convertIntToString1(504));
                }
                if (cgi.status_code_error == 1)
                {
                  response.check_cgi = false;
                  response.status = 500;
                  if(get_error_pages(convertIntToString1(500)).empty())
                  {
                      response.errorpage = 1;
                      response.url = generateErrorPage3(500, "INTERNAL SERVER ERROR");
                  }
                  else
                  response.url = get_error_pages(convertIntToString1(500));
                }
                else
                  response.url = "/output.txt";
              }
          }
        }
      }
      request = "";
      FD_CLR(socket, &read_fds);
      FD_SET(socket, &write_fds);
      compareLenBody = 0;
      body_lenght = 0;
      Get = true;
      check = false;
  }
}