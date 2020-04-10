/*
*  	Protocoale de comunicatii: 
*  	Laborator 6: UDP
*	client mini-server de backup fisiere
*/

#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "helpers.h"

void usage(char*file)
{
	fprintf(stderr,"Usage: %s ip_server port_server file\n",file);
	exit(0);
}

/*
*	Utilizare: ./client ip_server port_server nume_fisier_trimis
*/
int main(int argc,char**argv)
{
	if (argc!=4)
		usage(argv[0]);
	
	int fd;
	struct sockaddr_in to_station;
	char buf[BUFLEN];


	/*Deschidere socket*/
	int s = socket(AF_INET, SOCK_DGRAM, 0);

	if(s < 0) {
		perror("Failed to create socket!\n");
	}
	
	
	/* Deschidere fisier pentru citire */
	DIE((fd=open(argv[3],O_RDONLY))==-1,"open file");
	
	/*Setare struct sockaddr_in pentru a specifica unde trimit datele*/
	to_station.sin_family = AF_INET;
	to_station.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &to_station.sin_addr);

	/*
	*  cat_timp  mai_pot_citi
	*		citeste din fisier
	*		trimite pe socket
	*/	
	int b_read;
	while((b_read = read(fd, buf, BUFLEN))) {
		if(b_read < 0) {
			perror("Read error!\n");
		}
		int sent = sendto(s, buf, b_read, MSG_CONFIRM, (struct sockaddr*) &to_station, sizeof(to_station));
		// printf("Sent: %s\n", buf);
		sleep(0.005);

		if(sent == -1) {
			perror("Error when sending\n");
		}
	}

	/*Inchidere socket*/
	close(s);
	
	/*Inchidere fisier*/
	close(fd);

	return 0;
}
