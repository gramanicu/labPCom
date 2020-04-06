/*
 * Protocoale de comunicatii
 * Laborator 7 - TCP
 * Echo Server
 * client.c
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#include "helpers.h"

#define CONNECTION_TIMEOUT 10

void run_client(int sockfd) {
    char buf[BUFLEN];
    memset(buf, 0, BUFLEN);

    int byte_count;

    fprintf(stdout, "You : ");
    while (read(STDIN_FILENO, buf, BUFLEN - 1) > 0 && !isspace(buf[0])) {
        byte_count = strlen(buf) + 1;

        int bytes_send;
        int bytes_remaining = byte_count;
        int bytes_received;

        // TODO 4: Trimiteti mesajul catre server prin sockfd

        // memset(buf, 0, BUFLEN);
        DIE(send(sockfd, buf, byte_count, 0) < 0, "Send error");
        // TODO 5: Receptionati un mesaj venit de la server
        recv(sockfd, buf, BUFLEN, 0);

        fprintf(stderr, "\nOther : %s\nYou : ", buf);

        memset(buf, 0, BUFLEN);
    }
}

int main(int argc, char* argv[])
{
    int s = -1;
    struct sockaddr_in serv_addr;
    socklen_t socket_len = sizeof(struct sockaddr_in);
    memset(&serv_addr, 0, socket_len);

    if (argc != 3) {
        printf("\n Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }

    // TODO 1: Creati un socket TCP pentru conectarea la server
    s = socket(PF_INET, SOCK_STREAM, 0);
    DIE(s < 0, "Failed to create socked");

    // TODO 2: Completati in serv_addr adresa serverului, familia de adrese si portul pentru conectare
    inet_aton(argv[1], &serv_addr.sin_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));


    // TODO 3: Creati conexiunea catre server
    int status = -1;
    clock_t start = clock();
    while (status < 0) {
        status = connect(s, (struct sockaddr*)&serv_addr, socket_len);
        clock_t end = clock();

        float secs = (float)(end - start) / CLOCKS_PER_SEC;
        if(secs > CONNECTION_TIMEOUT) {    
            printf("Couldn't connect in %d seconds\n", CONNECTION_TIMEOUT);     
            close(s);
            shutdown(s, SHUT_RDWR);
            return 0;
        }
    }

    printf("Connected to server\n");
    char buf[BUFLEN];
    memset(buf, 0, BUFLEN);
    recv(s, buf, BUFLEN, 0);

    if(buf[0] == '1') {
        fprintf(stderr, "You : ");
        run_client(s);
    } else if (buf[0] == '2') {
        memset(buf, 0, BUFLEN);
        recv(s, buf, BUFLEN, 0);
        fprintf(stderr, "\nOther : %s\nYou : ", buf);
        run_client(s);
    }

    // TODO 6: Inchideti conexiunea si socketul creat
    close(s);
    shutdown(s, SHUT_RDWR);

    return 0;
}
