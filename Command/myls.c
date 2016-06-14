/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements "ls" command.
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:11
 */

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(void)
{
    int i, j;
    int count = 0;
    char * flist[512];
    char * temp;
    DIR * dirp;
    struct dirent * direntp;
    struct stat filestat;

    if ((dirp = opendir(".")) == NULL)
    {
        perror("error");

        return 1;
    }
    
    while ((direntp = readdir(dirp)) != NULL)
    {
        flist[count++] = direntp->d_name;
    }
    
    for (i = 0; i < count - 1; i++)
    {
        for (j = 0; j < count - i - 1; j++)
        {
            if (strcmp(flist[j], flist[j + 1]) > 0)
            {
                temp = flist[j];
                flist[j] = flist[j + 1];
                flist[j + 1] = temp;
            }
        }
    }
    
    for (i = 0; i < count; i++)
    {
        if (flist[i][0] != '.')
        {
            stat(flist[i], &filestat);
            
            if (S_ISREG(filestat.st_mode))
            {
                if (filestat.st_mode & 001001001)
                    printf("\033[1;32m%s  ", flist[i]);
                
                else
                    printf("\033[0m%s  ", flist[i]);
            }
            
            else if (S_ISDIR(filestat.st_mode))
                printf("\033[1;34m%s  ", flist[i]);
                
            else if (S_ISCHR(filestat.st_mode))
                printf("\033[1;37m%s  ", flist[i]);
            
            else if (S_ISBLK(filestat.st_mode))
                printf("\033[1;31m%s  ", flist[i]);
            
            else if (S_ISFIFO(filestat.st_mode))
                printf("\033[1;37m%s  ", flist[i]);
            
            else if (S_ISLNK(filestat.st_mode))
                printf("\033[1;36m%s  ", flist[i]);
            
            else if (S_ISSOCK(filestat.st_mode))
                printf("\033[1;35m%s  ", flist[i]);
        }
    }
    
    close(dirp);
    
    printf("\n");
      
    return 0;
}
