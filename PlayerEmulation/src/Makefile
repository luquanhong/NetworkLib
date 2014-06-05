

all : server client output


server : player_server.c
	gcc -o server player_server.c
	
client : player_client.c
	gcc -o client player_client.c
	
output : output.c
	gcc -o output output.c


clean:
	rm -rf *.o client server output