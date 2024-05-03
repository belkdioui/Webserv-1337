#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int socket_server = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_server < 0) {
        perror("socket creation failed");
        return 1;
    } else {
        std::cout << "socket creation successful" << std::endl;
    }

    sockaddr_in struct_addr;
    struct_addr.sin_family = AF_INET;
    struct_addr.sin_port = htons(8080);
    struct_addr.sin_addr.s_addr = INADDR_ANY;

    int on = 1;
    if (setsockopt(socket_server, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0) {
        std::cerr << "error: setsockopt() failed" << std::endl;
        close(socket_server);
        return 1;
    }

    if (bind(socket_server, (struct sockaddr*)&struct_addr, sizeof(struct_addr)) == -1) {
        perror("Bind failed");
        close(socket_server);
        return 1;
    }
    std::cout << "Bind successful" << std::endl;

    if (listen(socket_server, 1) == -1) {
        perror("Listen failed");
        close(socket_server);
        return 1;
    }
    std::cout << "Server is listening for incoming connections" << std::endl;

    fd_set read_fd;
    int client_socket = -1;
    int max_fd;
    FD_ZERO(&read_fd);

    while (true) {
        max_fd = socket_server;

        if (client_socket > 0) {
            FD_SET(client_socket, &read_fd);
            max_fd = std::max(max_fd, client_socket);
        }

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        FD_SET(socket_server, &read_fd);

        int activity = select(max_fd + 1, &read_fd, NULL, NULL, &timeout);

        if (activity < 0) {
            perror("select failed");
            close(socket_server);
            if (client_socket > 0) {
                close(client_socket);
            }
            return 1;
        } else if (activity == 0) {
            std::cout << "Timeout occurred, waiting for incoming connections..." << std::endl;
        } else {
            if (FD_ISSET(socket_server, &read_fd)) {
                sockaddr_in clientAddress;
                socklen_t clientAddrLen = sizeof(clientAddress);

                client_socket = accept(socket_server, (struct sockaddr*)&clientAddress, &clientAddrLen);
                if (client_socket < 0) {
                    perror("accept failed");
                } else {
                    std::cout << "New client connected: " << client_socket << std::endl;
                }
            } else if (client_socket > 0 && FD_ISSET(client_socket, &read_fd)) {
                char buffer[1024];
                ssize_t bytesRead = read(client_socket, buffer, sizeof(buffer));

                if (bytesRead == -1) {
                    perror("Error reading from socket");
                } else if (bytesRead == 0) {
                    std::cerr << "Connection closed by peer" << std::endl;
                    close(client_socket);
                    break;
                } else {
                    buffer[bytesRead] = '\0';
                    std::cout << "Received data from client: " << buffer << std::endl;
                }
            }
        }
    }

    close(socket_server);
    close(client_socket);

    return 0;
}
