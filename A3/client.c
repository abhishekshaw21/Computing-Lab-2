//Roll No.: 19CS60R48
//Name: Abhishek Shaw
//Description: TCP client
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
    printf("\n:::::: CLIENT ::::::\n\n");
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
    char serv_msg[512],serv_response[256];
    char *msg=serv_msg;
    size_t msgsize=256;
    //printf("\n Valid Commands\n/active​\n/send <dest client id> <Message>\n/broadcast <Message>\n/makegroup <client id1> <client id2> ... <client idn>\n/sendgroup <group id> <Message>\n/activegroups\n/makegroupreq <client id1> <client id2> ... <client idn>\n/joingroup <group id>\n/quit\n/activeallgroups\n/joingroup <group id>");
    int p=fork();
    if (p==0) {//write
            while (1) {
                bzero(msg, 256);
                bzero(serv_msg, 256);
                fgets(msg,msgsize, stdin);
                int num_bytes =0;
                for(;msg[num_bytes]!='\n';num_bytes++);
                msg[num_bytes] = '\0';
                send(sockfd,msg,strlen(msg),0);
                //n = write(sockfd,serv_msg,strlen(serv_msg));
                /*if (n < 0)
                    printf("ERROR writing to socket\n");
                else
                    printf("Write Request sent\n");*/
                if(!strcmp(serv_msg,"CLOSE\n")){
                    break;
                }
            }
    }
    else{//read
            while (1) {
                bzero(serv_response, 256);
                printf("\xE2\x9C\x93: ");
                recv(sockfd, serv_response, sizeof(serv_response), 0);
                //n = read(sockfd,serv_response,256);
                printf("%s\n",serv_response);
            }
    }
    close(sockfd);
    printf("\nDisconnected from the server\n");
    return 0;
}


