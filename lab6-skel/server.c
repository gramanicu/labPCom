/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s server_port file\n",file);
	exit(0);
}

/*
*	Utilizare: ./server server_port nume_fisier
*/
int main(int argc,char**argv)
{
	int fd;

	if (argc!=3)
		usage(argv[0]);
	
	struct sockaddr_in my_sockaddr, from_station ;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if(s < 0) {
		perror("Failed to create socket!\n");
	}
	
	/*Setare struct sockaddr_in pentru a asculta pe portul respectiv */
	my_sockaddr.sin_family = AF_INET;
	my_sockaddr.sin_port = htons(atoi(argv[1]));
	my_sockaddr.sin_addr.s_addr = INADDR_ANY;

	/* Legare proprietati de socket */
	int r = bind(s, (struct sockaddr*) &my_sockaddr, sizeof(my_sockaddr));
	
	if(r < 0) {
		perror("Failed to bind to socket!");
	}
	
	
	/* Deschidere fisier pentru scriere */
	DIE((fd=open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))==-1,"open file");
	socklen_t lenght = sizeof(from_station);

	int b_rec ;
	while((b_rec = recvfrom(s, buf, BUFLEN, MSG_WAITALL, (struct sockaddr*) &from_station, &lenght))) {
		printf("Received %d bytes\n", b_rec);
		write(fd, buf, b_rec);

		if(b_rec == BUFLEN) {
			continue;
		} else {
			close(s);
			close(fd);
			break;
		}
	}


	return 0;
}
