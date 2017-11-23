/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 * This code was updated in Mac.
 *
 * Last Updated : 2017/09/27 20:30
 */
#include <cstdlib>
#include "MyDoubleVector.h"

MyDoubleVector::MyDoubleVector(void)
{
    size = 0;
    capacity = 0;
    data = new double[capacity];
}

MyDoubleVector::MyDoubleVector(const size_t c)
{
    size = 0;
    capacity = c;
    data = new double[capacity];
}

MyDoubleVector::MyDoubleVector(const MyDoubleVector & v)
{
    size = v.size;
    capacity = v.capacity;
    data = new double[capacity];

    for (size_t i = 0; i < size; i++)
        data[i] = v.data[i];
}

MyDoubleVector::~MyDoubleVector(void)
{
    size = 0;
    capacity = 0;
    delete[] data;
}

size_t MyDoubleVector::getSize(void) const
{
    return size;
}

size_t MyDoubleVector::getCapacity(void) const
{
    return capacity;
}

bool MyDoubleVector::isEmpty(void) const
{
    if (size == 0)
        return true;

    else
        return false;
}

void MyDoubleVector::push_back(double x)
{
    if (size < capacity)
        data[size++] = x;

    else
    {
        reserve(getCapacity() + 5);
        push_back(x);
    }
}

void MyDoubleVector::pop_back(void)
{
    size--;
}

void MyDoubleVector::reserve(size_t n)
{
    double * buffer;

    capacity = n;
    buffer = new double[capacity];

    for (size_t i = 0; i < size; i++)
        buffer[i] = data[i];

    delete[] data;
    data = buffer;
}

void MyDoubleVector::clear(void)
{
    size = 0;
    capacity = 0;
    data = NULL;
}

MyDoubleVector MyDoubleVector::operator()(void)
{
    MyDoubleVector v;

    for (size_t i = 0; i < size; i++)
        v.push_back((double)(data[i]));

    return v;
}

MyDoubleVector MyDoubleVector::operator+(const MyDoubleVector & right)
{
    if (size == right.size)
    {
        MyDoubleVector v;

        for (size_t i = 0; i < size; i++)
            v.push_back(data[i] + right.operator[](i));

        return v;
    }
}

MyDoubleVector MyDoubleVector::operator-(void)
{
    MyDoubleVector v;

    for (size_t i = 0; i < size; i++)
        v.push_back(-1 * data[i]);

    return v;
}

MyDoubleVector MyDoubleVector::operator-(const MyDoubleVector & right)
{
    if (size == right.size)
    {
        MyDoubleVector v;

        for (size_t i = 0; i < size; i++)
            v.push_back(data[i] - right.operator[](i));

        return v;
    }
}

MyDoubleVector MyDoubleVector::operator*(const MyDoubleVector & right)
{
    if (size == right.size)
    {
        MyDoubleVector v;

        for (size_t i = 0; i < size; i++)
            v.push_back(data[i] * right.operator[](i));

        return v;
    }
}

MyDoubleVector & MyDoubleVector::operator=(const MyDoubleVector & right)
{
    size = right.size;
    capacity = right.capacity;

    for (size_t i = 0; i < size; i++)
        data[i] = right.data[i];

    return *this;
}

MyDoubleVector & MyDoubleVector::operator+=(const MyDoubleVector & right)
{
    if (size == right.size)
    {
        for (size_t i = 0; i < size; i++)
            data[i] += right.data[i];

        return *this;
    }
}

bool MyDoubleVector::operator==(const MyDoubleVector & right) const
{
    if (size == right.size)
    {
        for (size_t i = 0; i < size; i++)
        {
            if (data[i] != right.operator[](i))
                return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

double MyDoubleVector::operator[](unsigned int index) const
{
    if (index < size)
        return data[index];
}
