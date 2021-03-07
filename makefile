all:server.c client.c
	gcc -g -o server server.c -std= c99
	gcc -o client client.c
clean:
	rm -rf *.o server client
