/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implemets "cp" command.
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:11
 */

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PERMS 0644

int main(int argc, char * argv[])
{
    int original, copy;
    struct stat filestat;
    unsigned char * buff;
    
    if ((original = open(argv[1], O_RDONLY)) == -1)
    {
        perror("error");
        
        return 1;
    }
    
    stat(argv[1], &filestat);
    buff = (unsigned char *)malloc(filestat.st_size);
    memeset(buff, 0x00, filestat.st_size);
    read(original, buff, filestat.st_size);
    close(original);
    
    copy = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, PERMS);    
    write(copy, buff, filestat.st_size);
    close(copy);
    
    free(buff);
    
    return 0;
}
