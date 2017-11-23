/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:14
 */
#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

void rangeCheck(int input, int min, int max);
int sumOfDyadicNumbers(int n);

int main(void)
{
    int n;

    cout << "<<< Please input an integer n(1 ~ 100000000) : ";
    cin >> n;
    rangeCheck(n, 1, 100000000);

    cout << ">>> The Sum of All Dyadic Numbers less that or equal to n : " << sumOfDyadicNumbers(n) << endl;

    return 0;
}

void rangeCheck(int input, int min, int max)
{
    if (input < 1 || input > 100000000)
    {
        cerr << ">>> The input value is not valid!" << endl;

        exit(1);
    }
}

int sumOfDyadicNumbers(int n)
{
    int i = 0;
    int sum = 0;

    do
    {
        sum += (int)pow(2, i);
        i++;
    } while (pow(2, i) <= n);

    return sum;
}
