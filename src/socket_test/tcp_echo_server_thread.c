#include <tcp_echo_server.h>
#include <pthread.h> // for POSIX threads  

void * ThreadMain(void * arg); // Main program of a thread  

// Structure of arguments to pass to client thread 
struct ThreadArgs { 
    int clntSock; 
}; 
 
int main(int argc, char * argv[]) { 
    int servSock; 
    int clntSock; 
    unsigned short echoServPort; 
    pthread_t threadID; 

    struct ThreadArgs * threadArgs;

    if(argc != 2) { 
        fprintf(stderr, "Usage: %s <Server Port>", argv[0]); 
        exit(1);
    } 

    echoServPort = atoi(argv[1]); 
    servSock = CreateTCPServerSocket(echoServPort); 

    for(;;) { 
        clntSock = AcceptTCPConnection(servSock); 

        // Create memory for client argument 
        if((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)  {
            DieWithError("malloc() failed"); 
        }

        threadArgs->clntSock = clntSock; 

        // Create thread 
        if(pthread_create(&threadID, NULL, ThreadMain, 
            (void *) threadArgs) != 0) {
            DieWithError("pthread_create() failed"); 
        }

        printf("with thread %ld ", (long int) threadID); 
    } 
} 

void * ThreadMain(void * threadArgs) { 
    int clntSock; 

    // Guarantees that thread resource are deallocated upon return 
    pthread_detach(pthread_self()); 

    clntSock = ((struct ThreadArgs *) threadArgs)->clntSock; 
    free(threadArgs); // Deallocate memory for argument 

    HandleTCPClient(clntSock);  

    return (NULL); 
} 

