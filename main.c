#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcp_socket_server.h"

int main(int argc, char* argv[]) {


    int serverPort = getCommandLinePort(argc,argv);
    if(serverPort < 0) {
        fprintf(stderr,"\nError on the server startup: %s",SERVER_ERRORS_MESSAGES[-serverPort]);
        printf("\nExiting...");
        return serverPort;
    }

    printf("\n>>> Server PORT: %d",serverPort);




    return 0;
}
