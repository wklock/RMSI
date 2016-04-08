//
// Created by William Klock on 3/15/16. Based on server/client examples on tutiralspoint.com
// RAI.D drive that the will communicate with controllers and with eachother.
//
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>
#include "drive.h"

int main( int argc, char *argv[] ) {
   int sockfd, newsockfd, portno, clilen, hd_num;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n, pid;

   if (argc < 3) {
	   fprintf(stderr,"usage %s <controller ip> <port>\n", argv[0]);
	   exit(0);
   }

   /* First call to socket() function */
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0) {
	   perror("ERROR opening socket");
       exit(1);
   }

   /* Initialize socket structure */
   bzero((char *) &serv_addr, sizeof(serv_addr));
   portno = 5001;

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   /* Now bind the host address using bind() call.*/
   if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
	  perror("ERROR on binding");
	  exit(1);
   }

   /* Now start listening for the clients, here
	  * process will go in sleep mode and will wait
	  * for the incoming connection
   */

   listen(sockfd,5);
   clilen = sizeof(cli_addr);

   while (1) {
	  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	  if (newsockfd < 0) {
		 perror("ERROR on accept");
		 exit(1);
	  }

	  /* Create child process to handle communication */
	  pid = fork();

	  if (pid < 0) {
		 perror("ERROR on fork");
		 exit(1);
	  }

	  if (pid == 0) {
		 /* This is the child process */
		 close(sockfd);
		 doprocessing(newsockfd);
		 exit(0);
	  }
	  else {
		 close(newsockfd);
	  }

   } /* end of while */
}

void doprocessing (int sock) {
   int n;
   char buffer[256];
   bzero(buffer,256);
   n = read(sock,buffer,255);

   if (n < 0) {
	  perror("ERROR reading from socket");
	  exit(1);
   }

   printf("Controller received message: %s\n",buffer);
   n = write(sock,"I got your message",18);



   if (n < 0) {
	  perror("ERROR writing to socket");
	  exit(1);
   }

}
