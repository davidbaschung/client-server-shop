// Create and connect socket

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include "socket.h"

#define MAXBUFFER 1000
#define PORT 8080
#define true 1
#define false 0

#define READ 0
#define SELL 1
#define BUY 2
#define PRINT 3


// Client console Application for the e-commerce
void application(int mySocket) {

	// Allocate space for the buffer
	char* buffer = malloc(sizeof(char) * MAXBUFFER);
	int n=0, convType;
	printf(" ----- Welcome to the store ! -----\n");

	// While we dont write 'q' to quit
	for (;;) {
		for (int again = true ; again == true ; ) {
			again = false;
			printf("Do you want to read, sell, buy a key or print the objects? [r/s/b/p] : \t");

			// Erase buffer and reset n
			bzero(buffer, MAXBUFFER * sizeof(char));
			n=0;

			// Read from input
			while ((buffer[n++] = getchar()) != '\n');

			// Exit if the input is 'q' of 'quit' (after signaling it to the server)
			evalueExit(buffer, mySocket);

			// Repeat if no valid operation was specified
			if ( ! (strncmp("r", buffer, 1) == 0 || strncmp("s", buffer, 1) == 0 || strncmp("b", buffer, 1) == 0  || strncmp("p", buffer, 1) == 0))
				again = true;
		}

		// Write on the socket
		//write(mySocket, buffer, sizeof(buffer));
		send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);

		// What kind of operation?
		if(strncmp("r", buffer, 1) == 0)
			convType = READ;
		else if (strncmp("s", buffer, 1) == 0)
			convType = SELL;
		else if (strncmp("b", buffer, 1) == 0)
			convType = BUY;
		else 
			convType = PRINT;
		

		if (convType != PRINT)
		{
			printf("What is the key? : \t");

			// Erase buffer and reset n
			bzero(buffer, MAXBUFFER * sizeof(char));
			n=0;

			// Read from input
			while ((buffer[n++] = getchar()) != '\n');

			// Exit if the input is 'q' of 'quit' (after signaling it to the server)
			evalueExit(buffer, mySocket);
			
			// Write on the socket
			//write(mySocket, buffer, sizeof(char) * (n + 1));
			send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);


			// If it is a sell or a write operation, it needs a value
			if (convType == SELL || convType == BUY) {
				for (int again=true ; again==true ; ) {
					again = false;

					printf("What is the value? (integer) : \t");

					// Erase buffer and reset n
					bzero(buffer, MAXBUFFER * sizeof(char));
					n=0;

					// Read from input
					while ((buffer[n++] = getchar()) != '\n');

					// Exit if the input is 'q' of 'quit' (after signaling it to the server)
					evalueExit(buffer, mySocket);

					// Read buffer to check if it is an int
					for(int i = 0; i < strlen(buffer) - 1; i++) {
						if (!isdigit(buffer[i]))
							again = true;
					}
				}

				// Write on the socket
				//write(mySocket, buffer, sizeof(buffer));
				send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);

				// Erase the socket
				bzero(buffer, MAXBUFFER * sizeof(char));
			}
		}

		memset(buffer, sizeof(char) * MAXBUFFER, 0);

		usleep(100000);

		//read(mySocket, buffer, MAXBUFFER * sizeof(char));
		recv(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);
	

		// Exit if the input is 'q' of 'quit' (after signaling it to the server)
		evalueExit(buffer, mySocket);

		// Receive the new value of the key. If -1, the operation failed
		printf("Received answer : \n%s\n", buffer);

		// Erase the socket
		bzero(buffer, MAXBUFFER * sizeof(char));
	}

}


void evalueExit(char* buffer, int socket)
{
	// Check if the client sent a close signal
	if (strncmp("q\n", buffer, 2) == 0 || strncmp("quit\n", buffer, 5) == 0 ) {
		write(socket, buffer, sizeof(buffer));
		printf("End of communication\n");
		exit(0);
	}
}

// Create the client-side socket
int createSocket() {
	int mySocket, connection, length;
	struct sockaddr_in server, client;

	// Create socket
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// Check if the creation was succesfully
	if (mySocket != -1) {
		printf("socket created\n");
	} else {
		fprintf(stderr, "Error while creating socket\n");
		exit(0);
	}

	// Init server
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(PORT);

	// Connect with server
	if (connect(mySocket, (struct sockaddr *)&server, sizeof(server)) == 0) {
		printf("Connected\n");
	} else {
		fprintf(stderr, "Error while connecting with server\n");
		exit(0);
	}

	// Communicate with client
	application(mySocket);

	// Close socket
	close(mySocket);
}
