/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:15
 */
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

void rangeCheck(int input, int min, int max);
int convert(int n);    // convert decimal to ternary

int main(void)
{
    int n;

    cout << "<<< Please input an integer n(1 ~ 10) : ";
    cin >> n;
    rangeCheck(n, 1, 10);

    cout << ">>> All the ternary strings of length n : " << endl;

    /* 'ternary number of length n' ranges from 0 to (3^n - 1) */
    for (int i = 0; i <= pow(3, n) - 1; i++)
    {
        cout.width(n);
        cout.fill('0');
        cout << convert(i) << endl;
    }

    return 0;
}

void rangeCheck(int input, int min, int max)
{
    if (input < min || input > max)
    {
        cerr << ">>> The input value is not valid!" << endl;

        exit(1);
    }
}

int convert(int n)
{
    if (n == 0)
        return 0;

    else if (n == 1)
        return 1;

    else if (n == 2)
        return 2;

    else
        return convert(n / 3) * 10 + (n % 3);
}
