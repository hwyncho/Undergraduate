/*
 * Name : HwiYeon Cho
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:28
 */
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <ncurses.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define HLINE LINES / 4 * 3
#define VLINE COLS / 4 * 3
#define PERMS 0777

typedef struct _coordinate
{
    int y;
    int x;
} COORDINATE;

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

typedef struct _windows
{
    WINDOW * Up;
    WINDOW * Down;
} WINDOWS;

/* Global Variables */
int client_socket;

int cursorOfButton;

char maskedPW[16];
INPUT input;

int player;
ACCOUNT player1;
ACCOUNT player2;

DATA game;

WINDOWS menuScreen;
WINDOWS gameScreen;
/* ---------------- */

/* User Define Fuction - Declaration */
void error_handler(const char * message);

void setClient(const char * ip, const char * port);

void initVar(void);
int setScr(void);
void clearScr(WINDOWS pScreen);
void getPW(WINDOW * pWindow);

void mainMenu(WINDOWS pScreen, int pCursor);
int mainMenuKey(WINDOW * pWindow);

void signInOut(WINDOWS pScreen, int pCursor);
int signInOutKey(WINDOW * pWindow);

void signIn(WINDOWS pScreen, int pCursor);
int signInKey(WINDOW * pWindow);

void signOut(WINDOWS pScreen, int pCursor);
int signOutKey(WINDOW * pWindow);

void logIn(WINDOWS pScreen, int pCursor);
int logInKey(WINDOW * pWindow);

void waitPlayer(WINDOWS pScreen);
void waitRoom(WINDOWS pScreen, int pCursor);
int waitRoomKey(WINDOW * pWindow);

void setGame(void);
void printMap(WINDOW * pWindow);
void play(WINDOWS pScreen, int pCursor);
int playKey(WINDOW * pWindow);
int playMenuKey(WINDOW * pWindow);
int checkPoint(WINDOW * pWindow);
int checkWinner(void);
/* ------------------- */

/* main Fuction */
void main(int argc, char * argv[])
{
    if (argc != 3)
    {
        printf(">>> Usage : %s <IP> <Port>\n", argv[0]);

        exit(1);
    }

    else
    {
        setClient(argv[1], argv[2]);

        initVar();
        setScr();
        mainMenu(menuScreen, 0);
    }
}
/* ------------ */

/* User Define Fuction - Definition */
void error_handler(const char * message)
{
    fputs(message, stderr);
    printf("\n");

    exit(1);
}

void setClient(const char * ip, const char * port)
{
    struct sockaddr_in client_address;

    client_socket = socket(PF_INET, SOCK_STREAM, 0);

    if (client_socket == -1)
        error_handler(">>> socket() error!");

    memset(&client_address, 0, sizeof(client_address));
    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = inet_addr(ip);
    client_address.sin_port = htons(atoi(port));

    if (connect(client_socket, (struct sockaddr *)&client_address, sizeof(client_address)) == -1)
        error_handler(">>> connect() error!");
}

void initVar(void)
{
    cursorOfButton = 0;
    memset(&maskedPW, 0x00, sizeof(maskedPW));
    memset(&input, 0, sizeof(INPUT));
    memset(&player1, 0, sizeof(ACCOUNT));
    memset(&player2, 0, sizeof(ACCOUNT));
}

int setScr(void)
{
    initscr();

    if (has_colors() == FALSE)
    {
        printf(">>> Terminal does not support colors!\n");
        endwin();
        exit(1);
    }

    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    keypad(stdscr, TRUE);
    curs_set(0);
    noecho();

    refresh();

    menuScreen.Up = newwin(HLINE, COLS, 0, 0);
    menuScreen.Down = newwin(LINES / 4, COLS, HLINE, 0);

    wbkgd(menuScreen.Up, COLOR_PAIR(1));
    wbkgd(menuScreen.Down, COLOR_PAIR(2));

    wrefresh(menuScreen.Up);
    wrefresh(menuScreen.Down);

    return 0;
}

void clearScr(WINDOWS pScreen)
{
    werase(pScreen.Up);
    werase(pScreen.Down);
}

void getPW(WINDOW * pWindow)
{
    int i = 0;
    int ch;

    while (i < 16)
    {
        noecho();
        
        ch = wgetch(pWindow);
        
        if (ch == 10)
        {
            input.PW[i] = '\n';
            maskedPW[i] = '\n';
            break;
        }
        
        else if (ch == 127)
        {
            i--;
            
            if (i < 0)
                i == 0;
                
            wprintw(pWindow, "\b \b");
        }
        
        else
        {
            input.PW[i] = (char)ch;
            maskedPW[i] = '*';
            i++;
    
            wprintw(pWindow, "*");
        }
        
        wrefresh(pWindow);
    }
    
    input.PW[i] = '\n';
    maskedPW[i] = '\n';
}

void mainMenu(WINDOWS pScreen, int pCursor)
{
    char * title[] = { "Software Practice 2", "OTHELLO", "2014726096", "HwiYeon Cho" };
    char * button[] = { "LOGIN", "SIGN IN & OUT", "EXIT" };

    clearScr(pScreen);

    curs_set(0);
    noecho();

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title[0])) / 2, title[0]);
    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title[1])) / 2, title[1]);
    mvwprintw(pScreen.Up, HLINE - 4, COLS - (COLS / 6), title[2]);
    mvwprintw(pScreen.Up, HLINE - 2, COLS - (COLS / 6), title[3]);

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, ((COLS / 3 - strlen(button[0])) / 2), button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3) + ((COLS / 3 - strlen(button[1])) / 2), button[1]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3 * 2) + ((COLS / 3 - strlen(button[2])) / 2), button[2]);

    switch (pCursor)
    {
    case 0:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, ((COLS / 3 - strlen(button[0])) / 2), button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3) + ((COLS / 3 - strlen(button[1])) / 2), button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3 * 2) + ((COLS / 3 - strlen(button[2])) / 2), button[2]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    mainMenu(menuScreen, mainMenuKey(menuScreen.Down));
}

int mainMenuKey(WINDOW * pWindow)
{
    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 2;
        break;
    
    case KEY_DOWN:
        cursorOfButton -= 2;
        break;

    case KEY_LEFT:
        cursorOfButton -= 1;
        break;

    case KEY_RIGHT:
        cursorOfButton += 1;
        break;

    case 10:
        switch (cursorOfButton)
        {
        case 0:
            cursorOfButton = 0;
            logIn(menuScreen, 0);
    
        case 1:
            cursorOfButton = 0;
            signInOut(menuScreen, 0);
    
        case 2:
            close(client_socket);
            endwin();
            exit(0);
        }
    }

    if (cursorOfButton > 2)
        cursorOfButton -= 3;

    else if (cursorOfButton < 0)
        cursorOfButton += 3;

    return cursorOfButton;
}

void signInOut(WINDOWS pScreen, int pCursor)
{
    char title[] = "SIGN IN & OUT";
    char * button[] = { "SIGN IN", "SIGN OUT", "BACK" };

    clearScr(pScreen);

    curs_set(0);
    noecho();

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title)) / 2, title);

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, ((COLS / 3 - strlen(button[0])) / 2), button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3) + ((COLS / 3 - strlen(button[1])) / 2), button[1]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3 * 2) + ((COLS / 3 - strlen(button[2])) / 2), button[2]);

    switch (pCursor)
    {
    case 0:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, ((COLS / 3 - strlen(button[0])) / 2), button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3) + ((COLS / 3 - strlen(button[1])) / 2), button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 3 * 2) + ((COLS / 3 - strlen(button[2])) / 2), button[2]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    signInOut(menuScreen, signInOutKey(menuScreen.Down));
}

int signInOutKey(WINDOW * pWindow)
{
    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 2;
        break;

    case KEY_DOWN:
        cursorOfButton -= 2;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 0:
            cursorOfButton = 0;
            signIn(menuScreen, 0);
    
        case 1:
            cursorOfButton = 0;
            signOut(menuScreen, 0);

        case 2:
            cursorOfButton = 0;
            mainMenu(menuScreen, 0);
        }
    }

    if (cursorOfButton > 2)
        cursorOfButton -= 3;

    else if (cursorOfButton < 0)
        cursorOfButton += 3;

    return cursorOfButton;
}

void signIn(WINDOWS pScreen, int pCursor)
{
    COORDINATE blankID;
    COORDINATE blankPW;
    char title[] = "SIGN IN";
    char inputID[] = "ID :";
    char inputPW[] = "PASSWORD :";
    char * button[] = { "SIGN IN", "BACK" };

    clearScr(pScreen);

    curs_set(1);
    echo();

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title)) / 2, title);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 - 5, inputID);
    getyx(pScreen.Up, blankID.y, blankID.x);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 , input.ID);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2 - 11, inputPW);
    getyx(pScreen.Up, blankPW.y, blankPW.x);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2, maskedPW);

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
    wrefresh(pScreen.Down);

    switch (pCursor)
    {
    case 0:
        mvwgetstr(pScreen.Up, blankID.y, blankID.x + 1, input.ID);
        wmove(pScreen.Up, blankPW.y, blankPW.x + 1);
        getPW(pScreen.Up);

        cursorOfButton = 1;
        signIn(menuScreen, 1);
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    signIn(menuScreen, signInKey(menuScreen.Down));
}

int signInKey(WINDOW * pWindow)
{
    int menu;
    int result;

    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 1;
        break;
    
    case KEY_DOWN:
        cursorOfButton -= 1;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 1:
            menu = 1;
            send(client_socket, &menu, sizeof(int), 0);
            send(client_socket, &input, sizeof(INPUT), 0);
            recv(client_socket, &result, sizeof(int), 0);
    
            switch (result)
            {
            case -1:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Failed to access DB! (Press any key...)");
                wrefresh(pWindow);
            
                getch();
            
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                signIn(menuScreen, 0);
                
            case 0:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Welcome to OTHELLO World! (Press any key...)");
                wrefresh(pWindow);
            
                getch();
            
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                mainMenu(menuScreen, 0);
                
            case 1:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> %s has already exist in DB! (Please any key...)", input.ID);
                wrefresh(pWindow);
            
                getch();
            
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                signIn(menuScreen, 0);
            }
    
        case 2:
            memset(&input, 0, sizeof(INPUT));
            memset(maskedPW, 0x00, sizeof(maskedPW));
    
            cursorOfButton = 0;
            signInOut(menuScreen, 0);
        }
    }

    if (cursorOfButton > 2)
        cursorOfButton -= 2;

    else if (cursorOfButton < 1)
        cursorOfButton += 2;

    return cursorOfButton;
}

void signOut(WINDOWS pScreen, int pCursor)
{
    COORDINATE blankID;
    COORDINATE blankPW;
    char title[] = "SIGN OUT";
    char inputID[] = "ID :";
    char inputPW[] = "PASSWORD :";
    char * button[] = { "SIGN OUT", "BACK" };

    clearScr(pScreen);

    curs_set(1);
    echo();

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title)) / 2, title);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 - 5, inputID);
    getyx(pScreen.Up, blankID.y, blankID.x);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 , input.ID);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2 - 11, inputPW);
    getyx(pScreen.Up, blankPW.y, blankPW.x);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2, maskedPW);

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
    wrefresh(pScreen.Down);

    switch (pCursor)
    {
    case 0:
        mvwgetstr(pScreen.Up, blankID.y, blankID.x + 1, input.ID);
        wmove(pScreen.Up, blankPW.y, blankPW.x + 1);
        getPW(pScreen.Up);

        cursorOfButton = 1;
        signOut(menuScreen, 1);
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    signOut(menuScreen, signOutKey(menuScreen.Down));
}

int signOutKey(WINDOW * pWindow)
{
    int menu;
    int result;

    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 1;
        break;
    
    case KEY_DOWN:
        cursorOfButton -= 1;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 1:
            menu = 2;
            send(client_socket, &menu, sizeof(int), 0);
            send(client_socket, &input, sizeof(INPUT), 0);
            recv(client_socket, &result, sizeof(int), 0);
    
            switch (result)
            {
            case -1:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Failed to access DB! (Press any key...)");
                wrefresh(pWindow);
                
                getch();
                
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                signOut(menuScreen, 0);
                
            case 0:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Failed to sign out! (Press any key...)");
                wrefresh(pWindow);
                
                getch();
                
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                signOut(menuScreen, 0);
                
            case 2:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Goodbye %s! (Please any key...)", input.ID);
                wrefresh(pWindow);
                
                getch();
                
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                mainMenu(menuScreen, 0);
            }
    
        case 2:
            memset(&input, 0, sizeof(INPUT));
            memset(maskedPW, 0x00, sizeof(maskedPW));
    
            cursorOfButton = 0;
            signInOut(menuScreen, 0);
        }
    }

    if (cursorOfButton > 2)
        cursorOfButton -= 2;

    else if (cursorOfButton < 1)
        cursorOfButton += 2;

    return cursorOfButton;
}

void logIn(WINDOWS pScreen, int pCursor)
{
    COORDINATE blankID;
    COORDINATE blankPW;
    char title[] = "LOGIN";
    char inputID[] = "ID :";
    char inputPW[] = "PASSWORD :";
    char * button[] = { "LOGIN", "BACK" };

    clearScr(pScreen);

    curs_set(1);
    echo();

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title)) / 2, title);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 - 5, inputID);
    getyx(pScreen.Up, blankID.y, blankID.x);

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2 , input.ID);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2 - 11, inputPW);
    getyx(pScreen.Up, blankPW.y, blankPW.x);

    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS - strlen(title)) / 2, maskedPW);

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
    wrefresh(pScreen.Down);

    switch (pCursor)
    {
    case 0:
        mvwgetstr(pScreen.Up, blankID.y, blankID.x + 1, input.ID);
        wmove(pScreen.Up, blankPW.y, blankPW.x + 1);
        getPW(pScreen.Up);

        cursorOfButton = 1;
        logIn(menuScreen, 1);
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    logIn(menuScreen, logInKey(menuScreen.Down));
}

int logInKey(WINDOW * pWindow)
{
    int menu;
    int result;
    int fd;
    char temp[4];
 
    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 1;
        break;
    
    case KEY_DOWN:
        cursorOfButton -= 1;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 1:
            menu = 3;
            send(client_socket, &menu, sizeof(int), 0);
            send(client_socket, &input, sizeof(INPUT), 0);
            recv(client_socket, &result, sizeof(int), 0);
    
            switch (result)
            {
            case -1:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Failed to access DB! (Press any key...)");
                wrefresh(pWindow);
                
                getch();
                
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                logIn(menuScreen, 0);
                
            case 0:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> Failed to login! (Please any key...)");
                wrefresh(pWindow);
                
                getch();
                
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                logIn(menuScreen, 0);
    
            case 1:
                mvwprintw(pWindow, (LINES - HLINE) / 2 + 2, 0, ">>> %s has already joined! (Please any key...)", input.ID);
                wrefresh(pWindow);

                getch();

                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
                
                cursorOfButton = 0;
                logIn(menuScreen, 0);
    
            case 2:
                recv(client_socket, &player, sizeof(int), 0);
        
                memset(&input, 0, sizeof(INPUT));
                memset(maskedPW, 0x00, sizeof(maskedPW));
        
                cursorOfButton = 0;
                waitPlayer(menuScreen);
            }
        case 2:
            memset(&input, 0, sizeof(INPUT));
            memset(maskedPW, 0x00, sizeof(maskedPW));
    
            cursorOfButton = 0;
            mainMenu(menuScreen, 0);
        }
    }

    if (cursorOfButton > 2)
        cursorOfButton -= 2;

    else if (cursorOfButton < 1)
        cursorOfButton += 2;

    return cursorOfButton;
}

void waitPlayer(WINDOWS pScreen)
{
    char title[] = "WAITING ANOTHER PLAYER";
    char button[] = "Waiting...";
    int numOfPlayer;

    clearScr(pScreen);

    curs_set(0);
    noecho();

    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS - strlen(title)) / 2, title);
    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS - strlen(button)) / 2, button);
    wrefresh(pScreen.Down);

    recv(client_socket, &numOfPlayer, sizeof(int), 0);
        
    switch (numOfPlayer)
    {
    case 1:
        waitPlayer(menuScreen);

    case 2:
        recv(client_socket, &player1, sizeof(ACCOUNT), 0);
        recv(client_socket, &player2, sizeof(ACCOUNT), 0);

        cursorOfButton = 0;
        waitRoom(menuScreen, 0);
    }
}

void waitRoom(WINDOWS pScreen, int pCursor)
{
    char * title[] = { "BATTLE PAGE", "1P :", "2P :", "WIN :", "/ LOSE :" };
    char * button[] = { "PLAY", "BACK" };
    double odds1 = 0.0;
    double odds2 = 0.0;

    clearScr(pScreen);

    curs_set(0);
    noecho();

    if (player1.WIN > 0)
        odds1 = ((double)player1.WIN / (player1.WIN + player1.LOSE)) * 100;

    if (player2.WIN > 0)
        odds2 = ((double)player2.WIN / (player2.WIN + player2.LOSE)) * 100;

    mvwprintw(pScreen.Up, LINES / 4, (COLS - strlen(title[0])) / 2, title[0]);

    mvwprintw(pScreen.Up, LINES / 4 + 2, ((COLS / 2) - (strlen(title[1]) + 1 + strlen(player1.ID))) / 2, "%s %s", title[1], player1.ID);
    mvwprintw(pScreen.Up, LINES / 4 + 2, (COLS / 2) + ((COLS / 2) - (strlen(title[2]) + 1 + strlen(player2.ID))) / 2, "%s %s", title[2], player2.ID);

    mvwprintw(pScreen.Up, LINES / 4 + 4, ((COLS / 2) - (strlen(title[3]) + 1 + strlen(title[4]) + 1)) / 2, "%s %d %s %d", title[3], player1.WIN, title[4], player1.LOSE);
    mvwprintw(pScreen.Up, LINES / 4 + 4, (COLS / 2) + ((COLS / 2) - (strlen(title[3]) + 1 + strlen(title[4]) + 1)) / 2, "%s %d %s %d", title[3], player2.WIN, title[4], player2.LOSE);

    mvwprintw(pScreen.Up, LINES / 4 + 5, ((COLS / 2) - 6) / 2, "(%d%c)", (int)odds1, '%');
    mvwprintw(pScreen.Up, LINES / 4 + 5, (COLS / 2) + ((COLS / 2) - 6) / 2, "(%d%c)", (int)odds2, '%');

    wrefresh(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
    mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);

    switch (pCursor)
    {
    case 0:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2 - strlen(button[0])) / 2, button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - HLINE) / 2, (COLS / 2) + (COLS / 2 - strlen(button[1])) / 2, button[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    waitRoom(menuScreen, waitRoomKey(menuScreen.Down));
}

int waitRoomKey(WINDOW * pWindow)
{
    int menu;
    int result;

    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton += 1;
        break;

    case KEY_DOWN:
        cursorOfButton -= 1;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 0:
            menu = 5; 
            send(client_socket, &menu, sizeof(int), 0);
    
            werase(pWindow);
            mvwprintw(pWindow, (LINES - HLINE) / 2, (COLS - strlen("Waiting...")) / 2, "Waiting...");
            wrefresh(pWindow);
            recv(client_socket, &result, sizeof(int), 0);
    
            switch (result)
            {
            case 0:
                cursorOfButton = 0;
                setGame();
                play(gameScreen, 0);
            }
    
        case 1:
            menu = 4; 
            send(client_socket, &menu, sizeof(int), 0);
    
            werase(pWindow);
            mvwprintw(pWindow, (LINES - HLINE) / 2, (COLS - strlen("Waiting...")) / 2, "Waiting...");
            wrefresh(pWindow);
            recv(client_socket, &result, sizeof(int), 0);
    
            switch (result)
            {
            case 0:
                memset(&player1, 0, sizeof(ACCOUNT));
                memset(&player2, 0, sizeof(ACCOUNT));
        
                cursorOfButton = 0;
                mainMenu(menuScreen, 0);
            }
        }
    }

    if (cursorOfButton > 1)
        cursorOfButton -= 2;

    else if (cursorOfButton < 0)
        cursorOfButton += 2;

    return cursorOfButton;
}

void setGame(void)
{
    curs_set(0);
    noecho();

    refresh();

    gameScreen.Up = newwin(LINES, VLINE, 0, 0);
    gameScreen.Down = newwin(LINES, COLS / 4, 0, VLINE);

    wbkgd(gameScreen.Up, COLOR_PAIR(1));
    wbkgd(gameScreen.Down, COLOR_PAIR(2));

    wrefresh(gameScreen.Up);
    wrefresh(gameScreen.Down);

    memset(&game, 0, sizeof(DATA));
}

void printMap(WINDOW * pWindow)
{
    int i, j;
    char border[] = "+---+---+---+---+---+---+";

    for (i = 0; i < 6; i++)
    {
        mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i), (VLINE - strlen(border)) / 2, border);

        for (j = 0; j < 6; j++)
        {
            mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j), "|");
    
            switch (game.dataOfCursor[i][j])
            {
            case 0:
                switch (game.dataOfStone[i][j])
                {
                case 0:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, "   ");
                    break;
            
                case 1:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, " O ");
                    break;
            
                case 2:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, " X ");
                    break;
                }
                break;
        
            case 1:
                wattron(pWindow, COLOR_PAIR(2));
        
                switch (game.dataOfStone[i][j])
                {
                case 0:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, "   ");
                    break;
            
                case 1:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, " O ");
                    break;
            
                case 2:
                    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j) + 1, " X ");
                    break;
                }
        
                wattroff(pWindow, COLOR_PAIR(2));
                break;
            }
        }

        mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i) + 1, (VLINE - strlen(border)) / 2 + (4 * j), "|");
    }

    mvwprintw(pWindow, (LINES - 13) / 2 + (2 * i), (VLINE - strlen(border)) / 2, border);

    wrefresh(pWindow);
}

void play(WINDOWS pScreen, int pCursor)
{
    int menu;
    char * title[] = { "1P(O)", "2P(X)" };
    char * button[] = { "NEXT TURN", "1P WIN!", "2P WIN!", "DRAW!", "REGAME", "EXIT" };
    int request = 1;

    clearScr(pScreen);

    curs_set(0);
    noecho();

    memset(&game, 0, sizeof(DATA));
    recv(client_socket, &game, sizeof(DATA), 0);

    printMap(pScreen.Up);

    mvwprintw(pScreen.Down, (LINES - 8) / 2, ((COLS / 4) - (strlen(title[0]) + 5)) / 2, "%s : %d", title[0], game.countP1);
    mvwprintw(pScreen.Down, (LINES - 8) / 2 + 1, ((COLS / 4) - (strlen(title[1]) + 5)) / 2, "%s : %d", title[1], game.countP2);

    wrefresh(pScreen.Down);

    switch (game.winner)
    {
    case -1:
        memset(&player1, 0, sizeof(ACCOUNT));
        memset(&player2, 0, sizeof(ACCOUNT));
    
        cursorOfButton = 0;
        mainMenu(menuScreen, 0);

    case 0:
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[0])) / 2, button[0]);
        
        wattron(pScreen.Down, A_UNDERLINE);
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[0])) / 2, "%c", button[0][0]);
        wattroff(pScreen.Down, A_UNDERLINE);
        break;

    case 1:
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[1])) / 2, button[1]);
        break;

    case 2:
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[1])) / 2, button[2]);
        break;

    case 3:
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[1])) / 2, button[3]);
        break;
    }

    mvwprintw(pScreen.Down, (LINES - 8) / 2 + 6, ((COLS / 4) - strlen(button[1])) / 2, button[4]);
    mvwprintw(pScreen.Down, (LINES - 8) / 2 + 8, ((COLS / 4) - strlen(button[2])) / 2, button[5]);

    wattron(pScreen.Down, A_UNDERLINE);
    mvwprintw(pScreen.Down, (LINES - 8) / 2 + 6, ((COLS / 4) - strlen(button[1])) / 2, "%c", button[4][0]);
    mvwprintw(pScreen.Down, (LINES - 8) / 2 + 8, ((COLS / 4) - strlen(button[2])) / 2 + 1, "%c", button[5][1]);
    wattroff(pScreen.Down, A_UNDERLINE);

    switch (game.turn % 2)
    {
    case 0:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - 8) / 2, ((COLS / 4) - (strlen(title[0]) + 5)) / 2, title[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 1, ((COLS / 4) - (strlen(title[1]) + 5)) / 2, title[1]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    switch (pCursor)
    {
    case 0:
        switch (game.winner)
        {
        case 0: case 3:
            wrefresh(pScreen.Down);
    
            if ((game.turn % 2 + 1) == player)
                play(gameScreen, playKey(gameScreen.Up));
        
            else
                play(gameScreen, 0);
    
        case 1: case 2:
            wrefresh(pScreen.Down);
    
            if (game.winner == player)
                play(gameScreen, 0);
    
            else
            {
                game.dataOfCursor[game.cursorOfMap / 6][game.cursorOfMap % 6] = 0;
                menu = 6;
                send(client_socket, &menu, sizeof(menu), 0);
                send(client_socket, &game, sizeof(DATA), 0);
        
                cursorOfButton = 2;
                play(gameScreen, 2);
            }
        }
    
    case 1:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 4, ((COLS / 4) - strlen(button[0])) / 2, button[0]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;

    case 2:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 6, ((COLS / 4) - strlen(button[1])) / 2, button[4]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;

    case 3:
        wattron(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        mvwprintw(pScreen.Down, (LINES - 8) / 2 + 8, ((COLS / 4) - strlen(button[2])) / 2, button[5]);
        wattroff(pScreen.Down, A_UNDERLINE | COLOR_PAIR(1));
        break;
    }

    wrefresh(pScreen.Down);

    play(gameScreen, playMenuKey(gameScreen.Down));
}

int playKey(WINDOW * pWindow)
{
    int menu;
    int beforeCursor = game.cursorOfMap;
    int result;

    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        game.cursorOfMap -= 6;
        break;
    
    case KEY_DOWN:
        game.cursorOfMap += 6;
        break;
    
    case KEY_LEFT:
        game.cursorOfMap -= 1;
        break;
    
    case KEY_RIGHT:
        game.cursorOfMap += 1;
        break;
    
    case 10:
        menu = 7;
        send(client_socket, &menu, sizeof(menu), 0);
        send(client_socket, &game, sizeof(DATA), 0);
        recv(client_socket, &result, sizeof(int), 0);
        switch (result)
        {
        case 0:
            mvwprintw(gameScreen.Down, LINES - 1, 0, ">>> Cannot select!");
            wrefresh(gameScreen.Down);
            sleep(1);
            break;

        case 1:
            memset(&game, 0, sizeof(DATA));
            recv(client_socket, &game, sizeof(DATA), 0);
    
            switch (game.turn % 2)
            {
            case 0:
                game.countP1 += 1;
                game.dataOfStone[game.cursorOfMap / 6][game.cursorOfMap % 6] = 1;
                game.turn += 1;
                break;
        
            case 1:
                game.countP2 += 1;
                game.dataOfStone[game.cursorOfMap / 6][game.cursorOfMap % 6] = 2;
                game.turn += 1;
                break;
            }
    
            menu = 11;
            send(client_socket, &menu, sizeof(int), 0);
            send(client_socket, &game, sizeof(DATA), 0);
    
            cursorOfButton = 0;
            play(gameScreen, 0);
        }
        break;

    case 'n':
        game.turn += 1;
        menu = 6;
        send(client_socket, &menu, sizeof(menu), 0);
        send(client_socket, &game, sizeof(DATA), 0);

        cursorOfButton = 0;
        play(gameScreen, 0);

    case 'r':
        game.dataOfCursor[game.cursorOfMap / 6][game.cursorOfMap % 6] = 0;
        menu = 6;
        send(client_socket, &menu, sizeof(menu), 0);
        send(client_socket, &game, sizeof(DATA), 0);

        cursorOfButton = 2;
        return cursorOfButton;

    case 'x':
        game.dataOfCursor[game.cursorOfMap / 6][game.cursorOfMap % 6] = 0;
        menu = 6;
        send(client_socket, &menu, sizeof(menu), 0);
        send(client_socket, &game, sizeof(DATA), 0);

        cursorOfButton = 3;
        return cursorOfButton;
    }

    if (game.cursorOfMap > 35)
        game.cursorOfMap -= 36;

    else if (game.cursorOfMap < 0)
        game.cursorOfMap += 36;

    game.dataOfCursor[beforeCursor / 6][beforeCursor % 6] = 0;
    game.dataOfCursor[game.cursorOfMap / 6][game.cursorOfMap % 6] = 1;

    menu = 6;
    send(client_socket, &menu, sizeof(menu), 0);
    send(client_socket, &game, sizeof(DATA), 0);

    return cursorOfButton;
}

int playMenuKey(WINDOW * pWindow)
{
    int menu;
    int request;

    keypad(pWindow, TRUE);

    switch (wgetch(pWindow))
    {
    case KEY_UP:
        cursorOfButton -= 1;
        break;
    
    case KEY_DOWN:
        cursorOfButton += 1;
        break;
    
    case KEY_LEFT:
        cursorOfButton -= 1;
        break;
    
    case KEY_RIGHT:
        cursorOfButton += 1;
        break;
    
    case 10:
        switch (cursorOfButton)
        {
        case 1:
            game.turn += 1;
            menu = 6;
            send(client_socket, &menu, sizeof(menu), 0);
            send(client_socket, &game, sizeof(DATA), 0);
    
            cursorOfButton = 0;
            play(gameScreen, 0);
    
        case 2:
            menu = 9;
            send(client_socket, &menu, sizeof(menu), 0);

            cursorOfButton = 0;
            play(gameScreen, 0);
    
        case 3:
            menu = 10;
            send(client_socket, &menu, sizeof(menu), 0);
    
            memset(&player1, 0, sizeof(ACCOUNT));
            memset(&player2, 0, sizeof(ACCOUNT));
            memset(&game, 0, sizeof(DATA));
    
            cursorOfButton = 0;
            mainMenu(menuScreen, 0);
        }
    }

    switch (game.winner)
    {
    case 0:
        if (cursorOfButton > 3)
            cursorOfButton -= 3;

        else if (cursorOfButton < 1)
            cursorOfButton += 3;

        break;

    case 1: case 2: case 3:
        if (cursorOfButton > 3)
            cursorOfButton -= 2;

        else if (cursorOfButton < 2)
            cursorOfButton += 2;

        break;
    }

    menu = 6;
    send(client_socket, &menu, sizeof(menu), 0);
    send(client_socket, &game, sizeof(DATA), 0);

    return cursorOfButton;
}
/* ------------------- */
