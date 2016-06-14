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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define PATH_MAX_SIZE 100
#define BUF_MAX_SIZE 1024

void error_handler(const char * message);

int main(int argc, char * argv[])
{
    int fd;
    char path[PATH_MAX_SIZE];
    char message[BUF_MAX_SIZE];
    int send_data[BUF_MAX_SIZE];
    int read_size;
    struct stat file_stat;
    
    int client_socket;
    struct sockaddr_in server_address;
    
    if (argc != 3)
    {
        printf(">>> Usage : %s <Server's IP> <Port>\n", argv[0]);
        
        exit(1);
    }
    
    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    
    if (client_socket == -1)
        error_handler(">>> socket() error!");
    
    memset(&server_address, 0, sizeof(struct sockaddr_in));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));
    
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handler(">>> connect() error!");
        
    while (1)
    {
        printf("<<< Please input the name of file('q' to exit) : ");
        memset(path, 0x00, sizeof(path));
        scanf("%s", path);
        
        if (strcmp(path, "q") == 0)
        {
            memset(message, 0x00, sizeof(message));
            strcpy(message, "quit");
            
            send(client_socket, message, strlen(message), 0);
            
            break;
        }
        
        else
            fd = open(path, O_RDONLY);
        
        if (fd < 0)
        {
            perror("error");
            
            continue;
        }
        
        else
        {
            send(client_socket, path, strlen(path), 0);
            
            sleep(1);
            
            fstat(fd, &file_stat);
            send(client_socket, &(file_stat.st_size), sizeof(off_t), 0);
            
            sleep(1);
        }
        
        while (1)
        {
            memset(send_data, 0x00, sizeof(send_data));
            read_size = read(fd, send_data, BUF_MAX_SIZE);
            
            if (read_size == 0)
                break;
                
            else
                send(client_socket, send_data, read_size, 0);
        }
        
        memset(message, 0x00, sizeof(message));
        recv(client_socket, message, BUF_MAX_SIZE, 0);
        
        if (strcmp(message, "success") == 0)
            printf(">>> Success!\n");
        
        else
            printf(">>> Fail!\n");
           
        close(fd);
    }
    
    close(client_socket);
    
    return 0;
}

void error_handler(const char * message)
{
    printf("%s\n", message);
    
    exit(1);
}