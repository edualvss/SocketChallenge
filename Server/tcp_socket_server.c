#include "tcp_socket_server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const char* SERVER_ERROR_MESSAGES[] = {
    "OK",
    "You must specify a PORT number to the start the server. In the command line specify: `-port <number>`.",
    "Check the PORT number in the command line argument. The port must be a integer positive value.",
    "Socket creation failed.",
    "Set socket option failed.",
    "Socket bind failed.",
    "Socket listen failed.",
    "Socket can not accept connections."
};

const char* END_OF_CHAT = "#";


int isPortValid(int portNumber) {
    return portNumber > 0;
}

int getCommandLinePort(int commandLineCount, char* commandLineArgs[]) {

    if( commandLineCount < 2 ) {
        return SERVER_NO_PORT;
    }

    int portNumber = 0;
    if(commandLineCount == 2) {
        portNumber = atoi(commandLineArgs[1]);
        if( !isPortValid(portNumber) ) {
            fprintf(stderr,"The command line argument `%s` is an invalid PORT number.",commandLineArgs[1]);
            return SERVER_INVALID_PORT;
        }
        printf("Assuming the first argument `%s` as the PORT number.",commandLineArgs[1]);
        return portNumber;
    }

    // Search for '-port' argument.
    for(int i = 1; i < commandLineCount; i++ ) {
        if(strcmp(commandLineArgs[i],"-port") == 0) {
            if( i == commandLineCount-1 ) {
                fprintf(stderr,"There is no NUMBER defined after -port argument.");
                return SERVER_NO_PORT;
            } else {
                portNumber = atoi(commandLineArgs[i+1]);
                if( !isPortValid(portNumber) ) {
                    fprintf(stderr,"The command line argument `%s` after '-port' is an invalid PORT number.",commandLineArgs[i+1]);
                    return SERVER_INVALID_PORT;
                } else {
                    return portNumber;
                }
            }
        }
    }

    return SERVER_NO_PORT;
}

/// Create a TCP IPv4 Socket with address reuse enabled in the specified port.
int acceptConnections(int port) {

    // 1. Create the socket
    _serverDescriptors.mainSocketId = socket(AF_INET, SOCK_STREAM, 0);
    if( _serverDescriptors.mainSocketId < 0 ) {
        return SERVER_SOCKET_CREATION_ERROR;
    }
    printf("\n* Socket created.");


    // 2. Set socket options
    int optionValue = 1;
    if (setsockopt(_serverDescriptors.mainSocketId, SOL_SOCKET, SO_REUSEADDR,
                   &optionValue, sizeof(optionValue)) < 0) {
        return SERVER_SOCKET_OPTION_ERROR;
    }
    printf("\n* Socket options defined.");


    // 3. Bind the socket to an address and port
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );

    if(bind(_serverDescriptors.mainSocketId,
             (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        return SERVER_SOCKET_BIND_ERROR;
    }
    printf("\n* Socket bind on the port: `%d`",port);


    // 4. Start listen (wait just one connection)
    if(listen(_serverDescriptors.mainSocketId,1) < 0) {
        return SERVER_SOCKET_LISTEN_ERROR;
    }
    printf("\nSocket listening...");


    // 5. Start accept connections
    printf("\n>>> Starting server...");
    _serverDescriptors.commSocketId = accept(_serverDescriptors.mainSocketId,
                                             (struct sockaddr *)&address,
                                            (socklen_t *)&addrlen);
    if(_serverDescriptors.commSocketId < 0) {
        return SERVER_SOCKET_ACCEPT_CONNECTION_ERROR;
    }
    close(_serverDescriptors.mainSocketId); // After accept a connection, stop listening (just one connection allowed)
    _serverDescriptors.mainSocketId = 0;
    return _serverDescriptors.commSocketId; // Accepting connection
}


void closeConnections() {
    if(close(_serverDescriptors.commSocketId) < 0) { // Close client connection (connected socket)
        fprintf(stderr,"\nIt was not possible close the client (socket) connection.");
    } else {
        printf("\n* Client connection closed.");
    }
    if(_serverDescriptors.mainSocketId) {
        if(close(_serverDescriptors.mainSocketId) < 0) { // Close server connection (listening socket - if more than 1 connection is allowed)
            fprintf(stderr,"\nIt was not possible close the server (socket) connection.");
        } else {
            printf("\n* Server connection closed.");
        }
    }
}


void chitChat(int socketId) {

    printf("\nAlice (client) started the chat with you (Bob)!\n");
    printf("Text `#` to exit from the chat.\n");

    char buffer[SOCKET_BUFFER_SIZE] = {0};


    int bytesReceived = 0;
    do {
        /// RECEIVE
        printf("\nWait for Alice...\n");
        bytesReceived = read(socketId, buffer, SOCKET_BUFFER_SIZE);
        buffer[bytesReceived] = '\0'; // Reuse the buffer (to send and receive) and avoid wrong end of string place.
        if( strcmp(buffer, END_OF_CHAT) == 0 ) {
            printf("\nAlice (client) ended the chat.");
            break;
        }
        printf("\n<< Alice (client): %s",buffer);

        //// SEND
        printf("\nBob (server) >> ");
        fgets(buffer,SOCKET_BUFFER_SIZE,stdin);
        buffer[ strlen(buffer) - 1 ] = '\0'; // Remove the \newline character.

        if( strcmp(buffer,END_OF_CHAT) == 0 ) {
            send(socketId,END_OF_CHAT,2,0); // Warn the client to end the conversation
            break;
        }
        send(socketId, buffer, strlen(buffer), 0);

    } while(1);
}
