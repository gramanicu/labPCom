#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <sys/types.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    // Ex 1.1: GET dummy from main server
    message = compute_get_request(
        "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/dummy",
        NULL, 
        NULL,
        0
    );
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 1.2: POST dummy and print response from main server
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    char *post_data[2];
    post_data[0] = "rush=B";
    post_data[1] = "my=friend";

    message = compute_post_request(
        "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/dummy",
        "application/x-www-form-urlencoded",
        post_data,
        2,
        NULL,
        0
    );
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 2: Login into main server
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    char *login[2];
    login[0] = "username=student";
    login[1] = "password=student";

    message = compute_post_request(
        "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/auth/login",
        "application/x-www-form-urlencoded",
        login,
        2,
        NULL,
        0
    );
    
    printf("------------------- \n%s\n ----------\n", message);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);
    // Ex 3: GET weather key from main server
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    char *cookie[1];
    cookie[0] = "connect.sid=s%3ACmDCjNnonXSvd6ysGlDPqCIsLz0DogCT.yflTrwVf7dJ5oF47MqxOrLa1rPxWneKt%2FMsBroksIfQ; Path=/; HttpOnly";

    message = compute_get_request (
        "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/weather/key",
        NULL,
        cookie,
        1
    );

    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 6: Logout from main server
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(
        "ec2-3-8-116-10.eu-west-2.compute.amazonaws.com",
        "/api/v1/auth/logout",
        NULL, 
        NULL,
        0
    );
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // BONUS: make the main server return "Already logged in!"

     sockfd = open_connection("37.139.20.5", 80, AF_INET, SOCK_STREAM, 0);


    // 2. API Openwheather

    char *api = "lat=45.26667&lon=27.98333&appid=b912dd495585fbf756dc6d8f415a7649";

    message = compute_get_request(
        "api.openweathermap.org",
        "/data/2.5/weather",
        api, 
        NULL,
        0
    );
    
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // free the allocated data at the end!
    free(message);
    free(response);

    return 0;
}
