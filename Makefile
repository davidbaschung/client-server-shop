all : client server test

client : clientFolder/client.c clientFolder/socket.c
	gcc -o client clientFolder/client.c clientFolder/socket.c

server : serverFolder/server.c serverFolder/init.c serverFolder/socket.c serverFolder/insert.c serverFolder/find.c serverFolder/remove.c serverFolder/hash.c serverFolder/printPairs.c
	gcc -o server -pthread serverFolder/server.c serverFolder/init.c serverFolder/socket.c serverFolder/insert.c serverFolder/find.c serverFolder/remove.c serverFolder/hash.c serverFolder/printPairs.c

test : clientFolder/testClient.c
	gcc -o test -pthread clientFolder/testClient.c