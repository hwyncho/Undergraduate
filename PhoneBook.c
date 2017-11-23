/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements phonebook using linked list.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:11
 */
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <Windows.h>

#define _CRT_SECURE_NO_WARNINGS

typedef struct _contacts
{
    char name[20];
    char number[20];
    struct _contacts *link;
} contacts;

void push(char tel[], char temp);
void pop(char tel[]);
contacts * createPhoneBook(void);
void loadPhonebook(void);     // 저장된 전화번호부 로드 함수 선언
int countContatcts(void);     // 전화번호부에 저장된 연락처의 수 카운트 함수 선언
void addNewContacts(void);    // 새 연락처 추가 함수 선언
void printPhoneBook(void);    // 전화번호부 출력 함수 선언
void searchContacts(void);    // 연락처 검색 함수 선언
void deleteContacts(void);    // 연락처 삭제 함수 선언
void autoCompletion(void);    // 자동완성 함수 선언
void game(void);              // 게임 함수 선언

const char * DB = "phonebook.txt";
contacts phonebook;
FILE * info;

int main(void)
{
    char menu;

    system("COLOR F0");
    system("mode con:cols=90 lines=31");

    /* 오프닝 */
    printf("■■■■■■■■■■                                                        \n");
    printf("■            ■  ■                                                        \n");
    printf("■  ■■■■  ■  ■                                                        \n");
    printf("■  ■    ■  ■  ■                                                        \n");
    printf("■  ■■■■  ■  ■■■■■  ■■■■■■■■■■■■■  ■■■■■■      \n");
    printf("■            ■          ■■■        ■■  ■      ■■■        ■■    \n");
    printf("■  ■■■■■■  ■■■■  ■  ■■■■  ■    ■■■  ■  ■■■■  ■    \n");
    printf("■  ■        ■  ■    ■  ■  ■    ■  ■  ■■  ■  ■  ■■■■  ■    \n");
    printf("■  ■        ■  ■    ■  ■  ■    ■  ■  ■    ■  ■            ■    \n");
    printf("■  ■        ■  ■    ■  ■  ■■■■  ■  ■    ■  ■  ■■■■■■    \n");
    printf("■  ■        ■  ■    ■  ■■        ■■  ■    ■  ■■          ■    \n");
    printf("■■■■■■  ■■■    ■■■■■■■■■■■■    ■■■■■■■■■■    \n");
    printf("■        ■■                            ■  ■                            \n");
    printf("■  ■■■  ■                            ■  ■                            \n");
    printf("■  ■  ■  ■                            ■  ■                            \n");
    printf("■  ■  ■  ■                            ■  ■    ■■■  ■■■■■■■■\n");
    printf("■  ■■■  ■                            ■  ■  ■■  ■  ■            ■\n");
    printf("■        ■■  ■■■■■■■■■■■■■■  ■■■  ■■  ■ 아무  키나 ■\n");
    printf("■  ■■■  ■■■        ■■■        ■■  ■■  ■■    ■            ■\n");
    printf("■  ■  ■■  ■  ■■■■  ■  ■■■■  ■  ■  ■■      ■    입력    ■\n");
    printf("■  ■    ■  ■  ■    ■  ■  ■    ■  ■    ■  ■■    ■            ■\n");
    printf("■  ■  ■■  ■  ■    ■  ■  ■    ■  ■  ■■■  ■■  ■   하세요   ■\n");
    printf("■  ■■■  ■■  ■■■■  ■  ■■■■  ■  ■  ■■  ■  ■   ......   ■\n");
    printf("■        ■■■■        ■■■        ■■  ■    ■  ■  ■            ■\n");
    printf("■■■■■■    ■■■■■■  ■■■■■■■■■    ■■■  ■■■■■■■■\n");

    if (_getche() != 0)
        system("cls");

    /* 메뉴 출력 및 선택 */
    do
    {
        printf("■■■■■■■■■■■■■■■■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■            ■■■            ■■■            ■\n");
        printf("■  (1)  연  락  처   등  록  ■ ■  1         ■■■  2         ■■■  3         ■\n");
        printf("■                            ■ ■            ■■■      ABC   ■■■      DEF   ■\n");
        printf("■■■■■■■■■■■■■■■■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■  (2)  연  락  처   출  력  ■ ■     ㄱㅋ   ■■■     ㄴㄹ   ■■■     ㄷㅌ   ■\n");
        printf("■                            ■ ■  4         ■■■  5         ■■■  6         ■\n");
        printf("■■■■■■■■■■■■■■■■ ■      GHI   ■■■      JKL   ■■■      MNO   ■\n");
        printf("■                            ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■  (3)  연  락  처   검  색  ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■     ㅂㅍ   ■■■     ㅅㅎ   ■■■     ㅈㅊ   ■\n");
        printf("■■■■■■■■■■■■■■■■ ■  7         ■■■  8         ■■■  9         ■\n");
        printf("■                            ■ ■     PQRS   ■■■      TUV   ■■■     WXYZ   ■\n");
        printf("■  (4)  연  락  처   삭  제  ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■■■■■■■■■■■■■■■■ ■            ■■■     ㅇㅁ   ■■■            ■\n");
        printf("■                            ■ ■  *         ■■■  0         ■■■  #         ■\n");
        printf("■  (5)  자   동    완   성   ■ ■            ■■■       +    ■■■            ■\n");
        printf("■                            ■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■■■■■■■■■■■■■■■■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■                                                ■\n");
        printf("■  (6)  미   니    게   임   ■ ■            ■■■            ■■■            ■\n");
        printf("■                            ■ ■                                                ■\n");
        printf("■■■■■■■■■■■■■■■■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
        printf("■                            ■ ■                                                ■\n");
        printf("■  (7)  종              료   ■ ■        원하는 메뉴의 번호를 입력하세요.        ■\n");
        printf("■                            ■ ■                                                ■\n");
        printf("■■■■■■■■■■■■■■■■ ■■■■■■■■■■■■■■■■■■■■■■■■■■\n");

        menu = _getch();
        fflush(stdin);

        system("cls");

        switch (menu)
        {
        case '1':
            printf(">>> 새 연락처를 등록합니다.\n\n");
            addNewContacts();
            break;

        case '2':
            printf(">>> [현재 등록된 연락처]\n");
            printPhoneBook();
            break;

        case '3':
            printf(">>> 검색하려는 연락처의 이름을 입력하세요.\n");
            searchContacts();
            break;

        case '4':
            printf(">>> 삭제하려는 연락처의 이름을 입력하세요.\n");
            deleteContacts();
            break;

        case '5':
            autoCompletion();
            break;

        case '6':
            game();
            break;

        case '7':
            printf(">>> 프로그램을 종료합니다.\n");
            break;

        default:
            printf(">>> 잘못된 값을 입력하였습니다.\n");
            break;
        }
    } while (menu != '7');

    return 0;
}

void push(char tel[], char tmp)
{
    int i = 0;

    while (tel[i] != 0)
        i++;

    tel[i] = tmp;
}

void pop(char tel[])
{
    int i = 0;

    while (tel[i] != 0)
        i++;

    if (i == 0)
        return;

    tel[i - 1] = 0;
}

contacts * createPhoneBook(void)
{
    contacts * tmp;
    tmp = (contacts *)malloc(sizeof(contacts));

    return tmp;
}

void loadPhoneBook(void)
{
    char name[20];
    char number[20];
    contacts * temp;

    info = fopen(DB, "r");

    temp = &phonebook;

    while (!feof(info))
    {
        if (fscanf(info, "%s", name) == EOF)
            break;

        fscanf(info, "%s", number);

        temp->link = createPhoneBook();
        temp = temp->link;

        strcpy(temp->name, name);
        strcpy(temp->number, number);

        temp->link = NULL;
    }
}

int countContacts(void)
{
    int count = 0;
    contacts * temp;

    info = fopen(DB, "r");
    if (info == NULL)
    {
        printf(">>> 파일이 열리지 않았습니다!\n");

        return 0;
    }

    loadPhoneBook();
    temp = &phonebook;

    while (temp->link != NULL)
    {
        temp = temp->link;
        count++;
    }

    return count;
}

void addNewContacts()
{
    char re;
    int res;
    contacts * temp, * head, * tail;
    
    head = tail = NULL;

    info = fopen(DB, "a");
    if (info == NULL)
    {
        printf(">>> 파일이 열리지 않았습니다!\n");

        return;
    }

    do
    {
        if ((temp = (contacts *)malloc(sizeof(contacts))) == NULL)
        {
            printf(">>> 메모리 할당에 실패했습니다!\n");

            return;
        }

        printf("<<< [이 름] : ");
        fflush(stdin);
        fgets(temp->name, sizeof(temp->name), stdin);

        printf("<<< [전화번호] : ");
        fflush(stdin);
        fgets(temp->number, sizeof(temp->number), stdin);

        printf(">>> 등록완료!\n");

        fprintf(info, "%s", temp->name);        // 파일에 이름 복사
        fprintf(info, "%s", temp->number);        // 파일에 전화번호 복사

        temp->link = NULL;

        if (head == NULL)
            head = tail = temp;

        tail->link = temp;
        tail = temp;

        while (1)
        {
            printf("\n>>> 계속 등록하시겠습니까? (Y / N) : ");
            re = getchar();

            if (re != 'Y' || re != 'y' || re != 'N' || re != 'n')
                break;

            puts("");
        }

        puts("");
    } while (re != 'N' && re != 'n');

    temp->link = NULL;
    printf("\n");

    res = fclose(info);
    if (res != 0)
    {
        printf(">>> 파일이 닫히지 않았습니다!\n");

        return;
    }
}

void printPhoneBook(void)
{
    int res;
    int cnt = 0;
    contacts * temp;

    info = fopen(DB, "r");
    if (info == NULL)
    {
        printf(">>> 파일이 열리지 않았습니다!\n");

        return;
    }

    printf("\n%-17s%-20s\n", "[이 름]", "[전화번호]");

    loadPhoneBook();
    temp = &phonebook;

    while ((temp = temp->link) != NULL)
        printf("\n%-17s%-20s", temp->name, temp->number);


    printf("\n\n>>> 메뉴로 돌아가려면 아무 키나 입력하세요.\n");
    if (_getch() != 0)
        system("cls");

    res = fclose(info);
    if (res != 0)
    {
        printf(">>> 파일이 닫히지 않았습니다!\n");

        return;
    }
}

void searchContacts(void)
{
    char Name[20];
    int count = 0;
    int res;
    contacts * temp;

    info = fopen(DB, "r");
    if (info == NULL)
    {
        printf(">>> 파일이 열리지 않았습니다!\n");

        return;
    }

    loadPhoneBook();    // 저장된 전화번호부 로드
    temp = &phonebook;

    printf("<<< 찾을 사람의 이름 : ");
    gets(Name);

    while (temp != NULL)
    {
        if (strcmp(Name, temp->name) == 0)
        {
            printf("\n%-17s%-20s\n", "[이 름]", "[전화번호]");
            printf("%-17s%-20s\n", temp->name, temp->number);
            count++;
            break;
        }

        temp = temp->link;
    }

    if (count == 0)
        printf("\n>>> 해당 연락처가 존재하지 않습니다!\n");

    printf("\n\n>>> 메뉴로 돌아가려면 아무 키나 입력하세요.\n");
    if (_getche() != 0)
        system("cls");

    res = fclose(info);
    if (res != 0)
    {
        printf(">>> 파일이 닫히지 않았습니다!\n");

        return;
    }
}

void deleteContacts(void)
{
    contacts * temp;
    contacts * tmp;
    char Name[15];

    printf("<<< 삭제할 사람의 성명 : ");
    gets(Name);

    loadPhoneBook();
    temp = &phonebook;

    while (temp->link != NULL)
    {
        // 해당 연락처가 존재할 경우
        if (strcmp(Name, temp->link->name) == 0)
        {
            tmp = temp->link;
            temp->link = (temp->link)->link;
            free(tmp);

            info = fopen(DB, "w");
            if (info == NULL)
            {
                printf(">>> 파일이 열리지 않았습니다!\n");

                return;
            }

            temp = &phonebook;

            while (temp != NULL)
            {
                fprintf(info, "%s\n", temp->name);
                fprintf(info, "%s\n", temp->number);
                temp = temp->link;
            }

            fclose(info);

            printf("\n>>> %s 님의 연락처가 삭제되었습니다!\n", Name);

            printf("\n\n>>> 메뉴로 돌아가려면 아무 키나 입력하세요.");
            if (_getch() != 0)
            {
                system("cls");

                return;
            }
        }

        temp = temp->link;
    }

    // 해당 연락처가 존재하지 않을 경우
    printf(">>> 해당 연락처가 존재하지 않습니다!\n");

    printf("\n>>> 메뉴로 돌아가시려면 아무 키나 입력하세요.");
    if (_getch() != 0)
        system("cls");
}

void autoCompletion()
{
    char tmp;
    char in[18] = "";
    contacts * temp;

    while (1)
    {
        system("cls");
        printf("<<< 이름 또는 전화번호를 입력하세요 : %s\n", in);
        printf("\n%-17s", "[이 름]");
        printf("%-20s\n", "[전화번호]");

        loadPhoneBook();
        temp = &phonebook;

        while (temp != NULL)
        {
            if (strstr(temp->name, in) != NULL || strstr(temp->number, in) != NULL)
            {
                printf("%-17s%-20s\n", temp->name, temp->number);
            }

            temp = temp->link;
        }

        tmp = _getche();

        if (tmp == 8)    // Backspace 키를 눌렀을 경우
        {
            pop(in);
            continue;
        }

        if (tmp == 9)    // Tab 키를 눌렀을 경우 함수 종료
        {
            system("cls");

            return;
        }

        push(in, tmp);
    }
}

void game(void)
{
    int i;
    int j = countContacts();
    int rand_num;    // 난수를 저장할 변수
    int count = 0;    // 틀린 횟수를 저장할 변수
    int re;
    char Num[20];
    contacts * temp;

    info = fopen(DB, "r");
    if (info == NULL)
    {
        printf(">>> 파일이 열리지 않았습니다!\n");

        return;
    }

    loadPhoneBook();

    //게임 시작
    printf(">>> 게임을 시작합니다!\n\n");

    do
    {
        srand((unsigned int)time(NULL));
        rand_num = rand() % j + 1;    // rand_num의 범위 : 1 ~ j

        temp = &phonebook;            // temp가 가리키는 위치를 전화번호부의 제일 앞으로 초기화

        for (i = 0; i < rand_num; i++)
            temp = temp->link;

        printf("<<< %s 님의 번호를 입력하세요. : ", temp->name);

        while (count < 3)
        {
            fflush(stdin);
            gets(Num);

            if (strcmp(Num, temp->number) == 0)
            {
                printf("\n>>> 정답입니다!\n\n");
                break;
            }

            count++;
            printf("\n>>> 틀렸습니다!\n<<< 다시 입력하세요. (기회 : %d) : ", 3 - count);
        }

        printf("\n\n<<< 게임을 계속 하시겠습니까? (Y / N) : ");
        count = 0;

        fflush(stdin);
        re = getchar();

        system("cls");
    } while (re != 'N' && re != 'n');

    system("cls");
}
