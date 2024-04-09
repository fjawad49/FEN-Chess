#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "hw4.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    ChessGame game;
    char buffer[BUFFER_SIZE] = {0};
    int connfd = 0;
    struct sockaddr_in serv_addr;

    // Connect to the server
    if ((connfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    if (connect(connfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    initialize_game(&game);
    display_chessboard(&game);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strlen(buffer)-1] = '\0';
        printf("Message Sent Client:%s\n", buffer);
        int com = send_command(&game, buffer, connfd, true);
                display_chessboard(&game);
        while(com == COMMAND_ERROR || com == COMMAND_UNKNOWN){
            memset(buffer, 0, BUFFER_SIZE);
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strlen(buffer)-1] = '\0';
            com = send_command(&game, buffer, connfd, true);
        }
        printf("Client Send: %d\n", com);
        if(com == COMMAND_SAVE || com == COMMAND_DISPLAY){
            continue;
        }
        if(com == COMMAND_FORFEIT){
            game.currentPlayer = 1;
            break;
        }

        memset(buffer, 0, BUFFER_SIZE);
        int nbytes = read(connfd, buffer, BUFFER_SIZE);
        if (nbytes <= 0) {
            perror("[Server] read() failed.");
            exit(EXIT_FAILURE);
        }
        printf("Message Received Client:%s\n", buffer);
        com = receive_command(&game, buffer, connfd, false);
        printf("Client Receive: %d\n", com);
                display_chessboard(&game);
        if (com == COMMAND_FORFEIT){
            game.currentPlayer = 0;
            break;
        }
    }
    printf("Client connection closing\n");
    // Please ensure that the following lines of code execute just before your program terminates.
    // If necessary, copy and paste it to other parts of your code where you terminate your program.
    FILE *temp = fopen("./fen.txt", "w");
    char fen[200];
    chessboard_to_fen(fen, &game);
    fprintf(temp, "%s", fen);
    fclose(temp);
    close(connfd);
        printf("hello\n");
    return EXIT_SUCCESS;
}
