//Roll No.: 19CS60R48
//Name: Abhishek Shaw
//Description: TCP server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define max_clients 10
#define max_group 4
#define portN 2110
#define max_group_size 10
struct clientInfo{
    int sockfd;
    char id[6];//5 Digit Random Number
    char online;
};
struct groupInfo{
    char id[6];
    struct clientInfo members[max_clients];
    char adminId[6];
    int number_of_members;
};
struct requestMapping{
    char gId[6];
    char cId[6];
    char request;
    //P-pending
};
int getClientidbysocketid(struct clientInfo *cInfo,char *clientId,int fd){
    int i;
    for (i=0; i<max_clients; i++) {
        if (cInfo[i].sockfd==fd){
            strcpy(clientId, cInfo[i].id);
            return i;
        }
    }
    clientId="";
    return -1;
}
int getIdbyClientId(struct clientInfo *cInfo,char *clientId){
    int i;
    for (i=0; i<max_clients; i++) {
        if (strcmp(cInfo[i].id, clientId)==0) {
            return i;
        }
    }
    return -1;
}
int validateClientid(struct clientInfo *cInfo,char *clientId){
    int i;
    for (i=0; i<max_clients; i++) {
        if (strcmp(cInfo[i].id,clientId)==0 && cInfo[i].online=='Y'){
            return i;
        }
    }
    return -1;
}
int validateGroupid(struct groupInfo *gInfo,char *gId){
    int i;
    for (i=0; i<max_group; i++) {
        if (strcmp(gInfo[i].id, gId)==0) {
            return i;
        }
    }
    return -1;
}
void activeallgroups(struct groupInfo *gInfo,char *members){
    int i;
    for (i=0; i<max_clients; i++) {
        if (gInfo[i].number_of_members!=0) {
            strcat(members,gInfo[i].id);
            strcat(members, " ");
        }
    }
}
void activemembers(struct clientInfo *cInfo,char *members){
    int i;
    for (i=0; i<max_clients; i++) {
        if (cInfo[i].online=='Y') {
            strcat(members, cInfo[i].id);
            strcat(members, " ");
        }
    }
    return ;
}
int isMember(struct groupInfo *gInfo,int clientFd,char *gId){
	int i1,i2;
	for (i1=0; i1<max_group; i1++)
        if (strcmp(gInfo[i1].id, gId)==0) {
            for(i2=0;i2<gInfo[i1].number_of_members;i2++){
            	if(gInfo[i1].members[i2].sockfd==clientFd)
            		return 1;
            }
            return 0;
        }
    return 0;
}
int main(int argc, const char * argv[]) {
      printf("\n:::::: SERVER ::::::\n");
      int sockfd,clilen,addrlen , new_socket,activeConnection=0;
      struct clientInfo client_socket[max_clients];
      struct groupInfo gInfo[max_group];
      struct requestMapping requestInfo[max_clients*2];
      int groupCount=0,groupRequestCount=0;
      int activity, i , valread , sd;
      int max_sd;
      struct sockaddr_in serv_addr;
      char buffer[256],op1[10],op2[20];
      int yes=-1;
      sockfd = socket(AF_INET, SOCK_STREAM, 0);
      if (sockfd < 0)
           printf("ERROR opening socket");
      if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int))==-1){
          perror("setsockopt error");
          exit(1);
      }
      bzero((char *) &serv_addr, sizeof(serv_addr));
      //specify an address for the socket
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port=htons(portN);
      serv_addr.sin_addr.s_addr=INADDR_ANY;
      /*binds a socket to an address, in this case the address of the current
      host and port number on which the server will run*/
      if (bind(sockfd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0)
                    printf("ERROR on binding");

      /*The listen system call allows the process to listen on the socket for
      connections.backlog defines the maximum length to which the pending connection
      may grow max 20 safe side 5 to 10*/

      listen(sockfd,5);
      fd_set readfds;
      //initialise all client_socket[] to 0 so not checked
      for (i = 0; i < max_clients; i++)
      {
          client_socket[i].sockfd = 0;
          client_socket[i].online='N';
      }
      //accept the incoming connection
      addrlen = sizeof(serv_addr);
      printf("Server is listening at port %d \n",portN);
     while(1){
          FD_ZERO(&readfds);
          FD_SET(sockfd,&readfds);
          max_sd = sockfd;
          clilen = sizeof(serv_addr);
          //add child sockets to set
          for ( i = 0 ; i < max_clients ; i++)
          {
              //socket descriptor
              sd = client_socket[i].sockfd;
              //if valid socket descriptor then add to read list
              if(sd > 0)
                  FD_SET( sd , &readfds);
              //highest file descriptor number, need it for the select function
              if(sd > max_sd)
                  max_sd = sd;
          }
       
          //wait for an activity on one of the sockets , timeout is NULL ,
          //so wait indefinitely
          activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
         
          if ((activity < 0))// && (errno!=EINTR))
          {
              printf("select error");
          }
               
          //If something happened on the master socket ,
          //then its an incoming connection
          if (FD_ISSET(sockfd, &readfds))
          {
              if(activeConnection>5){
              		printf("No. of maximum connection excceeded");
              		break;
              }
              if ((new_socket = accept(sockfd,
                      (struct sockaddr *)&serv_addr, (socklen_t*)&addrlen))<0)
              {
                  perror("accept");
                  exit(EXIT_FAILURE);
              }
              //add new socket to array of sockets
              for (i = 0; i < max_clients; i++)
              {
                  //if position is empty
                  if( client_socket[i].sockfd == 0 )
                  {
                      client_socket[i].sockfd = new_socket;
                      client_socket[i].online='Y';
                      strcpy(client_socket[i].id,"C0000");
                      int temp=i+1;
                      int k=4;
                      while (temp>0) {
                          client_socket[i].id[k]=(temp%10)+'0';
                          temp=temp/10;
                          k--;
                      }
                      char *message=(char*)malloc(256);
                      //inform user of socket number - used in send and receive commands
                      printf("Connected with client socket id: %s \n" , client_socket[i].id);
                      sprintf(message,"Your Client Id is %s\n",client_socket[i].id);
            	      send(client_socket[i].sockfd, message, strlen(message), 0);
                      break;
                  }
              }
          }
               
          //else its some IO operation on some other socket
          for (i = 0; i < max_clients; i++)
          {
              sd = client_socket[i].sockfd;
              if (FD_ISSET( sd , &readfds))
              {
                  //Check if it was for closing , and also read the
                  //incoming message
                  bzero(buffer, 256);
                  bzero(op1, 10);
                  bzero(op2, 20);
                  printf("\n---------------------------------------------------\n");
                  valread = read( sd , buffer, 256);
                  strncpy(op1, buffer, 10);
                  strncpy(op2, buffer, 20);
                  printf("Message from Client %s: ",client_socket[i].id);
                  fputs(buffer, stdout);
                  printf("\n");
                  char *msg=buffer;
                  if (valread== 0)
                  {
                      //Somebody disconnected , get his details and print
                     printf("Disconnected Client: %s:",client_socket[i].id);
                      for (i=0; i<max_clients; i++) {
                          if(sd==client_socket[i].sockfd){
                              client_socket[i].sockfd = 0;
                              client_socket[i].online='N';
                          }
                      }
                      close(sd);
                  }
                  if (strstr(op2, "/activeallgroups")) {
                      int g1;
                      char *members=(char*)malloc(256);
                      for (g1=0; g1<groupCount; g1++) {
                          if (gInfo[g1].number_of_members>0) {
                                  strcat(members, gInfo[g1].id);
                                  strcat(members, " ");
                          }
                      }
                      send(client_socket[i].sockfd, members, strlen(members), 0);
                      //free(members);
                      continue;
                  }
                  if (strstr(op2,"/activegroups")) {
                      int g1,m1;
                      char *members=(char*)malloc(256);
                      for (g1=0; g1<groupCount; g1++) {
                          if (gInfo[g1].number_of_members>0) {
                              if(strcmp(gInfo[g1].adminId,client_socket[i].id)==0){
                                  strcat(members, gInfo[g1].id);
                                  strcat(members, " ");
                              }
                              else{
                          for (m1=0; m1<gInfo[g1].number_of_members; m1++) {
                              if(gInfo[g1].members[m1].sockfd==client_socket[i].sockfd){
                                  strcat(members, gInfo[g1].id);
                                  strcat(members, " ");
                              }
                          }
                              }
                        }
                      }
                      send(client_socket[i].sockfd, members, strlen(members), 0);
                      //free(members);
                      continue;
                  }
                  if (strstr(op1,"/active" )) {
                      char *members=(char *)malloc(512);
                      activemembers(client_socket, members);
                      send(sd,members,strlen(members),0);
                      //free(members);
                      continue;
                  }
                  if (strstr(op2,"/sendgroup" )) {
                      char *message=(char*)malloc(256);
                      char *groupid=(char*)malloc(6);
                      char *senderName=(char *)malloc(512);
                      bzero(message, 512);
                      bzero(senderName, 512);
                      bzero(groupid, 6);
                      strncpy(groupid,msg+11,5); //group_id
                      strncpy(message,msg+16,strlen(msg)-16);//message
                      sprintf(senderName, "Group %s -> Client %s: %s",groupid,client_socket[i].id,message);
                      if(!isMember(gInfo,client_socket[i].sockfd,groupid)){
                      	    send(client_socket[i].sockfd, "You are not a member of this group!", 50, 0);
                      	    continue;
                      }
                      int g1,m1;
                      for (g1=0; g1<groupCount; g1++) {
                          if (strcmp(gInfo[g1].id,groupid)==0) {
                              for (m1=0; m1<gInfo[g1].number_of_members; m1++) {
                                  send(gInfo[g1].members[m1].sockfd, senderName, strlen(senderName), 0);
                              }
                              break;
                          }
                      }
                      if(g1==groupCount){
                          send(client_socket[i].sockfd, "Invalid Group Id", 20, 0);
                      }
                      //free(message);
                      //free(groupid);
                      continue;
                  }
                  if (strstr(op1,"/send")) {
                      char *message=(char*)malloc(256);
                      char *clientid=(char*)malloc(6);
                      char *senderName=(char *)malloc(512);
                      bzero(message, 512);
                      bzero(senderName, 512);
                      bzero(clientid, 6);
                      strncpy(clientid,msg+6,5); //client_id
                      strncpy(message,msg+12,strlen(msg)-12);//message
                      //printf("Id->%s Message->%s\n",clientid,message);
                      int cd=validateClientid(client_socket, clientid);
                      if (cd==-1) {
                          send(client_socket[i].sockfd, "Invalid Client Id", 20,0);
                      }
                      else{
                          sprintf(senderName, "Message from %s: %s",client_socket[i].id,message);
                          //senderName=strcat(senderName, message);
                          int s=send(client_socket[cd].sockfd, senderName, strlen(senderName), 0);
                          if (s<0) {
                              send(client_socket[i].sockfd, "Could not send message to client", 30, 0);
                          }
                      }
                      //free(senderName);
                      //free(message);
                      //free(clientid);
                      continue;
                  }
                  if (strstr(op2,"/broadcast" )) {
                      char *message=(char*)malloc(512);
                      char *senderName=(char *)malloc(512);
                      bzero(message, 512);
                      bzero(senderName, 512);
                      strncpy(message, msg+11, strlen(msg)-11);
                      sprintf(senderName, "Message from %s: %s",client_socket[i].id,message);
                      int k;
                      for (k=0; k<max_clients; k++) {
                          if (client_socket[k].online=='Y') {
                              send(client_socket[k].sockfd, senderName, strlen(senderName), 0);
                          }
                      }
                      //free(senderName);
                      //free(message);
                      continue;
                  }
                  if (strstr(op2,"/makegroupreq" )) {
                      int i1, membercount=(strlen(msg)-12)/6;
                      char *message=(char *)malloc(512);
                      int *members=(int *)malloc(max_clients*sizeof(int));
                      memset(members,0,max_clients*sizeof(int));
                      char gMembers[membercount][6];
                      for (i1=0; i1<membercount; i1++) {
                          strncpy(gMembers[i1], msg+14+6*i1,5);
                          gMembers[i1][5]='\0';
                      }
                      if (membercount==0) {
                          send(client_socket[i].sockfd, "No Client Id, Group cannot be formed", 40,0);
                          break;
                      }
                      for (i1=0; i1<membercount; i1++) {
                      	  int temp;
                      	  temp=validateClientid(client_socket, gMembers[i1]);
                          if(temp==-1){
                              sprintf(message, "Client %s is invalid\n",gMembers[i1]);
                              send(client_socket[i].sockfd,message,strlen(message),0);
                              //free(message);
                              break;
                          }
                          members[temp]=1;
                      }
                      if (i1!=membercount) {
                          continue;
                      }
                      members[i]=1;
                      strcpy(gInfo[groupCount].adminId,client_socket[i].id);
                         strcpy(gInfo[groupCount].id,"G0000");
                         int temp=groupCount+1;
                         int k=4;
                         while (temp>0) {
                           gInfo[groupCount].id[k]=(temp%10)+'0';
                           temp=temp/10;
                           k--;
                         }
                      int x=0;
                      for (i1=0; i1<max_clients; i1++) {
                          if(members[i1]!=0&&i1!=i){
		              strcpy(requestInfo[groupRequestCount].gId,gInfo[groupCount].id);
		              strcpy(requestInfo[groupRequestCount].cId,client_socket[i1].id);
		              requestInfo[groupRequestCount].request='P';
		              groupRequestCount++;
		              x++;
                          }
                      }
                      if(x==0){
                      	 send(client_socket[i].sockfd, "Need atleast 2 member to form a group", 40,0);
                       	     continue;
                      }
                      for (i1=0; i1<max_clients; i1++) {
                          if(members[i1]!=0 && i1!=i){
                          	sprintf(message,"%s requested you to join %s",client_socket[i].id,gInfo[groupCount].id);
				send(client_socket[i1].sockfd,message,strlen(message),0);
                          }
                      }
                      gInfo[groupCount].number_of_members=1;
                      gInfo[groupCount].members[0].sockfd=client_socket[i].sockfd;
                      strcpy(gInfo[groupCount].members[0].id,client_socket[i].id);
                      sprintf(message, "Group created successfully %s \n",gInfo[groupCount].id);
                      send(client_socket[i].sockfd,message,strlen(message),0);
                      groupCount++;
                      //free(message);
                      continue;
                  }
                  if (strstr(op1,"/makegroup" )) {
                      int i1,j1,membercount=(strlen(msg)-9)/6;
                      int *members=(int *)malloc(max_clients*sizeof(int));
                      memset(members,0,max_clients*sizeof(int));
                      char *message=(char *)malloc(512);
                      char gMembers[membercount][6];
                      for (i1=0; i1<membercount; i1++) {
                          strncpy(gMembers[i1], msg+11+6*i1,5);
                          gMembers[i1][5]='\0';
                      }
                      if (membercount==0) {
                          send(client_socket[i].sockfd, "No Client Id given, Group cannot be formed", 40,0);
                          continue;
                      }
                      for (i1=0; i1<membercount; i1++) {
                      	  int temp;
                      	  temp=validateClientid(client_socket, gMembers[i1]);
                          if(temp==-1){
                              sprintf(message, "Client %s is invalid\n",gMembers[i1]);
                              send(client_socket[i].sockfd,message,strlen(message),0);
                              //free(message);
                              break;
                          }
                          members[temp]=1;
                      }
                      if(i1!=membercount){
                      	continue;
                      }
                      members[i]=1;
		      strcpy(gInfo[groupCount].adminId,client_socket[i].id);
		      strcpy(gInfo[groupCount].id,"G0000");
		      int temp=groupCount+1;
		      int k=4;
		      while (temp>0) {
		         gInfo[groupCount].id[k]=(temp%10)+'0';
		         temp=temp/10;
		          k--;
		      }
                      membercount=0;
                      sprintf(message,"You have been added to %s by %s",gInfo[groupCount].id,client_socket[i].id);
                      for (i1=0; i1<max_clients; i1++) {
                       	 if(members[i1]!=0){
		            strcpy(gInfo[groupCount].members[membercount].id,client_socket[i1].id);
		            gInfo[groupCount].members[membercount].sockfd = client_socket[i1].sockfd;
		            if(i1!=i)
		            send(client_socket[i1].sockfd,message,strlen(message),0);
		            membercount++;
		          }
                       }
                       printf("\n");
                       if(membercount==1){
                       	     send(client_socket[i].sockfd, "Need atleast 2 member to form a group", 40,0);
                       	     continue;
                       }
                       gInfo[groupCount].number_of_members=membercount;
                       bzero(message,512);
                       sprintf(message, "Group created successfully %s \n",gInfo[groupCount].id);
                       send(client_socket[i].sockfd,message,strlen(message),0);
                       groupCount++;
                      continue;
                  }
                  if (strstr(op2,"/joingroup" )) {
                      int g1,g2,flag=0;
                      char *groupId=(char *)malloc(512);
                      char *message=(char *)malloc(512);
                      strncpy(groupId, msg+11, 5);
                      //printf("groupId->%s\n",groupId);
                      groupId[5]='\0';
                      for (g1=0; g1<groupRequestCount; g1++) {
                          //find the geoup
                          int memNum;
                          if(strcmp(groupId, requestInfo[g1].gId)==0 && strcmp(client_socket[i].id, requestInfo[g1].cId)==0){
                              for (g2=0; g2<groupCount; g2++) {
                                  if (strcmp(gInfo[g2].id,requestInfo[g1].gId)==0) {
                                      if(requestInfo[g2].request=='P'){
		                              memNum=gInfo[g2].number_of_members;
		                              gInfo[g2].members[memNum].sockfd = client_socket[i].sockfd;
		                              strcpy(gInfo[g2].members[memNum].id, client_socket[i].id);
		                              requestInfo[g2].request='A';
		                              gInfo[g2].number_of_members++;
		                              send(client_socket[i].sockfd,"You have been added to the group",50,0);
		                              sprintf(message, "Client %s joined group %s \n",
		                              requestInfo[g1].cId,requestInfo[g1].gId);
		                              send(client_socket[getIdbyClientId 
		                              (client_socket,gInfo[g2].adminId)].sockfd, message, strlen(message), 0);
                                              flag=1;
		                              break;
                                      }
                                  }
                              }
                              continue;
                          }
                      }
                      if (flag==0) {
                          send(client_socket[i].sockfd, "No Request found", 40, 0);
                      }
                      continue;
                  }
                  if (strstr(op2, "/declinegroup")) {
                      int g1,flag=0;
                      char *groupId=(char *)malloc(512);
                      strncpy(groupId, msg+14, 5);
                      groupId[5]='\0';
                      for (g1=0; g1<groupRequestCount; g1++) {
                          //find the group
                          if(strcmp(groupId, requestInfo[g1].gId)==0 && strcmp(client_socket[i].id, requestInfo[g1].cId)==0){
                              if(requestInfo[g1].request=='P'){
                              requestInfo[g1].request='R';
                              send(client_socket[i].sockfd, "You have rejected the request", 30, 0);
                              flag=1;
                              break;
                              }
                          }
                      }
                      if (flag==0) {
                          send(client_socket[i].sockfd, "No Request found", 20, 0);
                      }
                      //free(groupId);
                      continue;
                  }
                  if (strstr(op1,"/quit")||strstr(op1,"Ctrl")) {
                      int i1,g1;
                      char *message=(char*)malloc(512);
                      sprintf(message, "Client %s is going offline\n",client_socket[i].id);
                      for (i1=0;i1<max_clients ; i1++) {
                          if(client_socket[i1].online=='Y' && i1!=i){
                              send(client_socket[i1].sockfd,message,strlen(message), 0);
                          }
                      }
                      bzero(message, 512);
                      sprintf(message, "Client %s is going offline closing group\n",client_socket[i].id);
                      for (i1=0; i1<groupCount; i1++) {
                          if (strcmp(gInfo[i1].adminId,client_socket[i].id)==0) {
                              for (g1=0; g1<gInfo[i1].number_of_members; g1++) {
                                  send(gInfo[i1].members[g1].sockfd,message,strlen(message), 0);
                              }
                              gInfo[i1].number_of_members=0;
                          }
                      }
                      //Removing from non admin group
                      /*for (i1=0; i1<groupCount; i1++) {
                          for (g1=0; g1<gInfo[i1].number_of_members; g1++) {
                              if (strcmp(gInfo[i1].members[g1].id,client_socket[i].id)==0) {
                              }
                      }*/
                      printf("Disconnected Client: %s:",client_socket[i].id);
                      for (i1=0; i1<max_clients; i1++) {
                          if(sd==client_socket[i1].sockfd){
                              client_socket[i1].sockfd = 0;
                              client_socket[i1].online='N';
                          }
                      }
                      close(sd);
                      activeConnection--;
                      continue;
                  }
                  else
                  {
                      send(client_socket[i].sockfd, "Invalid Command", 20, 0);
                  }
              }
          }
      }
    return 0;
}
