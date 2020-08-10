#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>  // provides read, write function
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>  // defines host structure

#define sz 255

int main(int argc, char* argv[]){
    if(argc<=2){
        printf("NO port/IpAddress provided");
        exit(1);
    }

    struct sockaddr_in server_addr, client_addr;
    struct hostent *server;
    char str[sz];

    int port_no = atoi(argv[2]);
    int sockfd = socket(AF_INET , SOCK_STREAM , 0);
    
    if(sockfd < 0){
        printf("Socket connection wasn't generated");
        exit(1);
    }

    server=gethostbyname(argv[1]);

    if(!server){
        printf("Error opening server");
        exit(1);
    }

    bzero((char*) &server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char*) server->h_addr,(char*) &server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(port_no);

    if(connect(sockfd , (struct sockaddr *) &server_addr , sizeof(server_addr)) <0){
        printf("Error connecting");
        exit(1);
    }

    while(1){
        bzero(str,sizeof(str));
        fgets(str,sizeof(str),stdin);
        int n = write(sockfd , str, sizeof(str));
        if(n < 0){
            printf("Error while writing");
            exit(1);
        }

        bzero(str , sizeof(str));
        n = read(sockfd , str, sizeof(str));

        if(n < 0){
            printf("Error while reading");
            exit(1);
        }
        printf("Server : %s\n" , str);
    }
}

