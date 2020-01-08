//Roll No.: 19CS60R48
//Name: Abhishek Shaw
//Description: TCP client in which client connects to server sending some arbitrary message and server acknowledges it.
#include <stdlib.h>
#include<stdio.h>
#include<netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#define portN 2110
int main(){
    int sockfd,n;
    //Socket
    sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct hostent *server;
    server = gethostbyname("localhost");
    if (server == NULL) {
        printf("ERROR, no such host\n");
        exit(0);
    }
    //Connect
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portN);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
        printf("Error connecting to the server\n");
        exit(0);
    }
    else{
        printf("Successfully connected to server\n");
    }
    int choice,line_no;
    char msg2[256],serv_msg[512],serv_response[256];
    char *msg=msg2;
    size_t msgsize=255;
    while (1) {
        bzero(serv_msg,512);
        bzero(msg, 256);
        bzero(serv_response, 256);
        printf("Enter the choice\n1.To Read a line\n2.To Write a line\n3.To exit\n");
        scanf("%d",&choice);
        fflush(stdin);
        switch (choice) {
            case 1:
                printf("Enter the line number to read: ");
                scanf("%d",&line_no);
                sprintf(serv_msg,"READX %d",line_no);
                n = write(sockfd,serv_msg,strlen(serv_msg));
                printf("Read Request sent\n");
                if (n < 0)
                    printf("ERROR writing to socket\n");
                n = read(sockfd,serv_response,255);
                printf("The Message from Server:%s\n",serv_response);
                break;
            case 2:
                printf("Enter the message to be appended ");
                fflush(stdin);
                scanf("\n");
                fgets(msg,msgsize, stdin);
                //scanf("%s",msg);
                fflush(stdin);
                sprintf(serv_msg, "WRITEX %s",msg);
                n = write(sockfd,serv_msg,strlen(serv_msg));
                printf("Write Request sent");
                if (n < 0)
                     printf("ERROR writing to socket\n");
                n = read(sockfd,serv_response,255);
                printf("The Message from Server:%s\n",serv_response);
                break;
            case 3:
                sprintf(serv_msg, "CLOSE");
                n = write(sockfd,serv_msg,strlen(serv_msg));
                close(sockfd);
                printf("Disconnected from the server\n");
                exit(0);
            default:
                printf("Wrong choice please enter properly\n");
                break;
        }
    }
    close(sockfd);
    printf("Disconnected from the server\n");
    return 0;
}
