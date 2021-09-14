#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include "clientsQueue.h"

#define SERVERPORT 8989
#define BUFSIZE 4096
#define SOCKETERROR (-1)
#define SERVER_BACKLOG 100
#define THREAD_POOL_SIZE 20

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

/*do most operations with client*/
void * handle_connection(void* p_client_socket);
/*catch errors, print them,
program wont be terminated*/ 
int check(int exp, const char *msg);
/*process pool of threads*/
void * thread_function(void* arg);

int main(int argc, char ** argv){
    int server_socket, client_socket, addr_size;
    SA_IN server_addr, client_addr;

    /*
    init pool of threads
    */
    for(int i = 0; i<THREAD_POOL_SIZE; ++i){
        pthread_create(&thread_pool[i],NULL,thread_function, NULL);
    }
    
    check((server_socket = socket(AF_INET, SOCK_STREAM, 0)),
            "Failed to create socket");
    
    /*
    sin_family - Семейство адресов - 
    Константа AF_INET соответствует Internet-домену. 
    Сокеты, размещённые в этом домене, могут использоваться
        для работы в любой IP-сети. 
    */
    server_addr.sin_family = AF_INET;
    
    /*
    sin_addr - содержит IP адрес к которому будет привязан сокет.
    в нашем случае - 0.0.0.0
    */
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    /*
    sin_port - порт для сокета
    */
    server_addr.sin_port = htons(SERVERPORT);
    
    /*bind() выделяет сокету собственный локальный адрес */
    check(bind(server_socket,(SA*)&server_addr, sizeof(server_addr)),
            "Bind Failed!");
    
    /*
    Функция listen используется сервером, чтобы информировать ОС,
        что он ожидает ("слушает") запросы связи на данном сокете.
    Без такой функции всякое требование связи с этим сокетом
        будет отвергнуто.
    */
    check(listen(server_socket, SERVER_BACKLOG),
            "Listen Failed!");

    while (true) {
        //wait for client
        printf("Waiting for connections..\n");
        
        /*
        Функция accept
        Первый аргумент функции - сокет-дескриптор для принятия связей от клиентов.
        Второй аргумент - указатель на адрес клиента (структура sockaddr ) для соответствующего домена.
        Третий аргумент - указатель на целое число - длину структуры адреса.
        Второй и третий аргументы заполняются соответствующими значениями в момент установления связи клиента
            с сервером и позволяют серверу точно определить, с каким именно клиентом он общается.
        */
        addr_size = sizeof(SA_IN);
        check(client_socket = 
            accept(server_socket, (SA*)&client_addr, (socklen_t*)&addr_size),
                "Accept Failed!");
        printf("Connected!\n");
        
        //client connected, work with client        

        int* pclient = malloc(sizeof(int));
        *pclient = client_socket;
        pthread_mutex_lock(&mutex);
        enqueue(pclient);
        pthread_mutex_unlock(&mutex);
    } //while

    return 0;
}

int check(int exp, const char *msg){
    if (exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return exp;
}

void * thread_function(void* arg) {
    while (true)
    {
        pthread_mutex_lock(&mutex);
        int *pclient = dequeue();
        pthread_mutex_unlock(&mutex);
        if (pclient != NULL) {
            //we have a connection
            handle_connection(pclient);
        }
    }
}

void * handle_connection(void* p_client_socket) {
    int client_socket = *((int*)p_client_socket);
    free(p_client_socket);
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX+1];

    while((bytes_read = read(client_socket, buffer+msgsize,sizeof(buffer)-msgsize-1)) > 0 ) {
        msgsize += bytes_read;
        if(msgsize>BUFSIZE-1 || buffer[msgsize-1] == '\n') break;
    }
    check(bytes_read, "Recieve error!");
    buffer[msgsize-1] = 0;
    
    printf("REQUEST: %s\n", buffer);
    
    write(client_socket,buffer, msgsize);
    close(client_socket);
    printf("Closing connection\n");
    return NULL;
}