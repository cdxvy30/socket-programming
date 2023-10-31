#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define PORT 32876
#define BACKLOG 5

int main(int argc, char *argv[]) {
  socklen_t addrLen;
  char welcomeMsg[] = {"Hi, this is SERVER.\n"};
  int status = 0;
  char inputMsg[1024] = {0}, outMsg[1024] = {0};

  // Create socket & PORT for incoming request
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);
  int clientSockFd = 0;

  if (sockFd == -1) {
    printf("Fail to create a socket.");
  }

  struct sockaddr_in serverInfo, clientInfo;
  bzero(&serverInfo, sizeof(serverInfo));
  serverInfo.sin_family = PF_INET;
  serverInfo.sin_addr.s_addr = INADDR_ANY;
  serverInfo.sin_port = htons(PORT);

  // Binding
  status = bind(sockFd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
  if (status == -1) {
    perror("Bind socket failed......\n");
    close(sockFd);
    exit(1);
  }
 
  // Listening
  status = listen(sockFd, BACKLOG);
  if (status == -1) {
    printf("Socket %d listens failed......\n", sockFd);
    close(sockFd);
    exit(0);
  }

  printf("SERVER is ready @ [%s:%d].\nWaiting for connection......\n", inet_ntoa(serverInfo.sin_addr), ntohs(serverInfo.sin_port));

  addrLen = sizeof(clientInfo);

  while(1) {
    clientSockFd = accept(sockFd, (struct sockaddr *)&clientInfo, &addrLen);
    printf("Accept connection request from CLIENT [%s:%d].\n", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));


    while(1) {
      int nbytes = recv(clientSockFd, inputMsg, sizeof(inputMsg), 0);
      if (nbytes <= 0) {
        close(clientSockFd);
        printf("CLIENT closed connection.\n");
        break;
      }
      inputMsg[nbytes] = '\0';
      printf("Receive from CLIENT: %s\n", inputMsg);

      if (strcmp(inputMsg, "Bye") == 0) {
        close(clientSockFd);
        printf("CLIENT closed connection.\n");
        break;
      }
      
      sprintf(outMsg, "%s", inputMsg);
      send(clientSockFd, outMsg, strlen(outMsg), 0);
    }
  }

  close(sockFd);
  return 0;
}
