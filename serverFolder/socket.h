#ifndef OS_PROJECT_SOCKET_H
#define OS_PROJECT_SOCKET_H

	void * application(void *arg);
	int createSocket(struct table *t);
	int sellOperation(char* key, int mySocket, char* buffer);
	int buyOperation(char* key, int mySocket, char* buffer);
	int readOperation(char* key, int mySocket, char* buffer);
	char * printOperation(char *key, int mySocket);
	void evaluateExit(char *buffer, int socket);
#endif