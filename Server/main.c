#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "tcp_socket_server.h"
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char* argv[]) {

    printf("###############################################\n");
    printf("###############################################\n");
    printf("################ Socket Server ################\n");
    printf("###############################################\n");
    printf("###############################################\n");
    printf("----------------- C Challenge -----------------\n");
    printf("- Eduardo Alves da Silva\n");
    printf("- Date: 23 november 2024\n");
    printf("-----------------------------------------------\n");


    // 1. Setup server port
    int serverPort = getCommandLinePort(argc,argv);
    if(serverPort < 0) {
        fprintf(stderr,"\nError on the server startup: %s",
                SERVER_ERRORS_MESSAGES[-serverPort]);
        printf("\nExiting...");
        return serverPort;
    }
    printf("\n>>> Server PORT: %d",serverPort);


    // 2. Create the server to accept one client connection
    int commChannel = acceptConnections(serverPort);
    if( commChannel < 0) {
        fprintf(stderr,"\nError on the communication creation: %s.",
                SERVER_ERRORS_MESSAGES[-commChannel]);
        closeConnections();
        printf("\nExiting...");
        return commChannel;
    }
    printf("\n\n>>>>>> CONNECTION ESTABLISHED <<<<<\n\n");


    // 3. Send/receive data from the client

    char buffer[SOCKET_BUFFER_SIZER] = {0};
    char* testMessage = "Message sent from server";
    int valRead;
    // Send
    send(commChannel, testMessage, strlen(testMessage),0);
    // Receive
    valRead = read(commChannel, buffer, SOCKET_BUFFER_SIZER);
    printf("\nMessage received from the client: %s",buffer);


    closeConnections();
    printf("\n###############################################\n");
    printf("##################### END #####################\n");
    printf("###############################################\n");

    return 0;
}
