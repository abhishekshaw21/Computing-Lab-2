//Roll No.: 19CS60R48
//Name: Abhishek Shaw
//Description: TCP server in which server listens on some port, client connects to server sending some arbitrary message and server acknowledges it.
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#define portN 2110
int validate(char *buffer){
    char *r="READX";
    char *w="WRITEX";
    if(strstr(buffer,r)||strstr(buffer,w)){
        return 1;
    }
    return 0;
}
int tokenize(char *buffer, char *movie, int flag)
{
    int line = 0, i, j = 0;
    if(flag == 1)  //Read request, extract the line number
    {
        i = 6;
        while(buffer[i])
            line = line * 10 + (buffer[i++] - '0');
        return line;
    }
    else if(flag == 2) //write request, extract the movie name
    {
        i = 7;
        while(buffer[i])
            movie[j++] = buffer[i++];
    }
    movie[j] = '\0';
    return -1;
}
int main(){
    struct sockaddr_in serv_addr;
    char *fileName="input.txt";
    if( !(access(fileName, F_OK ) != -1 )) {
        printf("File does not exist.\nPlease add %s. \nServer cannot start\n",fileName);
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; //used for IPv4
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portN);
    //socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //bind
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ){
        printf("ERROR on binding\n");
        exit(0);
    }
    //listen
    listen(sockfd,5);
    struct sockaddr_in cli_addr; //storing client address
    socklen_t clilen; //storing length for client address, i.e. 32 bit integer
    clilen = sizeof(cli_addr);
    printf("The Server is listening at port %d\n",portN);
    //accept
    int newsockfd;
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr,&clilen);
    printf("A client is connected to the server\n");
    char buffer[256],movie[256],moviecmp[256],mbuffer[256],mbuffercmp[256];
    FILE *filePointer;
    char c;
    int count,line,current_line,len,n;
    char *next="\n";
    do{
        printf("Waiting for Repsonse from Client\n");
        bzero((char *) &buffer, sizeof(buffer));
        count=0;
        n = read(newsockfd,buffer,255); //reads information from socket to local buffer
        printf("Request from Client: %s\n", buffer);
        if(!strcmp("CLOSE", buffer)){
            break;
        }
        if(!validate(buffer)){
            printf("Invalid Input");
            write(newsockfd, "Invalid Input", 16);
            continue;
        }
        filePointer=fopen(fileName, "r");
        for (c = getc(filePointer); c != EOF; c = getc(filePointer))
            if (c == '\n')
                count = count + 1;
        count = count + 1; //Last line of the file
        fclose(filePointer);
        if(buffer[0]=='R'){//Read
            current_line=1;
            line = tokenize(buffer, movie, 1);
            if(line>count){
                n = write(newsockfd,"Requested Line number exceeds total number of line",50);
                continue;
            }
            fflush(stdout);
            filePointer=fopen(fileName, "r");
            for (c=getc(filePointer); current_line<line; c=getc(filePointer))
                if(c=='\n')
                    current_line=current_line+1;
            fseek(filePointer, -1, SEEK_CUR);
            //printf("HELL %d\n",current_line);
            if (current_line==line) {
                fgets(movie, 250, filePointer);
                n = write(newsockfd,movie,strlen(movie));
            }
            fclose(filePointer);
        }
        else if(buffer[0]=='W'){//write
            tokenize(buffer, movie, 2);
            len = strlen(movie);
            strcpy(moviecmp, movie);
            for(int i = 0; i < len ; i++)
                moviecmp[i] = toupper(moviecmp[i]);
            filePointer = fopen("input.txt", "r");
            fgets(mbuffer, 255, filePointer);
            len = strlen(mbuffer);
            mbuffer[len - 1] = '\0';
            strcpy(mbuffercmp, mbuffer);
            len = strlen(mbuffer);
            for(int i = 0; i < len ; i++)
                mbuffercmp[i] = toupper(mbuffercmp[i]);
            current_line = 2;
            while(strcmp(moviecmp, mbuffercmp) && current_line <= count)
            {
                fgets(mbuffer, 255, filePointer);
                len = strlen(mbuffer);
                mbuffer[len - 1] = '\0';
                strcpy(mbuffercmp, mbuffer);
                len = strlen(mbuffer);
                for(int i = 0; i < len ; i++)
                    mbuffercmp[i] = toupper(mbuffercmp[i]);
                current_line++;
                
            }
            if(current_line > count)
            {
                fclose(filePointer);
                filePointer = fopen(fileName, "r+");
                fseek(filePointer, 0, SEEK_END);
                fputs(movie, filePointer);
                fclose(filePointer);
                write(newsockfd, "Success", 8);
            }
            else
                write(newsockfd, "Movie name exist", 16);
        }
    } while(1);
    close(newsockfd);
    close(sockfd);
    printf("Server Closed\n");
    return 0;
}
