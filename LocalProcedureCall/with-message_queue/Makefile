SERVER = lpc_server.o
CLIENT = lpc_client.o main.o
DATA = *.txt

all : server client

server: $(SERVER)
	gcc -o $@ $(SERVER)

lpc_server.o: lpc_server.c lpc.h
	gcc -c $<

client: $(CLIENT)
	gcc -o $@ $(CLIENT)

lpc_client.o: lpc_client.c lpc.h lpc_client.h
	gcc -c $<

main.o: main.c lpc.h lpc_client.h
	gcc -c $<
	
clean:
	rm -f server $(SERVER) client $(CLIENT) $(DATA)
