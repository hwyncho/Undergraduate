/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:12
 */

#ifndef __MyDoubleVector_H__
#define __MyDoubleVector_H__

class MyDoubleVector
{
public:
	MyDoubleVector(void);
	MyDoubleVector(const MyDoubleVector & v);
	~MyDoubleVector(void);

	size_t size(void) const;
	size_t capacity(void) const;
	bool empty(void) const;
	void push_back(double x);
	void pop_back(void);
	void reserve(size_t n);
	void clear(void);

	MyDoubleVector & operator=(const MyDoubleVector & right);
	MyDoubleVector & operator+=(const MyDoubleVector & right);
	double & operator[](unsigned int index) const;
	MyDoubleVector & operator+(const MyDoubleVector & right);
	MyDoubleVector & operator-(const MyDoubleVector & right);
	MyDoubleVector & operator*(const MyDoubleVector & right);
	void operator-(void) const;
	bool operator==(const MyDoubleVector & right);
	MyDoubleVector & operator()(void);

private:
	size_t mySize;
	size_t myCapacity;
	double * myData;
};

#endif // !__MyDoubleVector_H__