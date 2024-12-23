#include "enum_const.h"

#include "proxy.h"
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

int create_server_socket() {
    struct sockaddr_in server_addr;
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        printf("Error while creating server socket\n");
        return SOCKET_ERROR;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_UNSPEC;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    printf("Server socket created\n");

    int err = bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (err == BIND_ERROR) {
        perror("Failed to bind server socket");
        close(server_socket);
        return SOCKET_ERROR;
    }

    printf("Server socket bound to %d\n", server_addr.sin_addr.s_addr);

    err = listen(server_socket, MAX_USERS_COUNT);
    if (err == LISTEN_ERROR) {
        perror("Server socket failed to listen");
        close(server_socket);
        return SOCKET_ERROR;
    }
    return server_socket;
}

int connect_to_remote(char *host) {
    struct addrinfo hints, *res0;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo((char *) host, HTTP, &hints, &res0);
    if (status != ADD_INFO_STATUS_ERROR) {
        perror("Error get addr info");
        freeaddrinfo(res0);
        return SOCKET_ERROR;
    }
    int dest_socket = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol);
    if (dest_socket == SOCKET_ERROR) {
        perror("Error while creating remote server socket");
        return SOCKET_ERROR;
    }

    int err = connect(dest_socket, res0->ai_addr, res0->ai_addrlen);
    if (err == SOCKET_ERROR) {
        printf("Error while connecting to remote server\n");
        close(dest_socket);
        freeaddrinfo(res0);
        return SOCKET_ERROR;
    }
    return dest_socket;
}

int read_request(int client_socket, char *request) {
    ssize_t bytes_read = read(client_socket, request, MAX_BUFFER_SIZE);
    if (bytes_read < 0) {
        perror("Error while read request");
        close(client_socket);
        return EXIT_FAILURE;
    }
    if (bytes_read == 0) {
        perror("Connection closed from client");
        close(client_socket);
        return EXIT_FAILURE;
    }
    request[bytes_read] = END_STR;
    printf("Received request:\n%s", request);
    return EXIT_SUCCESS;
}
