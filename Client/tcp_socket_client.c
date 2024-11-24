#include "tcp_socket_client.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <errno.h>

const char* CLIENT_ERROR_MESSAGES[] = {
    "OK",
    "You must specify the SERVER ADDRESS (command line argument -server <ip4_address>) **AND** the SERVER PORT (-port <number>) to establish a connection.",
    "You must specify the SERVER PORT number to attempt to connect. In the command line specify: `-port <number>`.",
    "Check the SERVER PORT number in the command line argument. The port must be a integer positive value.",
    "You must specify the SERVER ADDRESS number to attempt to connect. In the command line specify: `-server <ip4_address>`.",
    "Check the SERVER ADDRESS in the command line argument. The address must be in IPv4 compliant address (e.g. -server 127.0.0.1).",
    "Client socket creation failed.",
    "Client socket connection failed."
};

const char* END_OF_CHAT = "#";


int isPortValid(int portNumber) {
    return portNumber > 0;
}


int getCommandLineAddress(int commandLineCount, char* commandLineArgs[],
                          struct sockaddr_in *serverAddress) {

    if( commandLineCount < 4 ) {
        return CLIENT_LACK_ARGUMENTS;
    }

    // Search for '-server' argument.
    for(int i = 1; i < commandLineCount; i++ ) {
        if(strcmp(commandLineArgs[i],"-server") == 0) {
            if( i == commandLineCount-1 ) {
                fprintf(stderr,"There is no ADDRESS defined after -server argument.");
                return CLIENT_NO_SERVER_ADDRESS;
            } else {
                if(inet_pton(AF_INET, commandLineArgs[i+1],&(*serverAddress).sin_addr) <= 0) {
                    fprintf(stderr,"The command line argument `%s` after '-server' is an invalid IPv4 ADDRESS.",commandLineArgs[i+1]);
                    return CLIENT_INVALID_SERVER_ADDRESS;
                }
                return CLIENT_SERVER_ADDRESS_OK;
            }
        }
    }
    return CLIENT_NO_SERVER_ADDRESS;
}


int getCommandLinePort(int commandLineCount, char* commandLineArgs[]) {

    if( commandLineCount < 4 ) {
        return CLIENT_LACK_ARGUMENTS;
    }

    int portNumber = 0;

    // Search for '-port' argument.
    for(int i = 1; i < commandLineCount; i++ ) {
        if(strcmp(commandLineArgs[i],"-port") == 0) {
            if( i == commandLineCount-1 ) {
                fprintf(stderr,"There is no NUMBER defined after -port argument.");
                return CLIENT_NO_SERVER_PORT;
            } else {
                portNumber = atoi(commandLineArgs[i+1]);
                if( !isPortValid(portNumber) ) {
                    fprintf(stderr,"The command line argument `%s` after '-port' is an invalid PORT number.",commandLineArgs[i+1]);
                    return CLIENT_INVALID_SERVER_PORT;
                } else {
                    return portNumber;
                }
            }
        }
    }

    return CLIENT_NO_SERVER_PORT;
}


// Establish a TCP/IPv4 socket connection with the specified server address and port.
// Return the socket file descriptor of the connection.
int establishConnection(struct sockaddr_in *serverAddress, int serverPort) {

    serverAddress->sin_family = AF_INET;
    serverAddress->sin_port = htons(serverPort);

    // 1. Create the socket
    int socketId = 0;
    int numberConnectionAttempt = 0;

    do {
        sleep(1);
        if ((socketId = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            return CLIENT_SERVER_SOCKET_CREATION_ERROR;
        }

        // 2. Try to connect to the server
        if(connect(socketId, (struct sockaddr *)serverAddress, sizeof(struct sockaddr_in)) < 0) {
            fprintf(stderr,"\n[%d] Trying connect... Check the server - Problem: %s",++numberConnectionAttempt, strerror(errno));
            close(socketId);
            socketId = 0;
        }
    } while( socketId == 0 );

    return socketId;
}


void chitChat(int socketId) {

    char buffer[BUFFER_SIZE] = {0};

    printf("\nStart the CHAT.\n You will be called `Alice` (client) - Text your the first message to Bob (server)!\n");
    printf("Text `#` to exit from the chat.\n");

    int bytesReceived = 0;

    do {
        //// SEND
        printf("\nAlice (client) >> ");
        fgets(buffer,BUFFER_SIZE,stdin);
        buffer[ strlen(buffer) - 1 ] = '\0'; // Remove the \newline from the string

        if( strcmp(buffer,END_OF_CHAT) == 0 ) {
            send(socketId,END_OF_CHAT,2,0); // Warn the server to end the conversation
            break;
        }
        send(socketId, buffer, strlen(buffer), 0);

        /// RECEIVE
        printf("\nWait for Bob...\n");
        bytesReceived = read(socketId, buffer, BUFFER_SIZE);
        buffer[bytesReceived] = '\0';   // Read just the amount of bytes received (for buffer reuse removing the \newline marker).
        if( strcmp(buffer, END_OF_CHAT) == 0 ) {
            printf("\nBob ended the chat.");
            break;
        }
        printf("\n<< Bob (server): %s",buffer);

    } while(1);
}
