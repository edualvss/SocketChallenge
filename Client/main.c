#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#include "tcp_socket_client.h"

int main(int argc, char *argv[]) {

    printf("***********************************************\n");
    printf("***********************************************\n");
    printf("**************** Socket Client ****************\n");
    printf("***********************************************\n");
    printf("***********************************************\n");
    printf("----------------- C Challenge -----------------\n");
    printf("- Eduardo Alves da Silva\n");
    printf("- Date: 23 november 2024\n");
    printf("-----------------------------------------------\n");

    // 1. Load Server Address and Port
    struct sockaddr_in serverAddress;
    int status = getCommandLineAddress(argc,argv,&serverAddress);
    int serverPort = getCommandLinePort(argc,argv);
    if( status < 0 || serverPort < 0 ) {
        int errorCode = status < 0 ? status : serverPort;
        fprintf(stderr,"\nError on the client startup: %s",
                CLIENT_ERROR_MESSAGES[-errorCode]);
        printf("\nExiting...");
        return errorCode;
    }

    printf("\n>>> Trying to connect to:\n"
           "* Server Address: %s\n"
           "* Server Port: %d\n",
           inet_ntoa(serverAddress.sin_addr),
           serverPort);

    int socketId = establishConnection(&serverAddress,serverPort);
    if( socketId < 0) {
        printf("\n - Error on establish connection with the server: %s",CLIENT_ERROR_MESSAGES[-socketId]);
        printf("\nExiting...");
        return status;
    }

    printf("\n\n>>> Connection Established!\n");


    return 0;
}
