 /* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rarraji <rarraji@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 15:35:12 by rarraji           #+#    #+#             */
/*   Updated: 2024/05/02 09:13:07 by rarraji          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "cgi.hpp"
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


Cgi::Cgi()
{
}
Cgi::~Cgi()
{
  
}
void Cgi::SetHeader(std::string header_tmp)
{
  this->header = header_tmp;
}
void Cgi::SetBody(std::string body_tmp)
{
  this->body = body_tmp;
}

std::string Cgi::GetHeader()
{
  return(this->header);
}
std::string Cgi::GetBody()
{
  return(this->body);
}

std::string itoa(int number) 
{
    std::ostringstream ss;
    ss << number;
    return ss.str();
}
void Cgi::run()
{
    size_t pos = header.find("Content-Type: ");
    std::string content_type = header.substr(pos + 14, header.find("\r\n", pos) - (pos + 14));
    std::string cc = std::string("CONTENT_TYPE=") + content_type;
    std::string cc1 = std::string("QUERY_STRING=") + querystingcgi;
    methode = "REQUEST_METHOD=" + methode;
    std::string url_tmp =  url;
    url = "PATH_INFO=" + url;
    std::string envp[1] = 
    {
      ("CONTENT_LENGTH=1000000000000\0"),
    };
    char *en[6];
    for (int i = 0; i < 1; i++)
      en[i] = &envp[i][0];
    en[1] = (char *)cc.c_str();
    en[2] = (char *)cc1.c_str();
    en[3] = (char *)methode.c_str();
    en[4] = (char *)url.c_str();
    en[5] = NULL;
    chdir(root.c_str());
    int outputFile = open("output.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    std::ofstream ss("./input.txt");
    if(ss.is_open())
      ss << body;
    ss.close();
    int inputFile = open("./input.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (outputFile == -1) 
    {
        std::cerr << "Failed to open output file 1\n";
        return;
    }
    pid_t pid = fork();
    if (pid < 0) 
    {
      std::cerr << "Fork failed\n";
      return ;
    } 
    else if (pid == 0) 
    {
      std::string arg[1] = 
      {
        ("/usr/bin/python3\0"),
      };

      char *argg[3];
      int i = 0;
      for (i = 0; i < 1; i++)
        argg[i] = &arg[i][0];
      argg[1] = (char *)url_tmp.c_str();
      argg[2] = NULL;
      dup2(inputFile, STDIN_FILENO);
      dup2(outputFile, STDOUT_FILENO);

      close(outputFile);
      close(inputFile);
      execve("/usr/bin/python3", const_cast<char* const*>(argg), en);
      std::cerr << "execv() failed\n";
      return ;
    } 
    else 
    {
        int status;
        time_t start = time(NULL);
        time_out = 0;
        while (waitpid(pid, &status, WNOHANG) != -1)
        {
          time_t end = time(NULL);
          if(end - start > 25)
          {
            std::cerr << "TIME OUT \n";
            time_out = 1;
            kill(pid, 15);
            return ;
          }
        }
        status_code_error = 0;
        status = status >> 8;
        if(status != 0)
        {
          status_code_error = 1;
          return;
        }
        close(outputFile);
        close(inputFile);
        std::ostringstream output;
        int outputFile = open("output.txt", O_RDONLY);
        size_t size = 0;
        std::string buffer;
        char buff[1024];
        while (int s = read(outputFile, buff, 1024))
        {
          size += s;
          std::string bb(buff,s);
          buffer.append(bb);
        }

        if(buffer.find("HTTP/1.1") == std::string::npos)
        {
          status_code_error = 1;
        }
        else if(buffer.find("HTTP/1.1") != std::string::npos && buffer.find("Content-Length:") == std::string::npos)
        {
          size_t pos = buffer.find("\r\n") + 2;
          std::string content_L = "Content-Length: " + itoa(buffer.substr(buffer.find("\r\n\r\n") + 4).size()) + "\r\n";
          buffer.insert(pos, content_L);
        }
        close(outputFile);
        std::ofstream file("output.txt", std::ios::in);
        file << buffer;
        file.close();
    }
}