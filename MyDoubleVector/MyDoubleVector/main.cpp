/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:13
 */

#include <cstdlib>
#include <iostream>
#include "MyDoubleVector.h"

using namespace std;

int main()
{
	MyDoubleVector v1;
	MyDoubleVector v2;

	/* empty() Test */
	cout << endl;
	cout << "----------empty() Test----------" << endl;

	cout << ">>> The object v1 is empty? : " << v1.empty() << endl;
	cout << ">>> The object v2 is empty? : " << v2.empty() << endl;

	cout << "--------------------------------" << endl;

	/* reserve() & capacity() Test */
	cout << endl;
	cout << "--reserve() & capacity() Test--" << endl;

	cout << "<<< v1.reserve(4);" << endl;
	v1.reserve(4);
	cout << ">>> The capacity of v1 is : " << v1.capacity() << endl;

	cout << "<<< v2.reserve(3);" << endl;
	v2.reserve(3);
	cout << ">>> The capacity of v2 is : " << v2.capacity() << endl;

	cout << "-------------------------------" << endl;


	/* push_back() & pop_back() & size() & operator[] Test */
	cout << endl;
	cout << "---push_back() & pop_back() & size() & operator[] Test---" << endl;
	cout << "<<< v1.push_back(1);" << endl;
	v1.push_back(1);
	cout << "<<< v1.push_back(2);" << endl;
	v1.push_back(2);
	cout << "<<< v1.push_back(3);" << endl;
	v1.push_back(3);

	cout << ">>> v1 = (";
	for (size_t i = 0; i < v1.size(); i++)
		cout << v1.operator[](i) << " ";
	cout << ")" << endl;

	cout << ">>> The size of v1 is : " << v1.size() << endl;

	cout << "<<< v2.push_back(9);" << endl;
	v2.push_back(9);
	cout << "<<< v2.push_back(8);" << endl;
	v2.push_back(8);

	cout << ">>> v2 = (";
	for (size_t j = 0; j < v2.size(); j++)
		cout << v2.operator[](j) << " ";
	cout << ")" << endl;

	cout << ">>> The size of v2 is : " << v2.size() << endl;

	cout << "<<< v1.pop_back();" << endl;
	v1.pop_back();

	cout << ">>> v1 = (";
	for (size_t i = 0; i < v1.size(); i++)
		cout << v1.operator[](i) << " ";
	cout << ")" << endl;

	cout << ">>> The size of v1 is : " << v1.size() << endl;

	cout << "--------------------------------------------------------" << endl;

	/* operator+ & operator- & operator* & operator- Test */
	cout << endl;
	cout << "---operator+ & operator- & operator* & operator- Test---" << endl;

	cout << "v1 + v2 = (";
	v1.operator+(v2);
	for (size_t j = 0; j < v1.size(); j++)
		cout << v1.operator[](j) << " ";
	cout << ")" << endl;
	cout << "v1 - v2 = (";
	v1.operator-(v2);
	for (size_t j = 0; j < v1.size(); j++)
		cout << v1.operator[](j) << " ";
	cout << ")" << endl;

	cout << "v1 * v2 = (";
	v1.operator*(v2);
	for (size_t j = 0; j < v1.size(); j++)
		cout << v1.operator[](j) << " ";
	cout << ")" << endl;

	cout << "-v1 = (";
	v1.operator-();
	for (size_t j = 0; j < v1.size(); j++)
		cout << v1.operator[](j) << " ";
	cout << ")" << endl;

	/* operator= & operator+= & operator== & operator() Test */
	cout << endl;
	cout << "---operator= & operator+= & operator== & operator() & clear() Test---" << endl;

	cout << "<<< v1 = v2;" << endl;
	v1.operator=(v2);
	cout << ">>> v1 = (";
	for (size_t i = 0; i < v1.size(); i++)
		cout << v1.operator[](i) << " ";
	cout << ")" << endl;

	cout << "<<< v1 += v2;" << endl;
	v1.operator+=(v2);
	cout << ">>> v1 = (";
	for (size_t i = 0; i < v1.size(); i++)
		cout << v1.operator[](i) << " ";
	cout << ")" << endl;

	cout << ">>> v1 == v2 is : ";
	cout << v1.operator==(v2) << endl;

	cout << "<<< (double)v1;" << endl;
	v1.operator()();
	cout << ">>> v1 = (";
	for (size_t i = 0; i < v1.size(); i++)
		cout << v1.operator[](i) << " ";
	cout << ")" << endl;

	cout << "<<< v2.clear();" << endl;
	v2.clear();
	cout << ">>> The size of v2 is : " << v2.size() << endl;

	cout << "--------------------------------------------------------------------" << endl;

	cout << endl;

	return 0;
}
