#include "hw4.h"
#define BUFFER_SIZE 1024

int main() {
    int listenfd, connfd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    ChessGame game;

    // Create socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set options to reuse the IP address and IP port if either is already in use
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))");
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt))");
        return -1;
    }

    // Bind socket to port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(listenfd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listenfd, 1) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    INFO("Server listening on port %d", PORT);
    // Accept incoming connection
    if ((connfd = accept(listenfd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    INFO("Server accepted connection");

    initialize_game(&game);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(connfd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("Message Recived Server:%s\n", buffer);
        int com = receive_command(&game, buffer, connfd, true);
        printf("Server Receive: %d\n", com);
        if (com == COMMAND_FORFEIT){
            break;
        }
        
        
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';
        printf("Message Sent Server:%s\n", buffer);
        com = send_command(&game, buffer, connfd, false);
        while(com == COMMAND_ERROR || com == COMMAND_UNKNOWN){
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strlen(buffer)-1] = '\0';
            com = send_command(&game, buffer, connfd, false);
        }
        printf("Server Send: %d\n", com);
        if (com == COMMAND_FORFEIT){
            break;
        }

    }
    printf("Server connection closing\n");
    close(connfd);
    close(listenfd);
    return EXIT_SUCCESS;
}

