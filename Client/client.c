#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>
#define PORT 8989
#define SA struct sockaddr




typedef enum Bool {
    FALSE,
    TRUE
} Bool;


int SERVERPORT = 8989;
Bool condition = true;//1 - создать 0 - удалить
char* adress = "0.0.0.0";
char* path = "./data";
char* nameOfFile = "work.txt";  

int main(int argc, char ** argv){

    //get port and path from argv
    if (argc > 1)//if we give arguments
       {
           printf("with arguments\n");
           SERVERPORT = atoi(argv[1]);
           path = argv[2];
           adress = argv[3];
           int tmp = atoi(argv[4]);
           if (tmp == 0){
               condition = false;
           }
           nameOfFile = argv[5];
           
       }
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    // создание и изменение сокета

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1) {

        printf("socket creation failed...\n");

        exit(0);

    }

    else

        printf("Socket successfully created..\n");

    bzero(&servaddr, sizeof(servaddr));

  

    // назначаем IP, PORT

    servaddr.sin_family = AF_INET;

    servaddr.sin_addr.s_addr = inet_addr(adress);

    servaddr.sin_port = htons(SERVERPORT);

  

    // подключаем сокет клиента к сокету сервера

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {

        printf("connection with the server failed...\n");

        exit(0);

    }

    else

        printf("connected to the server..\n");
  
    // отправка данных
    unsigned int size = strlen(nameOfFile);
    send(sockfd, &size, 2, 0);
    send(sockfd, &condition, 1, 0);
    send(sockfd, nameOfFile, size, 0);
    // закрываем сокет

    close(sockfd);

} 