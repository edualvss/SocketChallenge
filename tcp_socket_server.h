#ifndef TCP_SOCKET_SERVER_H
#define TCP_SOCKET_SERVER_H

enum SERVER_ERRORS {
    SERVER_NO_PORT = -1,
    SERVER_INVALID_PORT = -2
};

const char* SERVER_ERRORS_MESSAGES[];


// Command line setup
int isPortValid(int portNumber);
int getCommandLinePort(int commandLineCount, char* commandLineArgs[]);


#endif // TCP_SOCKET_SERVER_H
