/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements Monte-Carlo Method.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:15
 */

#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

double areaOfUnitHemicircle(void);

int main(void)
{
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(3);

	cout << ">>> The Area of Unit Hemicircle : " << areaOfUnitHemicircle() << endl;

	return 0;
}

double areaOfUnitHemicircle(void)
{
	const int TOTAL = 10000000;
	int count = 0;
	double x, y;
	double area;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < TOTAL; i++)
	{
		x = (double)rand() / RAND_MAX;
		y = (double)rand() / RAND_MAX;

		if ((x * x + y * y) <= 1.0)
			count++;
	}

	area = ((double)count / TOTAL) * 2;

	return area;
}
