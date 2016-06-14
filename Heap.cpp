/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code implements Heap.
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:14
 */

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

class comparator
{
public:
	bool operator()(const int x, const int y) { return x > y; }
};

using namespace std;

bool rangeCheck(int n);
void insertNode(vector<char> & heap, const char data);
void removeRoot(vector<char> & heap);
void swapNode(char * node1, char * node2);
void original(vector<char> & heap);
void square(vector<char> & heap);
void make(vector<char> & heap, vector< vector<char> > & table, int y, int x);
void print(vector< vector<char> > & table, unsigned int y, unsigned int x, int mode);

int main(void)
{
	unsigned int i;
	int size;
	string input;
	vector<char> heap;

	while (true)
	{
		system("cls");

		cout << "<<< Please ENTER the size of heap (1 ~ 300) : ";
		cin >> size;
		cin.ignore();

		if (rangeCheck(size) == true)
		{
			heap.reserve(size);
			make_heap(heap.begin(), heap.end(), comparator());
			break;
		}

		else
		{
			cout << ">>> The input value is not valid! (Press ENTER key...)";
			cin.get();
		}
	}

INPUT:
	while (true)
	{
		cout << "<<< INPUT : ";
		getline(cin, input);

		i = 0;
		while (i < input.size())
		{
			switch (input[i])
			{
			case 'P':
				if ('1' <= input[i + 1] && input[i + 1] <= '9')
					insertNode(heap, input[i + 1]);

				else if ('A' <= input[i + 1] && input[i + 1] <= 'Z')
					insertNode(heap, input[i + 1]);

				else if ('a' <= input[i + 1] && input[i + 1] <= 'z')
					insertNode(heap, input[i + 1]);

				else
					goto INPUT;

				i += 2;
				break;

			case 'R':
				removeRoot(heap);
				i += 1;
				break;

			case ' ':
				i += 1;
				break;
			}
		}

		original(heap);
		square(heap);
	}
}

bool rangeCheck(int n)
{
	if (1 <= n && n <= 300) { return true; }
	else { return false; }
}

void insertNode(vector<char> & heap, const char data)
{
	heap.push_back(data);
	push_heap(heap.begin(), heap.end(), comparator());
}

void removeRoot(vector<char> & heap)
{
	pop_heap(heap.begin(), heap.end(), comparator());
	heap.pop_back();
}

void original(vector<char> & heap)
{
	unsigned int y, x = 0;
	unsigned int blank, upblank;
	unsigned int index = 0;

	y = (unsigned int)log2(heap.size()) + 1;
	for (unsigned int i = 0; i < y; i++)
		x += (unsigned int)pow(2, i);

	vector< vector<char> > table(y, vector<char>(x, ' '));

	for (unsigned int i = 0; i < y; i++)
	{
		blank = x / (unsigned int)pow(2, i + 1);
		upblank = x / (unsigned int)pow(2, i);

		for (unsigned int j = 0; j < x; j++)
		{
			if (index < heap.size() && j == blank)
			{
				table[i][j] = heap[index++];
				blank += (upblank + 1);
			}
		}
	}

	cout << endl;
	cout << ">>> Original Display" << endl;
	print(table, y, x, 1);
	cout << endl;

	cout << ">>> Rotated Display";
	print(table, y, x, 2);
	cout << endl;
}

void square(vector<char> & heap)
{
	unsigned int y = 1, x = 1;

	for (unsigned int i = 1; i < heap.size(); i++)
	{
		if ((unsigned int)log2(i + 1) % 2 == 0) { y += 2; }
		else { x += 2; }
	}

	vector< vector<char> > table(y, vector<char>(x, ' '));

	make(heap, table, y / 2, x / 2);

	cout << ">>> Square Display" << endl;
	print(table, y, x, 1);
	cout << endl;
}

void make(vector<char> & heap, vector< vector<char> > & table, int y, int x)
{
	static int index = 0;
	int a = 0, b = 0;
	int dirY[6] = { 0, 0, -2, 2, 2, -2 };
	int dirX[6] = { -2, 2, -2, -2, 2, 2 };

	for (int i = 0; i < 6; i++)
	{
		table[y + a][x + b] = heap[index++];
		a = dirY[i];
		b = dirX[i];

		if (index == heap.size())
		{
			index = 0;
			return;
		}
	}

	for (int i = 2; i < 6; i++)
	{
		a = dirY[i];
		b = dirX[i];
		make(heap, table, y + a, x + b);
	}
}

void print(vector< vector<char> > & table, unsigned int y, unsigned int x, int mode)
{
	switch (mode)
	{
	case 1:
		for (unsigned int i = 0; i < y; i++)
		{
			for (unsigned int j = 0; j < x; j++)
				cout << table[i][j];
			cout << endl;
		}
		break;

	case 2:
		for (int i = x; i >= 0; i--)
		{
			for (unsigned int j = 0; j < y; j++)
				cout << table[j][i];
			cout << endl;
		}
		break;
	}
}
