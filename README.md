# OS 2020 e-commerce

How to use the files: 
- server, client and test are compiled c programs
- autorun_test and script/aNormalDay are bash scripts
- script/aNormalPerson and script/expect are script using the 'expect' language, and extension of the Tcl language which basically allows the script to simulate user input


make: compiles server, client and test

server: start a server initializing random itemes from file serverFolder/words.txt or competely random if this operation fails. Then listen for incoming requests.

client: send a connection request to the server. If succeed, then it allows the user to communicate with the server.

test: used to generate multiple shops and customers concurrently and communicate with the server (this should be running). The first argument is the number of shops to generate. The second the number of item each shop has to sell (one by one). The third the number of customers to generate. The fourth the number of item each customer has to (try to) buy (one by one). The fifth it the time in millisecond between the creation of the shops and the creation of the customers. The item used by both shop and customer is always a 'testObject'.

autorun_test: single command to run a test of the program. It starts a server (if not already running), then execute a test with arguments '6 5 2 8 500', then finally opens a client (using the script expect) and ask for the item 'testObject'. It the server has just been initialized, its value should be 14 (6 * 5 - 2 * 8).

script/aNormalDay: simulates a normal day for the server. A random number of people 'comes in' to buy/sell a random number of a random item. This is done using the script script/aNormalPerson.



How to interpret server responses:


- For a read 'r' operation:
	1 to N : The value of the requested item
	-1 : The item is not present in the db

- For a sell 's' operation:
	0 : The operation succeeded, the element was already in the db and its value has been incremented
	1 : The operation succeeded, the element was not yet in the db
	-1 : The operation failed

- For a buy 'b' operation:
	0 : The operation succeeded
	-1 : The operation failed, either because the requested item was not present or because the requested quantity was too elevated
