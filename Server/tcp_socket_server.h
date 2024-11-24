#ifndef TCP_SOCKET_SERVER_H
#define TCP_SOCKET_SERVER_H

#define SOCKET_BUFFER_SIZE 1024

const char* END_OF_CHAT;

struct ServerSocketDescriptors {
    int mainSocketId;
    int commSocketId;
} _serverDescriptors;


// Errors: negative values related with the string ERROR messages vector.
enum SERVER_ERRORS {
    SERVER_NO_PORT = -1,
    SERVER_INVALID_PORT = -2,
    SERVER_SOCKET_CREATION_ERROR = -3,
    SERVER_SOCKET_OPTION_ERROR = -4,
    SERVER_SOCKET_BIND_ERROR = -5,
    SERVER_SOCKET_LISTEN_ERROR = -6,
    SERVER_SOCKET_ACCEPT_CONNECTION_ERROR = -7
};
const char* SERVER_ERROR_MESSAGES[];


// Command line setup
int isPortValid(int portNumber);
int getCommandLinePort(int commandLineCount, char* commandLineArgs[]);


// Server
int acceptConnections(int port);
void closeConnections();

void chitChat(int socketId);

#endif // TCP_SOCKET_SERVER_H
