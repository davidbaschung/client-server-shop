#ifndef OS_PROJECT_SOCKET_H
#define OS_PROJECT_SOCKET_H

	void application(int mySocket);
	int createSocket();
	// Evaluate if the client wants to end the communication
	void evalueExit(char* buffer, int socket);

#endif