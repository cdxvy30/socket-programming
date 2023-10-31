#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

#define serverPort 32876

int main(int argc, char *argv[]) {
  int status = 0;
  char inputMsg[1024] = {0}, outMsg[1024] = {0};
  
  int sockFd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockFd == -1) {
    printf("Fail to create a socket.");
  }

  struct sockaddr_in serverInfo;
  bzero(&serverInfo, sizeof(serverInfo));
  serverInfo.sin_family = PF_INET;
  serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
  serverInfo.sin_port = htons(serverPort);

  status = connect(sockFd, (struct sockaddr *)&serverInfo, sizeof(serverInfo));
  if (status == -1) {
    printf("connection error......\n");
    exit(1);
  }

  while (1) {
    printf("Please input message that will be sent to SERVER: ");
    fgets(outMsg, sizeof(outMsg), stdin);
    size_t len = strlen(outMsg);
    if (len > 0 && outMsg[len - 1] == '\n') {
      outMsg[len - 1] = '\0';
    }
    printf("Send: %s\n", outMsg);
    send(sockFd, outMsg, strlen(outMsg), 0);

    // int nbytes = recv(sockFd, inputMsg, sizeof(inputMsg), 0);
    // if (nbytes <= 0) {
    //     close(sockFd);
    //     printf("SERVER closed connection.\n");
    //     break;
    // }
    // inputMsg[nbytes] = '\0';
    // printf("Receive from SERVER: %s\n", inputMsg);
  }

  return 0;
}
