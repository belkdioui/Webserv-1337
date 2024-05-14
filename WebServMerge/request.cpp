/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 09:55:27 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/09 10:17:58 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

Request::Request()
{
  compareLenBody = 0;
  Get = true;
  last = true;
  start = 0;
  s = 0;
  se = 0;
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
    std::cout << "chunked" << std::endl; 
  }
  if ((pos = body.find("\r\n\r\n")) != std::string::npos)
  {
    body = body.substr(pos + 4 , body.length());
  }
}

int hexStringToDecimal(const std::string& hexString) 
{
    std::stringstream ss;
    ss << std::hex << hexString; // Utiliser std::hex pour indiquer que la chaîne est hexadécimale
    int decimalValue;
    ss >> decimalValue; // Convertit la chaîne hexadécimale en décimal
    return decimalValue;
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
    while(true && chunked)
    {
      std::stringstream ss(body);
      getline(ss, num_one, '\n');
      // getline(ss, num_one, '\r');
      // std::cout << "----->" << num_one << std::endl;
      if(hexStringToDecimal(num_one) == 0)
        break;
      nb =  hexStringToDecimal(num_one);
      body = body.substr(body.find("\n") + 1, body.length());
      new_body += body.substr(start, nb);
      // std::cout << "***> " << std::endl;
      // std::cout << new_body  << std::endl;
      // std::cout << "***> " << std::endl;
      RegContent(nb);
      start = 0;
    }
  }
  std::cout << Get << std::endl;
  if (!Get && !chunked)
  {
    // if ((pos = body.find("\r\n")) != std::string::npos)
    // {
      std::cout << "hereeeee\n";
    //   new_body = body.substr(pos + 2 , body.length());;
    // }
    // else
      new_body = body;
    // if(chunked)
    // {
      // int poss = new_body.find("----------------------------");
      //   new_body = new_body.substr(0, poss);
    // } 
  }
}
void Request::CreatFiles(std::string NameFile, std::string buf, bool check)
{
  try 
  {
    if (check)
    {
      std::ofstream outputFile(NameFile , std::ios::binary);
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
    else
    {
      std::ofstream outputFile(NameFile);
      if (!outputFile.is_open()) 
      {
        std::cerr << "Error: Unable to open file " << NameFile << std::endl;
        return;
      }
      outputFile << buf << std::endl;
    }
  } 
  catch (const std::exception& e) 
  {
    std::cerr << "Exception caught: " << e.what() << std::endl;
  }
}

int checkImgOrText(std::string buf)
{
  if (int pos = buf.find("image") != std::string::npos)
    return(1);
  return (0);  
}
void Request::UploadFiles()
{
  std::stringstream ss(new_body);
  std::string buf;
  bool check = false;
  getline(ss,buf);
  std::string Myboundary = buf.substr(0 , buf.length() - 1);
  std::cout << "=====================================ana hna" << std::endl;
  while (getline(ss,buf))
  {
    size_t pos;
    if ((pos = buf.find("filename=")) != std::string::npos || (pos = buf.find("name=")) != std::string::npos)
    {
      std::string content;
      std::string NameFile;
      if ((pos = buf.find("filename=")) != std::string::npos)
        NameFile = buf.substr(pos + 10 , buf.length());
      else if ((pos = buf.find("name=")) != std::string::npos)
      {
        NameFile = buf.substr(pos + 6 , buf.length()); 
        std::cout << "-->" << NameFile << std::endl;
      }
      NameFile = NameFile.substr(0 , NameFile.length() - 2);
      NameFile += '\0';
      std::cout << NameFile << std::endl;
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
      // std::cout << content;
    }
  }
  
  // std::cout << Myboundary << std::endl;
}

void Request::checkQuery()
{
  std::string str;
  std::string tmp;
  std::vector<std::string> MyQeury;
  size_t pos;
  std::cout << "URL : " << url << std::endl;
  if((pos = url.find("?")) != std::string::npos)
  {
    str = url.substr(pos+1, url.length());
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
  std::cout << "----------------------QUERY---------------------------" << std::endl;
  for (size_t i = 0; i < MyQeury.size(); i++)
  {
    std::cout << MyQeury[i] << std::endl;
  }
  std::cout << "-------------------------------------------------------" << std::endl; 
}


void Request::check_req_valid()
{
  // read line par line 
  std::stringstream ss(header);
  std::string buf;
  int i = 0;
  while(getline(ss, buf, '\n'))
  {
    // std::cout << "--->" << buf << std::endl;
    // check line 1
    if (i == 0)
    {
      std::stringstream ss(buf);
      std::string buff;
      int j = 0;
      while (getline(ss, buff, ' '))
      {
        if (j == 0)
        {
          if(buff.compare("GET") != 0 && buff.compare("POST") != 0 && buff.compare("DELETE") == 0)
          {
            methode = buff;
            //error    
          }
          std::cout << "Methode : " << buff << std::endl; 
        }
        if (j == 1)
        {
          url = buff;
          //check URL !!!!!!
          checkQuery();
        }
        if (j == 2)
        {
          if (buff.compare("HTTP/1.1") != 0)
          {
            // error
          }
        }  
        if (j == 3)
        {
          // error;
        }
        j++;
      }
    }
    host = header.substr(header.find("Host:") + 6, header.find("\n") + 1);
    // check lines 2 ...
    i++;
  }
}

void Request::Check_read(int socket, fd_set &read_fds, fd_set &write_fds)
{
  int valread;
  valread = read_socket(socket);
  size_t pos = 0;
  
  // header req
  if (valread > 0) 
  {
      buffer[valread] = '\0';
      request.append(buffer, valread); 
      AddHeaderReq(valread);
      if ((pos = request.find("Content-Length: ")) != std::string::npos)
      {
        // check if allow methode post configfile
        Get = false;
        // std::cout << "\033[0;35m"  << "========CHUNKED========" << "\033[0m" << std::endl;
        body_lenght = std::atoi(request.substr(pos + 16,  request.find("\r\n", pos + 16) - pos + 16).c_str());
        header_len = pos + 16;
      }
  }

  // body req && cgi && run response
  if ((request.find("\r\n\r\n") != std::string::npos && Get) || (request.find("\r\n\r\n0") != std::string::npos && !Get) || compareLenBody >= body_lenght)
  {
      check_req_valid();
      std::cout << "\033[0;35m" << "---------->>>>>ALL-REQUSTE<<<<<--------" << "\033[0m" << std::endl;
      if(!Get)
        body = request.substr(header_len, request.length());
      AddHeaderBody();
      if ((pos = header.find("\r\n\r\n")) != std::string::npos)
        header = header.substr(0, pos + 2);
      // // if (!cgi)
      UploadFiles();
      std::cout << "herre\n";
      response.SetHeader(header);
      response.SetBody(body);
      response.SetUrl(url);
      if (response.url.find(".py") != std::string::npos)
      {
        std::cout << "hnaaaaaaaaa\n";
        response.check_cgi = true;
        Cgi cgi;
        cgi.SetHeader(header);
        cgi.SetBody(body);
        cgi.run();  
      }
      else
        response.run();
      request = "";
      FD_CLR(socket, &read_fds);
      FD_SET(socket, &write_fds);
      compareLenBody = 0;
      body_lenght = 0;
      Get = true;
      check = false;
  }
}

// Transfer-Encoding