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

#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

Cgi::Cgi()
{
   std::cout << "\033[32m" << "+++++++++++++++++++++++++++++CGI++++++++++++++++++++++++++++++++++\033[0m" << std::endl;
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

void Cgi::run()
{
  // std::cout << "\033[0;31m" << "*******************************HEADER*************************************" << "\033[0m" << std::endl;
  // std::cout << "\033[0;31m" << header << "\033[0m" << std::endl;
  // std::cout << "\033[0;31m" << "**************************************************************************" << "\033[0m" << std::endl;
  // std::cout << "\033[0;33m" << "********************************BODY*************************************" << "\033[0m" << std::endl;
  // std::cout << "\033[0;33m" << body << "\033[0m" << std::endl;
  // std::cout << "\033[0;33m" << "*************************************************************************" << "\033[0m" << std::endl;
    const char *env[] = 
    {
      "PATH=/usr/bin/python3",
      "HOME=/home/user",
      nullptr
    };

    // Open a temporary file to store the output
    FILE* outputFile = fopen("output.txt", "w");
    if (!outputFile) 
    {
        std::cerr << "Failed to open output file\n";
        return;
    }
    
    
    pid_t pid = fork();
    if (pid < 0) 
    {
      // Fork failed
      std::cerr << "Fork failed\n";
      return ;
    } 
    else if (pid == 0) 
    {
      // Child process
      const char *argv[] = 
      {
        "PATH=/usr/bin/python3",
        "Solix.py",     
        nullptr
      };

      // Redirect stdout to the temporary file
      dup2(fileno(outputFile), STDOUT_FILENO);
      fclose(outputFile);
      
      int numEnvVars;
      for (numEnvVars = 0; env[numEnvVars] != nullptr; ++numEnvVars);
      const char* newEnv[numEnvVars + 2]; // +2 for the new variable and nullptr
      for (int i = 0; i < numEnvVars; ++i) 
      {
          newEnv[i] = env[i];
      }
      std::string newbody = "BODY="; 
      newbody += body; 
      // std::cout << newbody << std::endl;
      newEnv[numEnvVars] = newbody.c_str();
      newEnv[numEnvVars + 1] = nullptr;  
      execve("/usr/bin/python3", const_cast<char* const*>(argv), const_cast<char* const*>(newEnv));
      std::cerr << "execv() failed\n";
      return ;
    } 
    else 
    {
        int status;
        waitpid(pid, &status, 0);

        // Close the file
        fclose(outputFile);

        // Read the output from the file
        FILE* inputFile = fopen("output.txt", "r");
        if (!inputFile) 
        {
            std::cerr << "Failed to open input file\n";
            return;
        }
        char buffer[4096];
        std::ostringstream output;
        while (fgets(buffer, sizeof(buffer), inputFile) != nullptr) 
        {
            output << buffer;
        }
        std::string outputStr = output.str();

        fclose(inputFile);

        std::cout << "Output from Python script:\n" << outputStr << std::endl;
        // remove("output.txt");
    }
}
