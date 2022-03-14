#ifndef OS_PROJECT_TESTCLIENT_H
#define OS_PROJECT_TESTCLIENT_H

	int createSocket(int type, int times);
	void application(int mySocket, int type, int times);
	void* customer(void* times);
	void* shop(void* times);

#endif // OS_PROJECT_TESTCLIENT_H
