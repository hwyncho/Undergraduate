#!/bin/bash

##################################################
# StudentID : 2014726096
# Name : HwiYeon Cho
#
# This script implement Ataxx game.
# This script was written in Ubuntu.
#
# Last Updated : 2016/04/04 02:15
##################################################

################ Global Variables ################ 
# menu selection
menu=1
num=1

# map
cursorP1=63
cursorP2=56
cursor=0
before=0
select=0
select1=0
select2=0
turn=1
countP1=2
countP2=2
declare -a block
declare -a map

# ID & PW & WIN & LOSE
ID="ID"
PW="PW"
WIN=0
LOSE=0

# 1P & 2P
P1="1P LOGIN"
P2="2P LOGIN"
P1WIN=0
P1LOSE=0
P2WIN=0
P2LOSE=0

# Winner
WINNER=" "

# keys
up=$(echo -en "[A")
down=$(echo -en "[B")
left=$(echo -en "[D")
right=$(echo -en "[C")
enter=$(echo -en "\n")
##################################################

############## User Define Function ##############
# customized "echo"
myecho()
{
    COLUMNS=$(tput cols)
    string1="$2"
    string2="$3"
    string3="$4"
    width=$((($COLUMNS - (${#string1} + ${#string2} + ${#string3})) / 2))

    # align to Center
    if [ "$1" = "-c" ]
    then
        printf "%${width}s"
        echo -e "$2" "$4" "$3"

    # save the current cursor position
    elif [ "$1" = "-s" ]
    then
        printf "%${width}s"
        echo -e "[s"

    # align to Center & highlight to Blue
    elif [ "$1" = "-cb" ]
    then
        printf "%${width}s"
        echo -e "[44m$2[0m" "$4" "[44m$3[0m"

    # align to Center & highlight to Red
    elif [ "$1" = "-cr1" ]
    then
        printf "%${width}s"
        echo -e "[41m$2[0m" "$4" "[44m$3[0m"

    # align to Center & highlight to Red
    elif [ "$1" = "-cr2" ]
    then
        printf "%${width}s"
        echo -e "[44m$2[0m" "$4" "[41m$3[0m"

    # align to Center & highlight to Red & save the current cursor position
    elif [ "$1" = "-scr1" ]
    then
        printf "%${width}s"
        echo -e "[s[41m$2[0m" "$4" "[44m$3[0m"

    # align to Center & highlight to Red & save the current cursor position
    elif [ "$1" = "-scr2" ] 
    then
        printf "%${width}s"
        echo -e "[44m$2[0m" "$4" "[s[41m$3[0m"

    elif [ "$1" = "-m" ]
    then
        if [ $COLUMNS -ge 80 -o $COLUMNS -lt 132 ]
        then
            printf "%23s"
            echo -e "$2"
    
        elif [ $COLUMNS -ge 132 ]
        then
            printf "%49s"
            echo -e "$2"    
        fi

    elif [ "$1" = "-p1" ]
    then
        printf "%${width}s"
        echo -e "[44m$2[0m" "$4" "$3"

    elif [ "$1" = "-p2" ]
    then
        printf "%${width}s"
        echo -e "$2" "$4" "[44m$3[0m"
    fi
}

# main screen
mainTitle()
{
    myecho -c ' ___   ___   ___  ___  _      _ '
     myecho -c '/ __| / _ \ / __||_ _|| |    / |'
    myecho -c '\__ \| (_) |\__ \ | | | |__  | |'
    myecho -c '|___/ \___/ |___/|___||____| |_|'
    myecho -c '   _  _____  _   __  ____  __'
    myecho -c '  /_\|_   _|/_\  \ \/ /\ \/ /'
    myecho -c ' / _ \ | | / _ \  >  <  >  < '
    myecho -c '/_/ \_\|_|/_/ \_\/_/\_\/_/\_\'
    myecho -c ' '
    myecho -c "\t\t2014726096 Cho Hwi Yeon"
}

mainMenu()
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cb "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cb "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   JOIN   " "   EXIT   " "      "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        mainTitle
        mainMenu1
    fi
}

mainMenu1() # highlight "1P LOGIN"
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cr1 "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cb "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenu2() # highlight "SIGN IN"
{
    menu=2

    myecho -c " "
    myecho -c " "
    myecho -cr2 "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cb "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenu3()    # highlight "2P LOGIN"
{
    menu=3

    myecho -c " "
    myecho -c " "
    myecho -cb "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cr1 "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenu4() #highlight "SIGN OUT"
{
    menu=4

    myecho -c " "
    myecho -c " "
    myecho -cb "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cr2 "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenu5() #highlight "JOIN"
{
    menu=5

    myecho -c " "
    myecho -c " "
    myecho -cb "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cb "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr1 "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenu6()    #highlight "EXIT"
{
    menu=6

    myecho -c " "
    myecho -c " "
    myecho -cb "   $P1   " "   SIGN IN   " "      "
    myecho -c " "
    myecho -cb "   $P2   " "   SIGN OUT   " "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr2 "   JOIN   " "   EXIT   " "      "

    mainMenuKey
}

mainMenuKey()   # main menu direction
{
    read -n 3 key

    if [ "$key" = "$up" ]
    then
        menu=`expr $menu - 2`

    elif [ "$key" = "$down" ]
    then
        menu=`expr $menu + 2`

    elif [ "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            num=1
            clear
            logInP1
            logInMenu

        elif [ "$menu" = 2 ]
        then
            clear
            signIn
            signInMenu

        elif [ "$menu" = 3 ]
        then
            num=2
            clear
            logInP2
            logInMenu

        elif [ "$menu" = 4 ]
        then
            clear
            signOut
            signOutMenu

        elif [ "$menu" = 5 ]
        then
            temp1=$(checkP1ID)
            temp2=$(checkP2ID)

            if [ "$temp1" != "./DB/LogIn/$P1" -o "$temp2" != "./DB/LogIn/$P2" ]
            then
                myecho -c " "
                myecho -c ">>> You can't get into the lobby!"
                sleep 1.5
                resetIDPW
                clear
                mainTitle
                mainMenu

            else
                clear
                ataxxLobby
                lobbyMenu
            fi

        elif [ "$menu" = 6 ]
        then
            clear
            myexit
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 6`

    elif [ $menu -gt 6 ]
    then
        menu=`expr $menu - 6`
    fi

    clear
    mainTitle
    mainMenu$menu
}

# login screen
logInP1()
{
    myecho -c ' _  ___   _     ___    ___  ___  _  _  '
    myecho -c '/ || _ \ | |   / _ \  / __||_ _|| \| | '
    myecho -c '| ||  _/ | |__| (_) || (_ | | | | .` | '
    myecho -c '|_||_|   |____|\___/  \___||___||_|\_| '                                  
}

logInP2()
{
    myecho -c ' ___  ___   _     ___    ___  ___  _  _ '
    myecho -c '|_  )| _ \ | |   / _ \  / __||_ _|| \| |'
    myecho -c ' / / |  _/ | |__| (_) || (_ | | | | .` |'
    myecho -c '/___||_|   |____|\___/  \___||___||_|\_|'                                    
}

logInMenu()
{
    menu=1

    resetIDPW

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   LOGIN   " "   EXIT   " "      "
  
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        logInP$num
        logInMenu1
    fi
}

logInMenu1()    # highlight ID blank
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cr1 "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   LOGIN   " "   EXIT   " "      "
  
    logInMenuKey
}

logInMenu2()    # highlight PW blank
{
    menu=2

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cr1 "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   LOGIN   " "   EXIT   " "      "
  
    logInMenuKey
}

logInMenu3()    # highlight "LOGIN"
{
    menu=3

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr1 "   LOGIN   " "   EXIT   " "      "
  
    logInMenuKey
}

logInMenu4()    # highlight "EXIT"
{
    menu=4

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr2 "   LOGIN   " "   EXIT   " "      "
  
    logInMenuKey
}

logInMenuKey()    # login menu direction
{
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$down" -o "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            clear
            logInP$num
            inputID3

        elif [ "$menu" = 2 ]
        then
            clear
            logInP$num
            inputPW3

        elif [ "$menu" = 3 ]
        then
            temp1=$(checkID)
            temp2=$(checkPW)
            temp3=$(checkP1ID)

            if [ "$temp1" != "./DB/Accounts/$ID" -o "$temp2" != "$PW" ]
            then
                myecho -c " "
                myecho -c ">>> There is not ID or Password is not valid!"
                sleep 1.5
                resetIDPW
                clear
                logInP$num
                logInMenu1

            elif [ "$temp3" = "./DB/LogIn/$ID" ]
            then
                myecho -c " "
                myecho -c ">>> $ID has already loged in!"
                sleep 1.5
                resetIDPW
                clear
                logInP$num
                logInMenu1

            else
                logInDB
                myecho -c " "
                myecho -c ">>> LogIn has been successfully completed!"
                sleep 1.5
                if [ $num = 1 ]
                then
                    P1="$ID"
                    P1WIN=$(cat ./DB/Accounts/$ID/WIN.txt)
                    P1LOSE=$(cat ./DB/Accounts/$ID/LOSE.txt)

                elif [ $num = 2 ]
                then
                    P2="$ID"
                    P2WIN=$(cat ./DB/Accounts/$ID/WIN.txt)
                    P2LOSE=$(cat ./DB/Accounts/$ID/LOSE.txt)
                fi
        
                resetIDPW
                clear
                mainTitle
                mainMenu
            fi

        elif [ "$menu" = 4 ]
        then
            resetIDPW
            clear
            mainTitle
            mainMenu
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 4`

    elif [ $menu -gt 4 ]
    then
        menu=`expr $menu - 4`
    fi

    clear
    logInP$num
    logInMenu$menu
}

# signin screen
signIn()
{
    myecho -c ' ___  ___  ___  _  _   ___  _  _ '
     myecho -c '/ __||_ _|/ __|| \| | |_ _|| \| |'
    myecho -c '\__ \ | || (_ || .` |  | | | .` |'
      myecho -c '|___/|___|\___||_|\_| |___||_|\_|'
}

signInMenu()
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        signIn
        signInMenu1
    fi
}

signInMenu1()    # highlight ID blank
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cr1 "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "
  
    signInMenuKey
}

signInMenu2()    # highlight "Duplicate Check"
{
    menu=2

    myecho -c " "
    myecho -c " "
    myecho -cr2 "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "

    signInMenuKey
}

signInMenu3()    # highlight PW blank
{
    menu=3

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cr1 "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "

    signInMenuKey
}

signInMenu4()    # highlight "SIGN IN"
{
    menu=4

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr1 "   SIGN IN   " "    EXIT    " "      "
   
    signInMenuKey
}

signInMenu5()    # highlight "EXIT"
{
    menu=5

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr2 "   SIGN IN   " "    EXIT    " "      "

    signInMenuKey
}

signInMenuKey()    #signin menu direction
{
    read -n 3 key

    if [ "$key" = "$up" ]
    then
        menu=`expr $menu - 2`

    elif [ "$key" = "$down" ]
    then
        menu=`expr $menu + 2`

    elif [ "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            clear
            signIn
            inputID1

        elif [ "$menu" = 2 ]
        then
            temp=$(checkID)

            if [ "$temp" = "./DB/Accounts/$ID" ]
            then
                myecho -c " "
                myecho -c ">>> There is the same ID!"
                myecho -c ">>> Please input new ID!"
                sleep 1.5
                resetIDPW
                clear
                signIn
                signInMenu1

            else
                myecho -c " "
                myecho -c ">>> You can register this ID!"
                sleep 1.5
                clear
                signIn
                signInMenu3
            fi

        elif [ "$menu" = 3 ]
        then
            clear
            signIn
            inputPW1

        elif [ "$menu" = 4 ]
        then
            temp=$(checkID)

            if [ "$temp" = "./DB/Accounts/$ID" ]
            then
                myecho -c " "
                myecho -c ">>> There is the same ID!"
                myecho -c ">>> Please input new ID!"
                sleep 1.5
                resetIDPW
                clear
                signIn
                signInMenu1

            else
                accountDB
                myecho -c " "
                myecho -c ">>> Your account has been successfully created!"
                sleep 1.5
                resetIDPW
                clear
                mainTitle
                mainMenu
            fi

        elif [ "$menu" = 5 ]
        then
            resetIDPW
            clear
            mainTitle
            mainMenu
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 5`

    elif [ $menu -gt 5 ]
    then
        menu=`expr $menu - 5`
    fi

    clear
    signIn
    signInMenu$menu
}

# signout screen
signOut()
{
    myecho -c ' ___  ___  ___  _  _    ___   _   _  _____ '
    myecho -c '/ __||_ _|/ __|| \| |  / _ \ | | | ||_   _|'
    myecho -c '\__ \ | || (_ || .` | | (_) || |_| |  | |  '
    myecho -c '|___/|___|\___||_|\_|  \___/  \___/   |_|  '
}

signOutMenu()
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN OUT   " "   EXIT   " "      "
  
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        signOut
        signOutMenu1
    fi
}

signOutMenu1()    #highlight ID
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cr1 "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN OUT   " "   EXIT   " "      "
  
    signOutMenuKey
}

signOutMenu2()    #highlight PW
{
    menu=2

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cr1 "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN OUT   " "   EXIT   " "      "
  
    signOutMenuKey
}

signOutMenu3()    #highlight SIGN OUT
{
    menu=3

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr1 "   SIGN OUT   " "   EXIT   " "      "
  
    signOutMenuKey
}

signOutMenu4()    #highlight EXIT
{
    menu=4

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cr2 "   SIGN OUT   " "   EXIT   " "      "

    signOutMenuKey
}

signOutMenuKey()    #signout menu key
{
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$down" -o "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            clear
            signOut
            inputID2

        elif [ "$menu" = 2 ]
        then
            clear
            signOut
            inputPW2

        elif [ "$menu" = 3 ]
        then
            temp1=$(checkID)
            temp2=$(checkPW)

            if [ "$temp1" != "./DB/Accounts/$ID" -o "$temp2" != "$PW" ]
            then
                myecho -c " "
                myecho -c ">>> There is not ID or Password is not valid!"
                myecho -c ">>> Please re-enter!"
                sleep 1.5
                resetIDPW
                clear
                signOut
                signOutMenu1

            else
                rm -rf ./DB/Accounts/$ID
                myecho -c " "
                myecho -c ">>> Your account has been successfully removed!"
                sleep 1.5
                resetIDPW
                clear
                mainTitle
                mainMenu
            fi

        elif [ "$menu" = 4 ]
        then
            resetIDPW
            clear
            mainTitle
            mainMenu
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 4`

    elif [ $menu -gt 4 ]
    then
        menu=`expr $menu - 4`
    fi

    clear
    signOut
    signOutMenu$menu
}

# lobby screen
ataxxLobby()
{
    myecho -c '   _  _____  _   __  ____  __'
    myecho -c '  /_\|_   _|/_\  \ \/ /\ \/ /'
    myecho -c ' / _ \ | | / _ \  >  <  >  < '
    myecho -c '/_/ \_\|_|/_/ \_\/_/\_\/_/\_\'
    myecho -c ' _     ___   ___  ___ __   __'
    myecho -c '| |   / _ \ | _ )| _ )\ \ / /'
    myecho -c '| |__| (_) || _ \| _ \ \ V / '
    myecho -c '|____|\___/ |___/|___/  |_|  '
    myecho -c " "
    myecho -c ' _  ___ ' ' ___  ___ ' "                     "
    myecho -c '/ || _ \' '|_  )| _ \' "                     "
    myecho -c '| ||  _/' ' / / |  _/' "                     "
    myecho -c '|_||_|  ' '/___||_|  ' "                     "
    myecho -c " "
    myecho -c "ID : $P1" "ID : $P2" "               "
    myecho -c "WIN : $P1WIN      " "WIN : $P2WIN      " "               "
    myecho -c "LOSE : $P1LOSE     " "LOSE : $P2LOSE     " "               "
}

lobbyMenu()
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cb "   START   " "   EXIT   " "      "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        ataxxLobby
        lobbyMenu1
    fi
}

lobbyMenu1()    # highlight "START"
{
    menu=1

    myecho -c " "
    myecho -c " "
    myecho -cr1 "   START   " "   EXIT   " "      "

    lobbyMenuKey
}

lobbyMenu2()    #highlight "EXIT"
{
    menu=2

    myecho -c " "
    myecho -c " "
    myecho -cr2 "   START   " "   EXIT   " "      "

    lobbyMenuKey
}

lobbyMenuKey()  #lobby menu direction
{
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$down" -o "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            clear
            mapSelect
            mapMenu

        elif [ "$menu" = 2 ]
        then
            clear
            mainTitle
            mainMenu
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 2`

    elif [ $menu -gt 2 ]
    then
        menu=`expr $menu - 2`
    fi

    clear
    ataxxLobby
    lobbyMenu$menu
}

# map select screen
mapSelect()
{
    myecho -c '   _  _____  _   __  ____  __'
    myecho -c '  /_\|_   _|/_\  \ \/ /\ \/ /'
    myecho -c ' / _ \ | | / _ \  >  <  >  < '
    myecho -c '/_/ \_\|_|/_/ \_\/_/\_\/_/\_\'
    myecho -c ' __  __    _    ___   ___  ___  _     ___  ___  _____ '
    myecho -c '|  \/  |  /_\  | _ \ / __|| __|| |   | __|/ __||_   _|'
    myecho -c '| |\/| | / _ \ |  _/ \__ \| _| | |__ | _|| (__   | |  '
    myecho -c '|_|  |_|/_/ \_\|_|   |___/|___||____||___|\___|  |_|  '
    myecho -c " "
    myecho -c " "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?????????????????????????? "?????????????????????????? "         "
    myecho -c "?�???????????????????????? "?�???????????????????????? "         "
}

mapMenu()
{
    menu=1

    myecho -c " "
    myecho -cb "   MAP 1   " "   MAP 2   " "               "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        mapSelect
        mapMenu1
    fi
}

mapMenu1()  #highlight "MAP 1"
{
    menu=1

    myecho -c " "
    myecho -cr1 "   MAP 1   " "   MAP 2   " "               "

    mapMenuKey
}

mapMenu2()    #highlight "MAP 2"
{
    menu=2

    myecho -c " "
    myecho -cr2 "   MAP 1   " "   MAP 2   " "               "

    mapMenuKey
}

mapMenuKey()    #map menu direction
{
    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$left" ]
    then
        menu=`expr $menu - 1`

    elif [ "$key" = "$down" -o "$key" = "$right" ]
    then
        menu=`expr $menu + 1`

    elif [ "$key" = "$enter" ]
    then
        if [ "$menu" = 1 ]
        then
            resetMap1
            clear
            ataxxTitle
            ataxxMap1

        elif [ "$menu" = 2 ]
        then
            resetMap2
            clear
            ataxxTitle
            ataxxMap2
        fi
    fi

    if [ $menu -lt 1 ]
    then
        menu=`expr $menu + 2`

    elif [ $menu -gt 2 ]
    then
        menu=`expr $menu - 2`
    fi

    clear
    mapSelect
    mapMenu$menu
}

ataxxTitle()
{
    myecho -c '   _  _____  _   __  ____  __'
    myecho -c '  /_\|_   _|/_\  \ \/ /\ \/ /'
    myecho -c ' / _ \ | | / _ \  >  <  >  < '
    myecho -c '/_/ \_\|_|/_/ \_\/_/\_\/_/\_\'
}

resetMap1() # reset MAP 1
{
    cursorP1=63
    cursorP2=56
    cursor=0
    before=0
    select=0
    select1=0
    select2=0
    turn=1
    countP1=2
    countP2=2

    for i in {0..63}
    do
        block[$i]="   "
        map[$i]=0

        if [ "$i" = 0 -o "$i" = 63 ]
        then
            map[$i]=1

        elif [ "$i" = 7 -o "$i" = 56 ]
        then
            map[$i]=2
        fi
    done
}

resetMap2() # reset MAP 2
{
    cursorP1=63
    cursorP2=56
    cursor=0
    before=0
    select=0
    select1=0
    select2=0
    turn=1
    countP1=2
    countP2=2

    for i in {0..63}
    do
        block[$i]="   "
        map[$i]=0

        if [ "$i" = 9 -o "$i" = 14 -o "$i" = 18 -o "$i" = 21 -o "$i" = 27 -o "$i" = 28 -o "$i" = 35 -o "$i" = 36 -o "$i" = 42 -o "$i" = 45 -o "$i" = 49 -o "$i" = 54 ]
        then
            map[$i]=3

        elif [ "$i" = 0 -o "$i" = 63 ]
        then
            map[$i]=1

        elif [ "$i" = 7 -o "$i" = 56 ]
        then
            map[$i]=2
        fi
    done
}

ataxxMap1()    # MAP 1 screen
{
    menu=1

    if [ `expr $turn % 2` = 1 ]
    then
        block[$cursorP1]="[47m   [0m"

    else
        block[$cursorP2]="[47m   [0m"
    fi

    for i in {0..63}
    do
        if [ "${map[$i]}" = 1 ]
        then
            block[$i]="[41m   [0m"

        elif [ "${map[$i]}" = 2 ]
        then
            block[$i]="[42m   [0m"
        fi
    done

    myecho -c " "
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[0]}??{block[1]}??{block[2]}??{block[3]}??{block[4]}??{block[5]}??{block[6]}??{block[7]}??
     myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[8]}??{block[9]}??{block[10]}??{block[11]}??{block[12]}??{block[13]}??{block[14]}??{block[15]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[16]}??{block[17]}??{block[18]}??{block[19]}??{block[20]}??{block[21]}??{block[22]}??{block[23]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[24]}??{block[25]}??{block[26]}??{block[27]}??{block[28]}??{block[29]}??{block[30]}??{block[31]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[32]}??{block[33]}??{block[34]}??{block[35]}??{block[36]}??{block[37]}??{block[38]}??{block[39]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[40]}??{block[41]}??{block[42]}??{block[43]}??{block[44]}??{block[45]}??{block[46]}??{block[47]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[48]}??{block[49]}??{block[50]}??{block[51]}??{block[52]}??{block[53]}??{block[54]}??{block[55]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[56]}??{block[57]}??{block[58]}??{block[59]}??{block[60]}??{block[61]}??{block[62]}??{block[63]}??
    myecho -m "?�????????????????????????????????????????????????????????

    if [ `expr $turn % 2` = 1 ]
    then
        myecho -p1 "1P : $countP1" "2P : $countP2" "         "

    else
        myecho -p2 "1P : $countP1" "2P : $countP2" "         "
    fi

    checkWinner
    mapKey
}

ataxxMap2() # MAP 2 screen
{
    menu=2

    if [ `expr $turn % 2` = 1 ]
    then
        block[$cursorP1]="[47m   [0m"

    else
        block[$cursorP2]="[47m   [0m"
    fi

    for i in {0..63}
    do
        if [ "${map[$i]}" = 3 ]
        then
            block[i]="[43m   [0m"
            if [ "$i" = "$cursor" ]
            then
                block[$cursor]="[47m   [0m"
            fi

        elif [ "${map[$i]}" = 1 ]
        then
            block[$i]="[41m   [0m"

        elif [ "${map[$i]}" = 2 ]
        then
            block[$i]="[42m   [0m"
        fi
    done

    myecho -c " "
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[0]}??{block[1]}??{block[2]}??{block[3]}??{block[4]}??{block[5]}??{block[6]}??{block[7]}??
     myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[8]}??{block[9]}??{block[10]}??{block[11]}??{block[12]}??{block[13]}??{block[14]}??{block[15]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[16]}??{block[17]}??{block[18]}??{block[19]}??{block[20]}??{block[21]}??{block[22]}??{block[23]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[24]}??{block[25]}??{block[26]}??{block[27]}??{block[28]}??{block[29]}??{block[30]}??{block[31]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[32]}??{block[33]}??{block[34]}??{block[35]}??{block[36]}??{block[37]}??{block[38]}??{block[39]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[40]}??{block[41]}??{block[42]}??{block[43]}??{block[44]}??{block[45]}??{block[46]}??{block[47]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[48]}??{block[49]}??{block[50]}??{block[51]}??{block[52]}??{block[53]}??{block[54]}??{block[55]}??
    myecho -m "??????????????????????????????????????????????????????????
    myecho -m "??{block[56]}??{block[57]}??{block[58]}??{block[59]}??{block[60]}??{block[61]}??{block[62]}??{block[63]}??
    myecho -m "?�????????????????????????????????????????????????????????

    if [ `expr $turn % 2` = 1 ]
    then
        myecho -p1 "1P : $countP1" "2P : $countP2" "         "

    else
        myecho -p2 "1P : $countP1" "2P : $countP2" "         "
    fi

    checkWinner
    mapKey
}

mapKey()    # map direction
{
    if [ `expr $turn % 2` = 1 ]
    then
        cursor=$cursorP1

    else
        cursor=$cursorP2
    fi

    before=$cursor

    read -n 3 key

    if [ "$key" = "$up" ]
    then
        cursor=`expr $cursor - 8`

    elif [ "$key" = "$down" ]
    then
        cursor=`expr $cursor + 8`

    elif [ "$key" = "$left" ]
    then
        cursor=`expr $cursor - 1`

    elif [ "$key" = "$right" ]
    then
        cursor=`expr $cursor + 1`

    elif [ "$key" = "$enter" ]
    then
        stoneCopyMove
    fi

    if [ $cursor -lt 0 ]
    then
        cursor=`expr $cursor + 64`

    elif [ $cursor -gt 63 ]
    then
        cursor=`expr $cursor - 64`
    fi

    if [ `expr $turn % 2` = 1 ]
    then
        cursorP1=$cursor

    else
        cursorP2=$cursor
    fi

    block[$before]="   "
    clear
    ataxxTitle
    ataxxMap$menu
}

stoneCopyMove()
{
    if [ "$select" = 0 ]
    then
        if [ `expr $turn % 2` = 1 ]
        then
            if [ "${map[$cursor]}" = 1 ]
            then
                select1=$cursor
                if [ $select1 -lt 0 ]
                then
                    select1=`expr $select + 64`

                elif [ $select1 -gt 63 ]
                then
                    select1=`expr $select - 64`
                fi
    
                select=1

            elif [ "${map[$cursor]}" = 2 ]
            then
                myecho -c ">>> Cannot select this point!"
                sleep 1.5

            else
                myecho -c ">>> Please select the stone first!"
                sleep 1.5
            fi

        else
            if [ "${map[$cursor]}" = 2 ]
            then
                select1=$cursor
                if [ $select1 -lt 0 ]
                then
                    select1=`expr $select + 64`

                elif [ $select1 -gt 63 ]
                then
                    select1=`expr $select - 64`
                fi
    
                select=1

            elif [ "${map[$cursor]}" = 1 ]
            then
                myecho -c ">>> Cannot select this point!"
                sleep 1.5

            else
                myecho -c ">>> Please select the stone first!"
                sleep 1.5
            fi
        fi

    else
        if [ "${map[$cursor]}" = 3 ]
        then
            myecho -c ">>> Cannot select this point!"
            sleep 1.5

        else
            select2=$cursor

            diff=`expr $select2 - $select1`
            if [ "$diff" -lt 0 ]
            then
                diff=`expr 0 - $diff`
            fi

            if [ "$diff" = 1 -o "$diff" = 7 -o "$diff" = 8 -o "$diff" = 9 ]
            then
                if [ `expr $turn % 2` = 1 ]
                then
                    map[$select2]=1
                    countP1=`expr $countP1 + 1`
                    cursorP1=$cursor

                    stoneAbsorb

                else
                    map[$select2]=2
                    countP2=`expr $countP2 + 1`
                    cursorP2=$cursor

                    stoneAbsorb
                fi

                turn=`expr $turn + 1`
                select=0
                block[$before]="   "
                clear
                ataxxTitle
                ataxxMap$menu

            elif [ "$diff" = 2 -o "$diff" = 6 -o "$diff" = 10 -o "$diff" = 14 -o "$diff" = 15 -o "$diff" = 16 -o "$diff" = 17 -o "$diff" = 18 ]
            then
                map[$select1]=0
                block[$select1]="   "

                if [ `expr $turn % 2` = 1 ]
                then
                    map[$select2]=1
                    cursorP1=$cursor

                    stoneAbsorb

                else
                    map[$select2]=2
                    cursorP2=$cursor

                    stoneAbsorb
                fi

                turn=`expr $turn + 1`
                select=0
                block[$before]="   "
                clear
                ataxxTitle
                ataxxMap$menu

            else
                myecho -c ">>> Please select again!"
                sleep 1.5
            fi
        fi
    fi
}

stoneAbsorb()
{
    for i in {0..63}
    do
        diff=`expr $i - $select2`
        if [ "$diff" -lt 0 ]
        then
            diff=`expr 0 - $diff`
        fi

        if [ "$diff" = 1 -o "$diff" = 7 -o "$diff" = 8 -o "$diff" = 9 ]
        then
            if [ `expr $turn % 2` = 1 ]
            then
                if [ "${map[$i]}" = 2 ]
                then
                    map[$i]=1
                    countP1=`expr $countP1 + 1`
                    countP2=`expr $countP2 - 1`
                    block[$i]="[41m   [0m"
                fi

            else
                if [ "${map[$i]}" = 1 ]
                then
                    map[$i]=2
                    countP2=`expr $countP2 + 1`
                    countP1=`expr $countP1 - 1`
                    block[$i]="[42m   [0m"
                fi
            fi
        fi
    done
}


checkWinner()
{
    result=`expr $countP1 + $countP2`
    if [ "$menu" = 1 -a "$result" = 64 -o "$menu" = 2 -a "$result" = 52 ]
    then
        if [ "$countP1" -gt "$countP2" ]
        then
            WINNER="$P1"
            P1WIN=`expr $P1WIN + 1`
            P2LOSE=`expr $P2LOSE + 1`
            echo "$P1WIN" > ./DB/Accounts/$P1/WIN.txt
            echo "$P2LOSE" > ./DB/Accounts/$P2/LOSE.txt
            clear
            victory
            victoryMenu

        elif [ "$countP2" -gt "$countP1" ]
        then
            WINNER="$P2"
            P2WIN=`expr $21WIN + 1`
            P1LOSE=`expr $P1LOSE + 1`
            echo "$P2WIN" > ./DB/Accounts/$P2/WIN.txt
            echo "$P1LOSE" > ./DB/Accounts/$P1/LOSE.txt
            clear
            victory
            victoryMenu
        fi

    else
        if [ "$countP2" = 0 ]
        then
            WINNER="$P1"
            P1WIN=`expr $P1WIN + 1`
            P2LOSE=`expr $P2LOSE + 1`
            echo "$P1WIN" > ./DB/Accounts/$P1/WIN.txt
            echo "$P2LOSE" > ./DB/Accounts/$P2/LOSE.txt
            clear
            victory
            victoryMenu

        elif [ "$countP1" = 0 ]
        then
            WINNER="$P2"
            P2WIN=`expr $P1WIN + 1`
            P1LOSE=`expr $P1LOSE + 1`
            echo "$P2WIN" > ./DB/Accounts/$P2/WIN.txt
            echo "$P1LOSE" > ./DB/Accounts/$P1/LOSE.txt
            clear
            victory
            victoryMenu
        fi
    fi
}

victory()
{
    win=$(cat ./DB/Accounts/$WINNER/WIN.txt)
    lose=$(cat ./DB/Accounts/$WINNER/LOSE.txt)

    myecho -c '__      __ _____  _____  _______  ____   _____ __     __ _ '
    myecho -c '\ \    / /|_   _|/ ____||__   __|/ __ \ |  __ \\\ \   / /| |'
    myecho -c ' \ \  / /   | | | |        | |  | |  | || |__) |\ \_/ / | |'
    myecho -c '  \ \/ /    | | | |        | |  | |  | ||  _  /  \   /  | |'
    myecho -c '   \  /    _| |_| |____    | |  | |__| || | \ \   | |   |_|'
    myecho -c '    \/    |_____|\_____|   |_|   \____/ |_|  \_\  |_|   (_)'

    myecho -c ' '
    myecho -c ' '
    myecho -c '__      __ ___  _  _  _  _  ___  ___ '
    myecho -c '\ \    / /|_ _|| \| || \| || __|| _ \'
    myecho -c ' \ \/\/ /  | | | .` || .` || _| |   /'
    myecho -c '  \_/\_/  |___||_|\_||_|\_||___||_|_\'

    myecho -c " "
    myecho -c "$WINNER"
    myecho -c "WIN : $win" "LOSE : $lose" "   "
}

victoryMenu()
{
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   OK   "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        victory
        victoryMenu1
    fi
}

victoryMenu1()
{
    myecho -c " "
    myecho -c " "
    myecho -cr1 "   OK   "

    read -n 3 key

    if [ "$key" = "$up" -o "$key" = "$down" -o "$key" = "$left" -o "$key" = "$right" ]
    then
        clear
        victory
        victoryMenu1

    else
        clear
        ataxxLobby
        lobbyMenu
    fi
}

# exit screen
myexit()
{
    myecho -c " ___ __  __ ___  _____   ____ "
    myecho -c "| __|\ \/ /|_ _||_   _| |__ / "
    myecho -c "| _|  >  <  | |   | |    |_ \ "
    myecho -c "|___|/_/\_\|___|  |_|   |___/ "
    sleep 1

    clear
    myecho -c " ___ __  __ ___  _____   ___  "
    myecho -c "| __|\ \/ /|_ _||_   _| |_  ) "
    myecho -c "| _|  >  <  | |   | |    / /  " 
    myecho -c "|___|/_/\_\|___|  |_|   /___| "
    sleep 1

    clear
    myecho -c " ___ __  __ ___  _____   _  "
    myecho -c "| __|\ \/ /|_ _||_   _| / | "
    myecho -c "| _|  >  <  | |   | |   | | "
    myecho -c "|___|/_/\_\|___|  |_|   |_| "
    sleep 1

    clear
    exit
}

# reset ID & PW
resetIDPW()
{
    ID="ID"
    PW="PW"
}

# input ID of SIGN IN
inputID1() 
{
    ID="  "

    myecho -c " "
    myecho -c " "
    myecho -scr1 "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -cb "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "

    echo -ne "[u[41m"
    read ID
    echo -e "[0m"

    clear
    signIn
    signInMenu2
}

# input ID of SIGN OUT
inputID2()
{
    ID="  "

    myecho -c " "
    myecho -c " "
    myecho -scr1 "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN OUT   " "   EXIT   " "      "

    echo -ne "[u[41m"
    read ID
    echo -e "[0m"

    clear
    signOut
    signOutMenu2
}

# input ID of LOGIN
inputID3()
{
    ID="  "

    myecho -c " "
    myecho -c " "
    myecho -scr1 "         $ID         "
    myecho -c " "
    myecho -cb "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   LOGIN   " "   EXIT   " "      "

    echo -ne "[u[41m"
    read ID
    echo -e "[0m"

    clear
    logInP$num
    logInMenu2
}

# input PW of SIGN IN
inputPW1()
{
    PW="  "

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         " "   Duplicate Check   " "      "
    myecho -c " "
    myecho -scr1 "         $PW         " "[0m             [0m" "      "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN IN   " "    EXIT    " "      "

    echo -ne "[u[41m"
    read PW
    echo -e "[0m"

    clear
    signIn
    signInMenu4
}

# input PW of SIGN OUT
inputPW2()
{
    PW="  "

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -scr1 "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   SIGN OUT   " "   EXIT   " "      "

    echo -ne "[u[41m"
    read PW
    echo -e "[0m"

    clear
    signOut
    signOutMenu3
}

# input PW of LOGIN
inputPW3()
{
    PW="  "

    myecho -c " "
    myecho -c " "
    myecho -cb "         $ID         "
    myecho -c " "
    myecho -scr1 "         $PW         "
    myecho -c " "
    myecho -c " "
    myecho -c " "
    myecho -cb "   LOGIN   " "   EXIT   " "      "

    echo -ne "[u[41m"
    read PW
    echo -e "[0m"

    clear
    logInP$num
    logInMenu3
}

# make Database
makeDB()
{
    check=$(find . -name DB -type d)
    if [ "$check" = "./DB" ]
    then
        clear

    else
        mkdir ./DB
    fi

    check=$(find ./DB -name Accounts -type d)
    if [ "$check" = "./DB/Accounts" ]
    then
        clear

    else

        mkdir ./DB/Accounts
    fi

    check=$(find ./DB -name LogIn -type d)
    if [ "$check" = "./DB/LogIn" ]
    then
        rm -rf ./DB/LogIn
        mkdir ./DB/LogIn

    else
        mkdir ./DB/LogIn
    fi
}

# make new account's data
accountDB()
{
    mkdir ./DB/Accounts/$ID

    touch ./DB/Accounts/$ID/PW.txt
    echo "$PW" >> ./DB/Accounts/$ID/PW.txt

    touch ./DB/Accounts/$ID/WIN.txt
    echo "$WIN" >> ./DB/Accounts/$ID/WIN.txt 

    touch ./DB/Accounts/$ID/LOSE.txt
    echo "$LOSE" >> ./DB/Accounts/$ID/LOSE.txt
}

# check that the ID exists
checkID()
{
    find ./DB/Accounts -name "$ID" -type d
}

# check PW is correct
checkPW()
{
    cat ./DB/Accounts/$ID/PW.txt
}

# copy data of player from DB to login history
logInDB()
{
    cp -r ./DB/Accounts/$ID ./DB/LogIn/
}

# check 1P ID
checkP1ID()
{
    find ./DB/LogIn -name "$P1" -type d
}

# check 2P ID
checkP2ID()
{
    find ./DB/LogIn -name "$P2" -type d
}

##################################################

################# Main Function ##################
makeDB
clear
mainTitle
mainMenu
##################################################
