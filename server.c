#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>  // provides read, write function
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define sz 255

int main(int argc, char* argv[]){
    if(argc<=1){
        printf("NO port provided");
        exit(1);
    }
    int port_no = atoi(argv[1]);

    char str[sz];
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if(sockfd < 0){
        printf("Socket connection wasn't generated");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    bzero((char *) &server_addr , sizeof(server_addr));
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    int b = bind(sockfd , (struct sockaddr *) &server_addr , sizeof(server_addr) );
    if(b < 0){
        printf("BInding  wasn't successful");
        exit(1);
    }
    listen(sockfd , 8);

    socklen_t clilen = sizeof(client_addr);

    int newSockfd = accept(sockfd, (struct sockaddr *) &client_addr, &clilen );

    if(newSockfd < 0){
        printf("Error while accepting connection");
        exit(1);
    }

    while(1){
        bzero(str,sizeof(str));
        int n = read(newSockfd , str, sizeof(str));
        if(n < 0){
            printf("Error while reading from connection");
            exit(1);
        }

        printf("Client : %s\n" , str);
        bzero(str , sizeof(str));
        fgets(str , sizeof(str) , stdin);
        n = write(newSockfd , str, sizeof(str));

        if(n < 0){
            printf("Error while writing from connection");
            exit(1);
        }
    }
}