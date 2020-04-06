/*
 * Protocoale de comunicatii
 * Laborator 7 - TCP
 * Echo Server
 * server.c
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <string.h>

#include "helpers.h"

// Primeste date de pe connfd1 si trimite mesajul receptionat pe connfd2, la serverul de echo connfd1 == connfd2
int receive_and_send(int connfd1, int connfd2)
{
    char buf[BUFLEN];
    int bytes_send;
    int bytes_remaining;
    int bytes_received = 0;

    // TODO 6: Receptionati un mesaj de la connfd1

    bytes_received = recv(connfd1, buf, BUFLEN, 0);

    if (bytes_received != 0) {
        fprintf(stderr, "Received: %s", buf);
    }

    bytes_remaining = bytes_received;

    // TODO 7: Timiteti mesajul la connfd2
    DIE(send(connfd2, buf, bytes_received, 0) < 0, "Send error");

    return bytes_received;
}

void run_echo_server(int listenfd)
{
    struct sockaddr_in client_addr;

    int bytes_received;
    int connfd = -1;
    socklen_t socket_len = sizeof(struct sockaddr_in);

    // TODO 4: Ascultati pentru un singur client pe socketul dat
    DIE(listen(listenfd, 1) == -1, "Can't listen");

    // TODO 5: Acceptati o conexiune
    connfd = accept(listenfd, (struct sockaddr*) &client_addr, &socket_len);
    DIE(connfd < 0, "Bad connection");

    printf("Client connected\n");
    DIE(send(connfd, "1", strlen("1") + 1, 0) < 0, "Send error");

    do {
        bytes_received = receive_and_send(connfd, connfd);
    } while (bytes_received > 0);

    // TODO 8: Inchideti conexiunea si socket-ul clientului
    close(connfd);
    shutdown(connfd, SHUT_RDWR);  
}

void run_chat_server(int listenfd)
{
    struct sockaddr_in client_addr1;
    struct sockaddr_in client_addr2;

    int bytes_received;
    int connfd1 = -1;
    int connfd2 = -1;
    socklen_t socket_len = sizeof(struct sockaddr_in);

    // TODO 4: Ascultati pentru doi clineti pe socketul dat
    DIE(listen(listenfd, 2) == -1, "Can't listen");

    // TODO 5: Acceptati doua conexiuni
    connfd1 = accept(listenfd, (struct sockaddr*)&client_addr1, &socket_len);
    DIE(send(connfd1, "1", strlen("1") + 1, 0) < 0, "Send error");
    printf("Client connected. Waiting for the second client\n");

    connfd2 = accept(listenfd, (struct sockaddr*)&client_addr2, &socket_len);
    DIE(send(connfd2, "2", strlen("2") + 1, 0) < 0, "Send error");
    printf("Client connected\n");

    do {
        bytes_received = receive_and_send(connfd1, connfd2);

        if (bytes_received == 0)
            break;

        bytes_received = receive_and_send(connfd2, connfd1);
    } while (bytes_received > 0);

    // TODO 8: Inchideti conexiunile si socketii creati
    close(connfd1);
    close(connfd2);
    shutdown(connfd1, SHUT_RDWR); 
    shutdown(connfd2, SHUT_RDWR);   
}

int main(int argc, char* argv[])
{
    int listenfd = -1;
    struct sockaddr_in serv_addr;
    socklen_t socket_len = sizeof(struct sockaddr_in);


    if (argc != 3) {
        printf("\n Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }

    // TODO 1: Creati un socket TCP pentru receptionarea conexiunilor
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    DIE(listenfd<0, "Failed to create socket");

    // TODO 2: Completati in serv_addr adresa serverului pentru bind, familia de adrese si portul rezervat pentru server
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));


    // TODO 3: Asociati adresa serverului cu socketul creat folosind bind
    DIE(bind(listenfd, (struct sockaddr*)&serv_addr, socket_len) < 0, "Couldn't bind");
    run_echo_server(listenfd);
    // run_chat_server(listenfd);

    // TODO 9: Inchideti socketul creat
    close(listenfd);
    shutdown(listenfd, SHUT_RDWR);

    return 0;
}
