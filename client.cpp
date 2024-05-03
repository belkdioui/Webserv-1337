#include <cstdio>
#include <iostream>
#include <sys/_types/_ssize_t.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        perror("socket creation faild");
        return (1);
    }
    else
        std::cout<<"socket creation succes"<<std::endl;
    sockaddr_in struct_addr;
    struct_addr.sin_family = AF_INET;
    struct_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(struct_addr.sin_addr));
    if (connect(socket_fd,(struct sockaddr *)&struct_addr , sizeof(struct_addr)) == -1)
    {
        perror("Error connecting to server");
        close(socket_fd);
        return -1;
    }
    else 
        std::cout<<"connected !"<<std::endl;
    std::string msg;
    while(1)
    {
        std::cout<<"write your msg here :";
        std::cin>>msg;
        ssize_t bytes_written = write(socket_fd, msg.c_str(), msg.length());
        if (bytes_written == -1)
        {
            perror("Error writing to socket");
            close(socket_fd);
            return -1;
        }
        else if (bytes_written == 0)
        {
            std::cerr << "Connection closed by peer" << std::endl;
            close(socket_fd);
            return 0;
        }
        char buffer[1024];
        ssize_t bytesRead = read(socket_fd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("Error reading from socket");
        } else if (bytesRead == 0) {
            std::cerr << "Connection closed by peer" << std::endl;
            close (socket_fd);
            break;
        } else {
            buffer[bytesRead] = '\0';
            std::cout << "Received data from client: " << buffer << std::endl;
        }
    }
    close(socket_fd);
    return (0);
}