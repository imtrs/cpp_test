#include <stdio.h> 
#include <sys/types.h> 
#include <sys/socket.h> // socket() bind() connect() 
#include <arpa/inet.h>  // socketaddr_in,  inet_ntoa() 
//#include <netinet/in.h> // 만약 FreeBSD 라면 이 해더를 추가해야한다. 
#include <string.h> 
#include <unistd.h> 
#include <stdlib.h> 

#define RCVBUFSIZE 128 

int main(int argc, char * argv[]){ 
    int sock; 
    struct sockaddr_in echoServAddr; 
    unsigned short echoServPort; 
    char * servIP; 
    char * echoString; 
    char echoBuffer[RCVBUFSIZE]; 
    unsigned int echoStringLen; 
    int bytesRcvd, totalBytesRcvd; 

    if((argc < 3) || (argc  > 4)) { 
        printf("Usage: %s <Server IP> <Echo Word> [<Echo Port>]", argv[0]); 
        exit(1); 
    } 

    servIP = argv[1]; 
    echoString = argv[2]; 

    if(argc == 4) {
        echoServPort = atoi(argv[3]); 
    }
    else { 
        echoServPort = 7; // 에코 서버의 well-know port입니다.. 
    }
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) { 
        printf("socket() failed"); 
        exit(1); 
    } 

    memset(&echoServAddr, 0, sizeof(echoServAddr)); 
    echoServAddr.sin_family = AF_INET; 
    echoServAddr.sin_addr.s_addr = inet_addr(servIP); 
    echoServAddr.sin_port = htons(echoServPort); 

    if(connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) { 
        printf("connect() failed"); 
        exit(1); 
    } 

    echoStringLen = strlen(echoString); 

    if(send(sock, echoString, echoStringLen, 0) != echoStringLen) { 
        printf("send() failed"); 
        exit(1); 
    } 

    totalBytesRcvd = 0; 
    printf("Received: "); 

    while(totalBytesRcvd < echoStringLen) { 
        // 문자끝 NULL을 넣기 위해 RCVBUFSIZE-1 
        if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE-1, 0)) <= 0) { 
            // 리턴값이 0이면 서버와 연결이 끊어짐.. 
            printf("recv() 실패 혹은 서버와 연결이 끊어졌다."); 
            exit(1); 
        } 

        totalBytesRcvd += bytesRcvd; 
        echoBuffer[bytesRcvd] = '\0'; 
        printf(echoBuffer); 

    } 

    printf(" "); 

    close(sock); 
    exit(0); 
} 