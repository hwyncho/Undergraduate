/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements Ackermann Function.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:14
 */

#include <cstdlib>
#include <iostream>

using namespace std;

void rangeCheck(unsigned int input, unsigned int min, unsigned int max);
unsigned int function(unsigned int x, unsigned int y);

int main(void)
{
    unsigned int x, y;

    cout << ">>> Please input two integers." << endl;

    cout << "<<< x(0 ~ 3) : ";
    cin >> x;
    rangeCheck(x, 0, 3);

    cout << "<<< y(0 ~ 13) : ";
    cin >> y;
    rangeCheck(y, 0, 13);

    cout << ">>> Result : " << function(x, y) << endl;

    return 0;
}

void rangeCheck(unsigned input, unsigned int min, unsigned int max)
{
    if (input < min || input > max)
    {
        cerr << ">>> The input value is not valid!" << endl;

        exit(1);
    }
}

unsigned int function(unsigned int x, unsigned int y)
{
    while (x != 0)
    {
        if (y == 0)
            y = 1;

        else
            y = function(x, y - 1);

        x--;
    }

    return y + 1;
}
