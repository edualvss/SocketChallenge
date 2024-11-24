#ifndef TCP_SOCKET_CLIENT_H
#define TCP_SOCKET_CLIENT_H

#define BUFFER_SIZE 1024


const char* END_OF_CHAT;

struct sockaddr_in;

// Errors: negative values related with the string ERROR messages vector.
static const int CLIENT_SERVER_ADDRESS_OK = 1;
enum CLIENT_ERRORS {
    CLIENT_LACK_ARGUMENTS = -1,
    CLIENT_NO_SERVER_PORT = -2,
    CLIENT_INVALID_SERVER_PORT = -3,
    CLIENT_NO_SERVER_ADDRESS = -4,
    CLIENT_INVALID_SERVER_ADDRESS = -5,
    CLIENT_SERVER_SOCKET_CREATION_ERROR = -6,
    CLIENT_SERVER_SOCKET_CONNECTION_ERROR = -7
};
const char* CLIENT_ERROR_MESSAGES[];


int getCommandLinePort(int commandLineCount, char* commandLineArgs[]);
int getCommandLineAddress(int commandLineCount, char* commandLineArgs[],
                          struct sockaddr_in *serverAddress);



int establishConnection(struct sockaddr_in *serverAddress,int serverPort);

void chitChat(int socketId);



#endif // TCP_SOCKET_CLIENT_H
