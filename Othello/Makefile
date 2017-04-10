SERVER = Server.c
CLIENT = Client.c

all: server client

server: $(SERVER)
	gcc -o $@ $(SERVER) -lncurses

client: $(CLIENT)
	gcc -o $@ $(CLIENT) -lncurses
	
clean:
	rm -f server client
