/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Ubuntu.
 */
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PERMS 0777
#define PATH_MAX_SIZE 100
#define BUF_MAX_SIZE 1024

void error_handler(const char * message);

int main(int argc, char * argv[])
{
    int fd;
    char path[PATH_MAX_SIZE];
    char message[BUF_MAX_SIZE];
    int receive_data[BUF_MAX_SIZE];
    int receive_size;
    off_t file_size, save_size;

    int server_socket;
    int client_socket;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int client_address_size;

    if (argc != 2)
    {
        printf("Usage : %s <Port>\n", argv[0]);

        exit(1);
    }

    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
        error_handler(">>> socket() error!");

    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handler(">>> bind() error!");

    if (listen(server_socket, 5) == -1)
        error_handler(">>> listen() error!");

    while (1)
    {
        printf(">>> Waiting...\n");

        memset(&client_address, 0, sizeof(struct sockaddr_in));
        client_address_size = sizeof(client_address);
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);

        if (client_socket == -1)
            error_handler(">>> accept() error!");

        else
            printf(">>> New client : %s\n", inet_ntoa(client_address.sin_addr));

        while (1)
        {
            memset(path, 0x00, sizeof(path));
            recv(client_socket, path, PATH_MAX_SIZE, 0);

            if (strcmp(path, "quit") == 0)
                break;

            recv(client_socket, &file_size, sizeof(off_t), 0);
            save_size = 0;

            printf(">>> File name : %s, size : %dbyte\n", path, (int)file_size);

            fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, PERMS);

            while (1)
            {
                if (save_size != file_size)
                {
                    memset(receive_data, 0x00, sizeof(receive_data));
                    receive_size = recv(client_socket, receive_data, BUF_MAX_SIZE, 0);
                    write(fd, receive_data, receive_size);
                    save_size += (off_t)receive_size;
                }

                else
                {
                    printf(">>> Success!\n");
                    
                    break;
                }
            }

            memset(message, 0x00, sizeof(message));
            strcpy(message, "success");
            send(client_socket, message, strlen(message), 0);

            close(fd);
        }

        close(client_socket);
    }
}

void error_handler(const char * message)
{
    printf("%s\n", message);

    exit(1);
}
