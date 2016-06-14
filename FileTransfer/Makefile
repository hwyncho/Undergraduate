SERVER = server.c
CLIENT = client.c

all: server client

server: $(SERVER)
	gcc -o $@ $(SERVER)

client: $(CLIENT)
	gcc -o $@ $(CLIENT)

clean:
	rm -f server client
