all:server.o client.o
	gcc -o server server.o
	gcc -o client client.o
server.o:server.c
	gcc -c server.o server.c -std=c99
client.o:client.c
	gcc -c client.o client.c
clean:
	rm -rf *.o server client
