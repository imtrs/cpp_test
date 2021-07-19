#ifndef __TCP_ECHO_SERVER_H__
#define __TCP_ECHO_SERVER_H__

int CreateTCPServerSocket(unsigned short port); 
int AcceptTCPConnection(int servSock); 
void HandleTCPClient (int clntSocket); 

#endif//__TCP_ECHO_SERVER_H__