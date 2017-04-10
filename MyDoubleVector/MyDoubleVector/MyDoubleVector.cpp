/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:12
 */

#include <cstdlib>
#include <iostream>
#include "MyDoubleVector.h"

using namespace std;

MyDoubleVector::MyDoubleVector(void)
{
	mySize = 0;
	myCapacity = 0;
	myData = new double[myCapacity];
}

MyDoubleVector::MyDoubleVector(const MyDoubleVector & v)
{
	mySize = v.mySize;
	myCapacity = v.myCapacity;
	myData = new double[myCapacity];

	for (size_t i = 0; i < mySize; i++)
		myData[i] = v.myData[i];
}

MyDoubleVector::~MyDoubleVector(void)
{
	mySize = 0;
	myCapacity = 0;
	delete[] myData;
}

size_t MyDoubleVector::size(void) const
{
	return mySize;
}

size_t MyDoubleVector::capacity(void) const
{
	return myCapacity;
}

bool MyDoubleVector::empty(void) const
{
	if (mySize == 0)
		return true;

	else
		return false;
}

void MyDoubleVector::push_back(double x)
{
	if (mySize < myCapacity)
		myData[mySize++] = x;

	else
	{
		reserve(capacity() + 5);
		push_back(x);
	}
}

void MyDoubleVector::pop_back(void)
{
	mySize--;
}

void MyDoubleVector::reserve(size_t n)
{
	double * buffer;

	myCapacity = n;
	buffer = new double[myCapacity];

	for (size_t i = 0; i < mySize; i++)
		buffer[i] = myData[i];

	delete[] myData;
	myData = buffer;
}

void MyDoubleVector::clear(void)
{
	mySize = 0;
	myCapacity = 0;
	myData = NULL;
}

MyDoubleVector & MyDoubleVector::operator=(const MyDoubleVector & right)
{
	mySize = right.mySize;
	myCapacity = right.myCapacity;

	for (size_t i = 0; i < mySize; i++)
		myData[i] = right.myData[i];

	return *this;
}

MyDoubleVector & MyDoubleVector::operator+=(const MyDoubleVector & right)
{
	for (size_t i = 0; i < right.mySize; i++)
		push_back(right.operator[](i));

	return *this;
}

double & MyDoubleVector::operator[](unsigned int index) const
{
	if (index < mySize)
		return myData[index];

	else
		;
}

MyDoubleVector & MyDoubleVector::operator+(const MyDoubleVector & right)
{
	if (mySize == right.mySize)
	{
		for (size_t i = 0; i < mySize; i++)
			myData[i] += right.operator[](i);

		return *this;
	}

	else
		;
}

MyDoubleVector & MyDoubleVector::operator-(const MyDoubleVector & right)
{
	if (mySize == right.mySize)
	{
		for (size_t i = 0; i < mySize; i++)
			myData[i] -= right.operator[](i);

		return *this;
	}

	else
		;
}

MyDoubleVector & MyDoubleVector::operator*(const MyDoubleVector & right)
{
	if (mySize == right.mySize)
	{
		for (size_t i = 0; i < mySize; i++)
			myData[i] *= right.operator[](i);

		return *this;
	}

	else
		;
}

void MyDoubleVector::operator-(void) const
{
	for (size_t i = 0; i < mySize; i++)
		myData[i] = -(myData[i]);
}

bool MyDoubleVector::operator==(const MyDoubleVector & right)
{
	for (size_t i = 0; i < mySize; i++)
	{
		if (myData[i] != right.operator[](i))
			return false;
	}

	return true;
}

MyDoubleVector & MyDoubleVector::operator()(void)
{
	for (size_t i = 0; i < mySize; i++)
		myData[i] = (double)(myData[i]);

	return *this;
}