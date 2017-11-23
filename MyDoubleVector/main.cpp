/*
 * Name : HwiYeon Cho
 * E-mail : hwyn.cho@gmail.com
 *
 * This code was written in Windows.
 * This code was updated in Mac.
 *
 * Last Updated : 2017/09/27 20:30
 */
#include <cstdlib>
#include <iostream>
#include "MyDoubleVector.h"

using namespace std;

void printVector(MyDoubleVector v);

int main()
{
    MyDoubleVector v1;
    MyDoubleVector v2;

    /* isEmpty() Test */
    cout << endl;
    cout << "----------isEmpty() Test----------" << endl;

    cout << ">>> The object v1 is empty? : " << v1.isEmpty() << endl;
    cout << ">>> The object v2 is empty? : " << v2.isEmpty() << endl;

    cout << "--------------------------------" << endl;

    /* reserve() & capacity() Test */
    cout << endl;
    cout << "--reserve() & capacity() Test--" << endl;

    cout << "<<< v1.reserve(4);" << endl;
    v1.reserve(4);
    cout << ">>> The capacity of v1 is : " << v1.getCapacity() << endl;

    cout << "<<< v2.reserve(3);" << endl;
    v2.reserve(3);
    cout << ">>> The capacity of v2 is : " << v2.getCapacity() << endl;

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
    printVector(v1);
    cout << ")" << endl;

    cout << ">>> The size of v1 is : " << v1.getSize() << endl;

    cout << "<<< v2.push_back(9);" << endl;
    v2.push_back(9);
    cout << "<<< v2.push_back(8);" << endl;
    v2.push_back(8);

    cout << ">>> v2 = (";
    printVector(v2);
    cout << ")" << endl;

    cout << ">>> The size of v2 is : " << v2.getSize() << endl;

    cout << "<<< v1.pop_back();" << endl;
    v1.pop_back();

    cout << ">>> v1 = (";
    printVector(v1);
    cout << ")" << endl;

    cout << ">>> The size of v1 is : " << v1.getSize() << endl;

    cout << "--------------------------------------------------------" << endl;

    /* operator+ & operator- & operator* & operator- Test */
    cout << endl;
    cout << "---operator+ & operator- & operator* & operator- Test---" << endl;

    cout << "v1 + v2 = (";
    printVector(v1.operator+(v2));
    cout << ")" << endl;

    cout << "v1 - v2 = (";
    printVector(v1.operator-(v2));
    cout << ")" << endl;

    cout << "v1 * v2 = (";
    printVector(v1.operator*(v2));
    cout << ")" << endl;

    cout << "-v1 = (";
    printVector(v1.operator-());
    cout << ")" << endl;

    /* operator= & operator+= & operator== & operator() Test */
    cout << endl;
    cout << "---operator= & operator+= & operator== & operator() & clear() Test---" << endl;

    cout << "<<< v1 = v2;" << endl;
    v1.operator=(v2);
    cout << ">>> v1 = (";
    printVector(v1);
    cout << ")" << endl;

    cout << "<<< v1 += v2;" << endl;
    v1.operator+=(v2);
    cout << ">>> v1 = (";
    printVector(v1);
    cout << ")" << endl;

    cout << ">>> v1 == v2 is : ";
    cout << v1.operator==(v2) << endl;

    cout << "<<< (double)v1;" << endl;
    cout << ">>> v1 = (";
    printVector(v1.operator()());
    cout << ")" << endl;

    cout << "<<< v2.clear();" << endl;
    v2.clear();
    cout << ">>> The size of v2 is : " << v2.getSize() << endl;

    cout << "--------------------------------------------------------------------" << endl;

    cout << endl;

    return 0;
}

void printVector(MyDoubleVector v)
{
    for (size_t i = 0; i < v.getSize(); i++)
    {
        if (i != 0)
            cout << " ";

        cout << v.operator[](i);
    }
}
