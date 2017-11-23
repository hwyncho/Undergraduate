/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 * This code was updated in Mac.
 *
 * Last Updated : 2017/09/27 20:30
 */
#ifndef __MyDoubleVector_H__
#define __MyDoubleVector_H__

class MyDoubleVector
{
public:
    MyDoubleVector(void);
    MyDoubleVector(const size_t c);
    MyDoubleVector(const MyDoubleVector & v);
    ~MyDoubleVector(void);

    size_t getSize(void) const;
    size_t getCapacity(void) const;
    bool isEmpty(void) const;
    void push_back(double x);
    void pop_back(void);
    void reserve(size_t n);
    void clear(void);

    MyDoubleVector operator()(void);
    MyDoubleVector operator+(const MyDoubleVector & right);
    MyDoubleVector operator-(void);
    MyDoubleVector operator-(const MyDoubleVector & right);
    MyDoubleVector operator*(const MyDoubleVector & right);
    MyDoubleVector & operator=(const MyDoubleVector & right);
    MyDoubleVector & operator+=(const MyDoubleVector & right);
    bool operator==(const MyDoubleVector & right) const;
    double operator[](unsigned int index) const;

private:
    size_t size;
    size_t capacity;
    double * data;
};

#endif // !__MyDoubleVector_H__
