#include "tcp_socket_server.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* SERVER_ERRORS_MESSAGES[] = {
    "",
    "You must specify a PORT number to the start the server. In the command line specify: `-port <number>`.",
    "Check your PORT number command line argument. The port must be a integer positive value."
};

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
