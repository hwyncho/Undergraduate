/*
 * Name : HwiYeon Cho
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:27
 */
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PERMS 0777

typedef struct _input
{
    char ID[21];
    char PW[16];
} INPUT;

typedef struct _account
{
    char ID[21];
    char PW[16];
    int WIN;
    int LOSE;
} ACCOUNT;

typedef struct _data
{
    int turn;
    int winner;
    int countP1;
    int countP2;
    int cursorOfMap;
    int dataOfCursor[6][6];
    int dataOfStone[6][6];
} DATA;

/* Global Variables */
char currentDIR[101];
ACCOUNT player1;
ACCOUNT player2;
int numOfPlayer;

DATA game;
/* ---------------- */

/* User Define Fuction - Declaration */
void error_handler(const char * message);

int setServer(const char * port);
int setClient(int server_socket);

void initVar(void);

void signIn(int client_socket);
void signOut(int client_socket);
void logIn(int client_socket);
void logOut(int client_socket1, int client_socket2);

void setGame(int client_socket1, int client_socket2);
void move(int client_socket1, int client_socket2);
void checkPoint(int client_socket1, int client_socket2);
void checkWinner(int client_socket1, int client_socket2);
void regame(int client_socket1, int client_socket2);
void myExit(int client_socket1, int client_socket2);

int setDB(void);
int checkDB(INPUT input);
void makeData(INPUT input);
void updateData(ACCOUNT player);
void delData(const char * ID);
/* ---------------- */

int main(int argc, char * argv[])
{
    int i = 0, j = 0;
    int check, menu;
    int server_socket;
    int client_socket[2];
    fd_set read_fds;

    if (argc != 2)
    {
        printf(">>> Usage : %s <Port>\n", argv[0]);

        exit(1);
    }

    else
    {
        server_socket = setServer(argv[1]);

        for (i = 0; i < 2; i++)
        {
            printf(">>> Wait for client...\n");
            client_socket[i] = setClient(server_socket);

            printf(">>> client %d was connected!\n", i + 1);
        }
    }

    setDB();
    initVar();

    while (1)
    {
        FD_ZERO(&read_fds);
        FD_SET(client_socket[0], &read_fds);
        FD_SET(client_socket[1], &read_fds);

        check = select(client_socket[1] + 1, &read_fds, NULL, NULL, NULL);

        switch (check)
        {
        case -1:
            error_handler(">>> select() error!");
    
        default:
            for (i = 0; i < 2; i++)
            {
                if (FD_ISSET(client_socket[i], &read_fds))
                {
                    recv(client_socket[i], &menu, sizeof(int), 0);
        
                    switch (menu)
                    {
                    case 1:
                        printf(">>> client %d requested SIGN IN : ", i + 1);
                        signIn(client_socket[i]);
                        break;
            
                    case 2:
                        printf(">>> client %d requested SIGN OUT : ", i + 1);
                        signOut(client_socket[i]);
                        break;
            
                    case 3:
                        printf(">>> client %d requested LOGIN : ", i + 1);
                        logIn(client_socket[i]);
                                
                        switch (numOfPlayer)
                        {
                        case 2:
                            for (j = 0; j < 2; j++)
                            {
                                send(client_socket[j], &numOfPlayer, sizeof(int), 0);
                                send(client_socket[j], &player1, sizeof(ACCOUNT), 0);
                                send(client_socket[j], &player2, sizeof(ACCOUNT), 0);
                            }
                            break;
                        }
            
                        break;
            
                    case 4:
                        printf(">>> client %d requested LOGOUT : ", i + 1);
                        logOut(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 5:
                        printf(">>> client %d requested GAME : ", i + 1);
                        setGame(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 6:
                        move(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 7:
                        checkPoint(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 9:
                        printf(">>> client %d requested REGAME!\n", i + 1);
                        regame(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 10:
                        printf(">>> client %d requested EXIT!\n", i + 1);
                        myExit(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
            
                    case 11:
                        checkWinner(client_socket[i], client_socket[(i + 1) % 2]);
                        break;
                    }
                }
            }
            break;
        }
    }

    return 0;
}

/* User Define Fuction - Definition */
void error_handler(const char * message)
{
    fputs(message, stderr);
    printf("\n");

    exit(1);
}

int setServer(const char * port)
{
    int server_socket;
    struct sockaddr_in server_address;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
        error_handler(">>> socket() error!");

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(atoi(port));

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        error_handler(">>> bind() error!");

    if (listen(server_socket, 5) == -1)
        error_handler(">>> listen() error!");

    return server_socket;
}

int setClient(int server_socket)
{
    int client_socket;
    int client_address_size;
    struct sockaddr_in client_address;

    memset(&client_address, 0, sizeof(client_address));
    client_address_size = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);

    if (client_socket == -1)
        error_handler(">>> accpet() error!");

    else
        return client_socket;
}

void initVar(void)
{
    memset(&player1, 0, sizeof(ACCOUNT));
    memset(&player2, 0, sizeof(ACCOUNT));
    numOfPlayer = 0;
}


void signIn(int client_socket)
{
    int result;
    INPUT input;

    memset(&input, 0x00, sizeof(INPUT));
    recv(client_socket, &input, sizeof(input), 0);

    switch (checkDB(input))
    {
    case -1:
        printf("Fail!\n");
        result = -1;
        send(client_socket, &result, sizeof(int), 0);
        break;
    
    case 0:
        makeData(input);
        printf("Success!\n");
        result = 0;
        send(client_socket, &result, sizeof(int), 0);
        break;
            
    case 1: case 2:
        printf("Fail!\n");
        result = 1;
        send(client_socket, &result, sizeof(int), 0);
        break;
    }
}

void signOut(int client_socket)
{
    int result;
    INPUT input;

    memset(&input, 0x00, sizeof(INPUT));
    recv(client_socket, &input, sizeof(input), 0);

    switch (checkDB(input))
    {
    case -1:
        printf("Fail!\n");
        result = -1;
        send(client_socket, &result, sizeof(int), 0);
        break;
    
    case 0: case 1:
        printf("Fail!\n");
        result = 0;
        send(client_socket, &result, sizeof(int), 0);
        break;
            
    case 2:
        delData(input.ID);
        printf("Success!\n");
        result = 2;
        send(client_socket, &result, sizeof(int), 0);
        break;
    }
}

void logIn(int client_socket)
{
    int fd;
    int result;
    char temp[4];
    INPUT input;

    memset(temp, 0x00, sizeof(temp));
    memset(&input, 0x00, sizeof(INPUT));
    recv(client_socket, &input, sizeof(input), 0);

    switch (checkDB(input))
    {
    case -1:
        printf("Fail!\n");
        result = -1;
        send(client_socket, &result, sizeof(int), 0);
        break;
    
    case 0: case 1:
        printf("Fail!\n");
        result = 0;
        send(client_socket, &result, sizeof(int), 0);
        break;
            
    case 2:
        switch (numOfPlayer)
        {
        case 0:
            strcpy(player1.ID, input.ID);

            chdir(currentDIR);
            chdir("DB");
            chdir(player1.ID);

            fd = open("WIN", O_RDONLY);
            read(fd, temp, sizeof(temp));
            player1.WIN = atoi(temp);
            memset(temp, 0x00, sizeof(temp));
            close(fd);

            fd = open("LOSE", O_RDONLY);
            read(fd, temp, sizeof(temp));
            player1.LOSE = atoi(temp);
            memset(temp, 0x00, sizeof(temp));
            close(fd);

            printf("Success!\n");
            result = 2;
            send(client_socket, &result, sizeof(int), 0);

            numOfPlayer = 1;
            send(client_socket, &numOfPlayer, sizeof(int), 0);

            break;

        case 1:
            if (strcmp(player1.ID, input.ID) == 0)
            {
                printf("Fail!\n");
                result = 1;
                send(client_socket, &result, sizeof(int), 0);
            }

            else
            {
                strcpy(player2.ID, input.ID);

                chdir(currentDIR);
                chdir("DB");
                chdir(player2.ID);

                fd = open("WIN", O_RDONLY);
                read(fd, temp, sizeof(temp));
                player2.WIN = atoi(temp);
                memset(temp, 0x00, sizeof(temp));
                close(fd);

                fd = open("LOSE", O_RDONLY);
                read(fd, temp, sizeof(temp));
                player2.LOSE = atoi(temp);
                memset(temp, 0x00, sizeof(temp));
                close(fd);
    
                printf("Success!\n");
                result = 2;
                send(client_socket, &result, sizeof(int), 0);
    
                numOfPlayer = 2;
                send(client_socket, &numOfPlayer, sizeof(int), 0);
            }

            break;
        }
    }
}

void logOut(int client_socket1, int client_socket2)
{
    int request;
    int result;

    printf("Waiting... ");
    recv(client_socket2, &request, sizeof(int), 0);

    memset(&player1, 0, sizeof(ACCOUNT));
    memset(&player2, 0, sizeof(ACCOUNT));
    numOfPlayer = 0;
    printf("Success!\n");

    result = 0;
    send(client_socket1, &result, sizeof(int), 0);
    send(client_socket2, &result, sizeof(int), 0);
}

void setGame(int client_socket1, int client_socket2)
{
    int request;
    int result;

    printf("Waiting... ");
    recv(client_socket2, &request, sizeof(int), 0);

    printf("Success!\n");
    result = 0;
    send(client_socket1, &result, sizeof(int), 0);
    send(client_socket2, &result, sizeof(int), 0);

    memset(&game, 0, sizeof(DATA));

    game.dataOfStone[2][2] = 1;
    game.dataOfStone[2][3] = 2;
    game.dataOfStone[3][2] = 2;
    game.dataOfStone[3][3] = 1;

    game.dataOfCursor[2][2] = 1;

    game.countP1 = 2;
    game.countP2 = 2;
    game.cursorOfMap = 14;

    send(client_socket1, &game, sizeof(DATA), 0);
    send(client_socket2, &game, sizeof(DATA), 0);
}

void move(int client_socket1, int client_socket2)
{
    memset(&game, 0, sizeof(DATA));
    recv(client_socket1, &game, sizeof(DATA), 0);
    send(client_socket1, &game, sizeof(DATA), 0);
    send(client_socket2, &game, sizeof(DATA), 0);
}

void checkPoint(int client_socket1, int client_socket2)
{
    int checkDirY[8] = { -1, -1, 0, 1, 1, 1, 0, -1};
    int checkDirX[8] = { 0, 1, 1, 1, 0, -1, -1, -1};
    int y, x;
    int i, j;
    int count = 0;
    int result;

    memset(&game, 0, sizeof(DATA));
    recv(client_socket1, &game, sizeof(DATA), 0);

    switch (game.dataOfStone[game.cursorOfMap / 6][game.cursorOfMap % 6])
    {
    case 1: case 2:
        result = 0;
        send(client_socket1, &result, sizeof(int), 0);
        break;
    }

    for (i = 0; i < 8; i++)
    {
        y = game.cursorOfMap / 6;
        x = game.cursorOfMap % 6;

        for (j = 0; j < 5; j++)
        {
            y += checkDirY[i];
            x += checkDirX[i];

            if (y < 0 || y > 5 || x < 0 || x > 5 || game.dataOfStone[y][x] == 0)
                break;

            else
            {
                if (game.dataOfStone[y][x] == ((game.turn % 2) + 1))
                {
                    for ( ; j > 0; j--)
                    {
                        y -= checkDirY[i];
                        x -= checkDirX[i];
                        game.dataOfStone[y][x] = ((game.turn % 2) + 1);
                        count += 1;
            
                        switch (game.turn % 2)
                        {
                        case 0:
                            game.countP2 -= 1;
                            game.countP1 += 1;
                            break;
                
                        case 1:
                            game.countP1 -= 1;
                            game.countP2 += 1;
                            break;
                        }
                    }
        
                    break;
                }
            }
        }
    }

    switch (count)
    {
    case 0:
        result = 0;
        send(client_socket1, &result, sizeof(int), 0);
        break;

    default:
        result = 1;
        send(client_socket1, &result, sizeof(int), 0);
        send(client_socket1, &game, sizeof(DATA), 0);
        break;
    }
}

void checkWinner(int client_socket1, int client_socket2)
{
    memset(&game, 0, sizeof(DATA));
    recv(client_socket1, &game, sizeof(DATA), 0);

    if (game.countP1 + game.countP2 == 36)
    {
        if (game.countP1 > game.countP2)
        {
            game.winner = 1;
            player1.WIN += 1;
            updateData(player1);

            player2.LOSE += 1;
            updateData(player2);
        }

        else if (game.countP1 < game.countP2)
        {
            game.winner = 2;
            player2.WIN += 1;
            updateData(player2);

            player1.LOSE += 1;
            updateData(player1);
        }
        else
            game.winner = 3;
    }

    else
    {
        if (game.countP1 == 0)
        {
            game.winner = 2;
            player2.WIN += 1;
            updateData(player2);

            player1.LOSE += 1;
            updateData(player1);
        }

        else if (game.countP2 == 0)
        {
            game.winner = 1;
            player1.WIN += 1;
            updateData(player1);

            player2.LOSE += 1;
            updateData(player2);
        }
    }

    send(client_socket1, &game, sizeof(DATA), 0);
    send(client_socket2, &game, sizeof(DATA), 0);
}

void regame(int client_socket1, int client_socket2)
{
    memset(&game, 0, sizeof(DATA));

    game.dataOfStone[2][2] = 1;
    game.dataOfStone[2][3] = 2;
    game.dataOfStone[3][2] = 2;
    game.dataOfStone[3][3] = 1;

    game.dataOfCursor[2][2] = 1;

    game.countP1 = 2;
    game.countP2 = 2;
    game.cursorOfMap = 14;

    send(client_socket1, &game, sizeof(DATA), 0);
    send(client_socket2, &game, sizeof(DATA), 0);
}

void myExit(int client_socket1, int client_socket2)
{
    int request;

    memset(&game, 0, sizeof(DATA));

    game.winner = -1;
    send(client_socket2, &game, sizeof(DATA), 0);

    initVar();
    memset(&game, 0, sizeof(DATA));
}

int setDB(void)
{
    memset(currentDIR, 0x00, sizeof(currentDIR));
    getcwd(currentDIR, 100);

    if (mkdir("DB", PERMS) == -1)
        return 1;

    return 0;
}

int checkDB(INPUT input)
{
    struct dirent * direntp;
    DIR * dirp;
    int fd;
    char temp[16];

    chdir(currentDIR);

    if ((dirp = opendir("DB")) == NULL)
        return -1;

    while ((direntp = readdir(dirp)) != NULL)
    {
        if (strcmp(input.ID, direntp->d_name) == 0)
        {
            chdir("DB");
            chdir(input.ID);

            fd = open("PW", O_RDONLY);

            memset(temp, 0x00, sizeof(temp));
            read(fd, temp, strlen(input.PW));

            if (strcmp(input.PW, temp) == 0)
            {
                close(dirp);
    
                return 2;
            }

            close(dirp);

            return 1;
        }

        else
            continue;
    }

    close(dirp);

    return 0;
}

void makeData(INPUT input)
{
    ACCOUNT newAcc;
    int fd;
    char temp[4];

    memset(&newAcc, 0, sizeof(ACCOUNT));
    memset(temp, 0x00, sizeof(temp));

    strcpy(newAcc.ID, input.ID);
    strcpy(newAcc.PW, input.PW);
    newAcc.WIN = 0;
    newAcc.LOSE = 0;

    chdir(currentDIR);

    chdir("DB");
    mkdir(newAcc.ID, PERMS);

    chdir(newAcc.ID);

    fd = open("PW", O_RDWR | O_CREAT, PERMS);
    write(fd, newAcc.PW, strlen(newAcc.PW));

    sprintf(temp, "%d", newAcc.WIN);
    fd = open("WIN", O_RDWR | O_CREAT, PERMS);
    write(fd, temp, strlen(temp));
    memset(temp, 0x00, sizeof(temp));

    sprintf(temp, "%d", newAcc.LOSE);
    fd = open("LOSE", O_RDWR | O_CREAT, PERMS);
    write(fd, temp, strlen(temp));
    memset(temp, 0x00, sizeof(temp));
}

void updateData(ACCOUNT player)
{
    int fd;
    char temp[4];

    memset(temp, 0x00, sizeof(temp));

    chdir(currentDIR);
    chdir("DB");
    chdir(player.ID);

    sprintf(temp, "%d", player.WIN);
    fd = open("WIN", O_RDWR | O_CREAT | O_TRUNC, PERMS);
    write(fd, temp, strlen(temp));
    memset(temp, 0x00, sizeof(temp));

    sprintf(temp, "%d", player.LOSE);
    fd = open("LOSE", O_RDWR | O_CREAT | O_TRUNC, PERMS);
    write(fd, temp, strlen(temp));
    memset(temp, 0x00, sizeof(temp));
}

void delData(const char * ID)
{
    chdir(currentDIR);
    chdir("DB");
    chdir(ID);

    remove("PW");
    remove("WIN");
    remove("LOSE");

    chdir("..");

    remove(ID);
}
/* ------------------- */
