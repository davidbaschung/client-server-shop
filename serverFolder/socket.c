// Create a socket connection point

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

//#include "struct.h"
#include "insert.h"
#include "find.h"
#include "remove.h"
#include "printPairs.h"
#include "socket.h"

#define MAXBUFFER 1000
#define PORT 8080

#define READ 0
#define SELL 1
#define BUY 2
#define PRINT 3

struct table * dataTable;


extern int maxLengthOfWords;

// Server console Application for the e-commerce
void * application(void *arg) {
	int mySocket = *((int *) arg);
	int n, convType;
	char *stringBuilder = malloc(MAXBUFFER * sizeof(char));
	char* buffer = malloc(sizeof(char) * MAXBUFFER);
	
	printf("Inside thread socket %d!\n", mySocket);

	// Loop forever (exit happen if client send 'q')
	while (1) {

		// Erase buffer
		bzero(buffer, MAXBUFFER * sizeof(char));
		
		// Check if the client send a closing signal
		evaluateExit(buffer, mySocket);

		// Check which type of communication it is
		if (strncmp("r", buffer, 1) == 0) 
			convType = READ ;
		if (strncmp("s", buffer, 1) == 0)
			convType = SELL;
		if (strncmp("b", buffer, 1) == 0)
			convType = BUY;
		if (strncmp("p", buffer, 1) == 0)
			convType = PRINT;


		printf("Received [r/s/b/p] : %s", buffer);

		// Erase buffer
		bzero(buffer, MAXBUFFER * sizeof(char));

		if (convType != PRINT)
		{
			// Read msg and check if the client send a closing signal			
			evaluateExit(buffer, mySocket);

			printf("Received key : %s", buffer);
		}

		// Copy buffer in 'key'
		char* key = malloc(sizeof(char) * maxLengthOfWords);
		strcpy(key, buffer);
	
		// Remove \n char from 'key'
		key[strlen(key) - 1] = 0;

		int opResult;

		// If it's a read operation, the result is the value of the given key
		if (convType == READ)
			opResult = readOperation(key, mySocket, buffer);

		// If it's a sell operation, the result is 0 if it succeed, -1 if it fails
		if (convType == SELL)
			opResult = sellOperation(key, mySocket, buffer);

		// If it's a buy operation, the result is 0 if it succeed, -1 if it fails		
		if (convType == BUY)
			opResult = buyOperation(key, mySocket, buffer);

		// If it's a print operation, returns the list (valueless) of the objects
		if (convType == PRINT)
			stringBuilder = printOperation(stringBuilder, mySocket);

		char answer[MAXBUFFER];

		// Convert the result into a string and send to the client
		if (convType == PRINT)
		{
			sprintf(answer, "%s", stringBuilder);
			printAll(dataTable);
		} else {
			sprintf(answer, "%d", opResult);
			printf("Answer sent: %s\n", answer);
		}
		
		write(mySocket, answer, sizeof(answer));
		//send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);
//		printf("Sizeof(answer) = %d\n", sizeof(answer));

		// Erase the socket
		bzero(buffer, MAXBUFFER * sizeof(char));
		memset(stringBuilder, 0, sizeof(stringBuilder));
	}

}

int sellOperation(char* key, int mySocket, char* buffer)
{
	// Erase buffer
	bzero(buffer, MAXBUFFER * sizeof(char));

	// Check if the client send a close signal
	evaluateExit(buffer, mySocket);

	printf("Received value : %s", buffer);

	// Convert the value
	int value = atoi(buffer);

	// If we don't insert anything, return -1
	if (value <= 0) return -1;

	// Limit the length of key which is inserted in the table
	char* shorterKey = malloc(sizeof(char) * (maxLengthOfWords + 1));
	strncpy(shorterKey, key, maxLengthOfWords);

	return insertWithValue(dataTable, shorterKey, value);
}

int buyOperation(char* key, int mySocket, char* buffer)
{
	// Erase buffer
	bzero(buffer, MAXBUFFER * sizeof(char));
		
	// Check if the client send a close signal
	evaluateExit(buffer, mySocket);

	// Convert the value
	int value = atoi(buffer);

	// If we don't remove anything, return -1
	if (value <= 0) return -1;

	return decrementWithValue(dataTable, key, value);
}

int readOperation(char* key, int mySocket, char* buffer)
{
	printf("Received request for key : %s\n", key);

	return find(dataTable, key);
}

char* printOperation(char *key, int mySocket)
{
	return printObjects(key, dataTable);
}

void evaluateExit(char *buffer, int socket)
{
	// Read incoming message in buffer, chek if it's a FIN

	//printf("Buffer len : %d\n", strlen(buffer));
	
	while (strlen(buffer) == 0 && *buffer != '\n') {
		//if (read(socket, buffer, MAXBUFFER * sizeof(char)) == 0) {
		if (recv(socket, buffer, sizeof(char) * MAXBUFFER, 0) == 0) {
			printf("Client exit, close socket\n");
			pthread_exit(NULL);
		}
	}

	// Check if the client sent a close signal
	if (strncmp("q\n", buffer, 2) == 0 || strncmp("quit\n", buffer, 5) == 0 ) {
		printf("End of communication on socket %d\n", socket);
		pthread_exit(NULL);
	}
}



// Create the server-side socket
int createSocket(struct table* t) {
	dataTable = t;
	int mySocket, connection, length;
	struct sockaddr_in server, client;

	// Create socket
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// Check if the creation was successful
	if (mySocket != -1) {
		printf("socket created\n");
	} else {
		fprintf(stderr, "Error while creating socket\n");
		exit(0);
	}

	// Init server
	bzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(PORT);

	// Tell to reuse the port also if a previous instance of the programm is still reserving it temporarily
	int yes=1;
	if (setsockopt(mySocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) != 0) {
		fprintf(stderr, "Error setting the sockopt\n");
	}

	// Bind socket
	if ((bind(mySocket, (struct sockaddr *)&server, sizeof(server))) == 0) {
		printf("socket bound\n");
	} else {
		fprintf(stderr, "Error while binding socket\n");
		exit(0);
	}

	// Listen for incoming connections
	if ((listen(mySocket, 5)) == 0) {
		printf("Listening\n");
	} else {
		fprintf(stderr, "Error while listening for connection\n");
		exit(0);		
	}

	// Keep track of the threads
//	pthread_t tid[210];
	pthread_t tid[1];
	int i = 0;
	length = sizeof(client);

	while(1) {
		// Accept connection
		connection = accept(mySocket, (struct sockaddr *)&client, &length);
		if (connection >= 0) {
			printf("connection accepted\n");
		} else {
			fprintf(stderr, "Error while listening for connection\n");		
			exit(0);
		}

		if (i >= 200) {
			close(mySocket);
		}

		// Allocate a new space for the variable we pass to the thread, instead of passing the same to each
		int* conNb = malloc(sizeof(int));
		conNb = &connection;

		// Create a new thread wich manage the communication
		if (pthread_create(&tid[0], NULL, application, conNb) != 0)
			fprintf(stderr, "Failed to create thread\n");

		// Wait for the firts 50 threads to finish before accepting news
/*		if (i >= 200) 
		{
			i = 0;
			while (i < 200) {
				pthread_join(tid[i++], NULL);
				printf("Waiting for some threads to finish\n");
			}
			i = 0;
		}
*/		

	}


	// Close socket
	close(mySocket);
}
