#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <netinet/ip.h> /* superset of previous */
#include <tcp_echo_server.h>

static const int RCVBUFSIZE = 128;
static const int MAXPENDING = 5;

int CreateTCPServerSocket(unsigned short port) { 
    int sock; 
    struct sockaddr_in echoServAddr; 

    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { 
        printf("socket() failed"); 
        exit(1); 
    } 

    memset(&echoServAddr, 0, sizeof(echoServAddr)); 
    echoServAddr.sin_family = AF_INET; 
    echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    echoServAddr.sin_port = htons(port); 

    if(bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) { 
        printf("bind() failed"); 
        exit(1); 
    } 

    if(listen(sock, MAXPENDING) < 0) { 
        printf("listen() failed"); 
        exit(1); 
    } 

    return sock; 
} 

void HandleTCPClient (int clntSocket) { 
    char echoBuffer[RCVBUFSIZE]; 
    int recvMsgSize; 

    if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) { 
        printf("recv() failed"); 
        exit(1); 
    } 

    while(recvMsgSize > 0) { 
        if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize) { 
            printf("send() failed"); 
            exit(1); 
        } 

        if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0) { 
            printf("recv() failed"); 
            exit(1); 
        } 
    } 

    close(clntSocket); 
} 

int AcceptTCPConnection(int servSock) { 
    int clntSock; 
    struct sockaddr_in echoClntAddr; 
    unsigned int clntLen; 

    clntLen = sizeof(echoClntAddr); 

    if((clntSock = accept(servSock, (struct sockaddr *) &echoClntAddr, &clntLen)) < 0) { 
        printf("accept() failed"); 
        exit(1); 
    } 

    printf("Handling client %s ", inet_ntoa(echoClntAddr.sin_addr)); 

    return clntSock; 
} 