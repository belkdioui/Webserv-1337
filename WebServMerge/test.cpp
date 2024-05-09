#include <errno.h>
#include <iostream>
#include <cstring>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8001
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

void error(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int create_server_socket(void) 
{
    struct sockaddr_in sa;
    int socket_fd, status;

    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    sa.sin_port = htons(PORT);

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) 
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    status = bind(socket_fd, (struct sockaddr *)&sa, sizeof(sa));
    if (status == -1) 
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    return socket_fd;
}

void send_message_to_client(int client_socket, const char *msg) {
    int status = send(client_socket, msg, strlen(msg), 0);
    if (status == -1) {
        perror("Send error");
    }
}

void accept_new_connection(int server_socket, fd_set *all_sockets, int *fd_max) 
{
    int client_fd, status;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    client_fd = accept(server_socket, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd == -1) 
    {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    FD_SET(client_fd, all_sockets);
    if (client_fd > *fd_max) {
        *fd_max = client_fd;
    }

    std::cout << "[Server] Accepted new connection on client socket " << client_fd << ".\n";

    char msg_to_send[BUFFER_SIZE];
    snprintf(msg_to_send, sizeof(msg_to_send), "Welcome. You are client fd [%d]\n", client_fd);

    status = send(client_fd, msg_to_send, strlen(msg_to_send), 0);
    if (status == -1) {
        perror("Send failed");
    }
}

void read_data_from_socket(int socket, fd_set *all_sockets, int *fd_max, int server_socket) 
{
    char buffer[BUFFER_SIZE];
    int bytes_read, status;

    bytes_read = recv(socket, buffer, sizeof(buffer), 0);
    if (bytes_read <= 0) {
        if (bytes_read == 0) {
            std::cout << "[" << socket << "] Client socket closed connection.\n";
        } else {
            perror("Recv error");
        }
        close(socket);
        FD_CLR(socket, all_sockets);
    } else {
        std::cout << "[" << socket << "] Got message: " << buffer;

        char msg_to_send[BUFFER_SIZE];
        snprintf(msg_to_send, sizeof(msg_to_send), "[%d] says: %s", socket, buffer);

        for (int j = 0; j <= *fd_max; j++) {
            if (FD_ISSET(j, all_sockets) && j != server_socket && j != socket) {
                status = send(j, msg_to_send, strlen(msg_to_send), 0);
                if (status == -1) {
                    perror("Send error");
                }
            }
        }

        // If the message is from the server, broadcast it to all clients
        if (socket == server_socket) {
            for (int j = 0; j <= *fd_max; j++) {
                if (FD_ISSET(j, all_sockets) && j != server_socket) {
                    send_message_to_client(j, msg_to_send);
                }
            }
        }
    }
}

int main(void) {
    std::cout << "---- SERVER ----\n\n";

    int server_socket, status;
    // fd_set write_sockets, read_fds;
    // fd_set write_sockets_copy, read_fds_copy;
    int fd_max = 0;
    struct timeval timer;

    server_socket = create_server_socket();

    status = listen(server_socket, MAX_CLIENTS);
    if (status == -1) {
        perror("Listen error");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&write_sockets);
    FD_ZERO(&read_fds);
    FD_SET(server_socket, &read_fds);
    fd_max = server_socket;

    std::cout << "[Server] Listening on port " << PORT << "\n";

    while (1) {
        read_fds = all_sockets;
        timer.tv_sec = 2;
        timer.tv_usec = 0;

        status = select(fd_max + 1, &read_fds, NULL, NULL, &timer);
        if (status == -1) {
            perror("Select error");
            exit(EXIT_FAILURE);
        } else if (status == 0) {
            std::cout << "[Server] Waiting...\n";
            continue;
        }

        for (int i = 0; i <= fd_max; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == server_socket) {
                    accept_new_connection(server_socket, &all_sockets, &fd_max);
                } else {
                    read_data_from_socket(i, &all_sockets, &fd_max, server_socket);
                }
            }
        }
    }

    return 0;
}



// std::string generateHTML(const char* path) 
// {
//     std::ostringstream ss;
//     DIR *dir = opendir(path);
//     if (!dir) {
//         return "";
//     }

//     ss << "<html><head><title>Directory Listing</title><style>h1 {text-align:center;}</style></head><body><h1>Directory Listing</h1><ul><br>";

//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_name[0] != '.') {
//             ss << "<li><a href=\"" << entry->d_name << "\">" << entry->d_name << "</a></li>";
//         }
//     }

//     ss << "</ul></body></html>";
//     closedir(dir);
//     return ss.str();
// }


