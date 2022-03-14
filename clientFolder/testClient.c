#include <stdio.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "testClient.h"

#define MAXBUFFER 1000
#define PORT 8080

#define SELL 0
#define BUY 1

// Nb of times customer and shop functions have to loop
int customersTimes = 1;
int shopsTimes = 1;

// Results
int totalSell = 0;
int failedSell = 0;
int totalBuy = 0;
int failedBuy = 0;

// Mutex to write the result
sem_t mutexSell, mutexBuy;

// Automatic client
/*
*   1. argument: nb of shop selling 'testObject'
*   2. argument: nb of 'testObject' each shop sells
*   3. argument: nb of customers buying 'testObject' (or trying to do it)
*   4. argument: nb of 'testObject' each shop tries to buy
*   5. argument: milliseconds the customers waits after the shops start selling
*/
int main(int argc, char* argv[])
{
    printf("CLIENT SOCKET AUTOMATIC TEST\n");

    // Init mutex for sell results
    if (sem_init(&mutexSell, 0, 1) != 0)
    {
        printf("Mutex initialization failed\n");
        return 1;
    }

    // Init mutex for buy results
    if (sem_init(&mutexBuy, 0, 1) != 0)
    {
        printf("Mutex initialization failed\n");
        return 1;
    }

    // Default values
    int shops = 1;
    int customers = 1;
    int customersWait = 100;

    if (argc >= 2)
        shops = atoi(argv[1]);
    if (argc >= 3)
        shopsTimes = atoi(argv[2]);
    if (argc >= 4)
        customers = atoi(argv[3]);
    if (argc >= 5)
        customersTimes = atoi(argv[4]);
    if (argc >= 6)
        customersWait = atoi(argv[5]);

//    printf("%d %d %d %d %d\n", shops, shopsTimes, customers, customersTimes, customersWait);


    pthread_t threads[shops + customers];

    // Create shops threads
    if (shops > 0 && shopsTimes > 0) {
        for (int i = 0; i < shops; i++) {
            pthread_create(&threads[i], NULL, shop, (void *) (intptr_t) i);
            
            // Wait to be sure the server accept on different sockets
            usleep(10000);
        }
    }


    // Wait 'customersWait' milliseconds
    usleep(1000 * customersWait);

    // Create customers threads
    if (customers > 0 && customersTimes > 0) {
        int total = 0;
        if (shopsTimes != 0) total += shops;
        if (customersTimes != 0) total += customers;

        for (int i = shops; i < total; i++) {
            pthread_create(&threads[i], NULL, customer, (void *) (intptr_t) i);

            // Wait to be sure the server accept on different sockets
            usleep(10000);
        }
    }

    // Join threads
    for (int i = 0; i < (shops + customers); i++) {
        pthread_join(threads[i], NULL);
    }


    

    printf("Test finished\n");

    printf("\n\n");

    printf("Total Sell: %d, from which %d failed\n", totalSell, failedSell);

    printf("Total Buy: %d, from which %d failed\n", totalBuy, failedBuy);

    return 0;
}

void* shop(void* times) {
    //int i = (int) times;
    //printf("%d ----------------------------------------------\n\n\n\n\n", i);

    createSocket(SELL, shopsTimes);
}

void* customer(void* times) {
    //int i = (int) times;
    //printf("%d ----------------------------------------------\n\n\n\n\n", i);

    createSocket(BUY, customersTimes);
}

// Create the client-side socket
int createSocket(int type, int times) {
	int mySocket, connection, length;
	struct sockaddr_in server, client;

	// Create socket
	mySocket = socket(AF_INET, SOCK_STREAM, 0);

	// Check if the creation was successful
	if (mySocket != -1) {
		printf("socket %d created\n", mySocket);
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
	application(mySocket, type, times);

	// Close socket
	close(mySocket);

    return 0;
}


// Buy/sell items
void application(int mySocket, int type, int repTimes) {


	// Allocate space for the buffer
	char* buffer = malloc(sizeof(char) * MAXBUFFER);

    // Keep track of the operations results
    int operations = 0;
    int failedOperations = 0;

    // Wait 0,1 seconds (100'000 microseconds)
    //usleep(100000);

	// Repeat for 'times' times
	for (int i = 0; i < repTimes; i++) {
		
        // Write the operation type (s for sell, b for buy)
        if (type == SELL)
        {
            buffer[0] = 's';
            printf("Starting a sell operation\n");
        }
        else if (type == BUY)
        {        
            buffer[0] = 'b';
            printf("Starting a buy operation\n");
        }

        buffer[1] = '\n';

		// Write on the socket
		//write(mySocket, buffer, sizeof(char) * MAXBUFFER);
        send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);


        // Erase buffer
        bzero(buffer, sizeof(char) * MAXBUFFER);

        // Wait 0,1 seconds (100'000 microseconds)
        usleep(100000);

        // Write the object to sell/buy (always 'testObject' for the tests)
        buffer[0] = 't';
        buffer[1] = 'e';
        buffer[2] = 's';
        buffer[3] = 't';
        buffer[4] = 'O';
        buffer[5] = 'b';
        buffer[6] = 'j';
        buffer[7] = 'e';
        buffer[8] = 'c';
        buffer[9] = 't';
        buffer[10] = '\n';
        printf("Asking for 'testObject'\n");
        
        // Write on the socket
        //write(mySocket, buffer, sizeof(char) * MAXBUFFER);
        send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);

        // Wait 0,1 seconds (100'000 microseconds)
        usleep(100000);

        // Erase buffer
        bzero(buffer, sizeof(char) * MAXBUFFER);

        // Write the value to sell/buy (always 1 for the tests)
        buffer[0] = '1';
        buffer[1] = '\n';
        printf("Asking for 1 element\n");

        // Write on the socket
        //write(mySocket, buffer, sizeof(char) * MAXBUFFER);
        send(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);

        // Erase the buffer
        bzero(buffer, sizeof(char) * MAXBUFFER);
    
        // Wait 0,1 seconds (100'000 microseconds)
        usleep(100000);

        // Read the entire socket (1000 Bytes)
		//read(mySocket, buffer, MAXBUFFER * sizeof(char));
        int bytes = recv(mySocket, buffer, sizeof(char) * MAXBUFFER, 0);
        //printf("%d bytes\n", bytes);
        

        usleep(100000);

		// Receive the new value of the key. If -1, the operation failed
		printf("Received answer : \n%s\n", buffer);

        // Update operations count
        operations++;

        // If the operation failed, update failed count
        if (atoi(buffer) < 0)
            failedOperations++;

		// Erase the socket
		bzero(buffer, sizeof(char) * MAXBUFFER);
        memset(buffer, 0 , sizeof(char) * MAXBUFFER);

        // Wait a moment before starting a new request
        usleep(100000);
	}

    // Update total results using mutex to avoid concurrent modification problems
    if (type == SELL) {

        sem_wait(&mutexSell);

        totalSell += operations;
        failedSell += failedOperations;

        sem_post(&mutexSell);

    } else if (type == BUY) {
        sem_wait(&mutexBuy);

        totalBuy += operations;
        failedBuy += failedOperations;

        sem_post(&mutexBuy);
    }
    

    free(buffer);

}
