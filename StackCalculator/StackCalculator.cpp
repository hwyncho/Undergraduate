/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/04/04 02:13
 */

#include <iostream>
#include <fstream>
#include <stack>
#include <string>

using namespace std;

int loadOperators(char(*_myOp)[10]);
int priority(char _op);
int checkBalance(const string & _expr);
string makePostfix(const string & _expr);
void calculate(char(*_myOp)[10], const string & _postfix);

int main(void)
{
	char myOp[3][10];
	string expr, postfix;

	switch (loadOperators(myOp))
	{
	case -1:
		cout << ">>> operators.txt does not exist! (Press ENTER key...)";
		cin.get();
		return 1;

	case 0:
		while (true)
		{
			cout << "Input expressions : ";
			cin >> expr;

			if (expr.compare("EOI") == 0)
				return 0;

			else
			{
				switch (checkBalance(expr))
				{
				case -1:
					cout << ">>> Error!" << endl;
					break;

				case 0:
					postfix = makePostfix(expr);
					cout << ">>> Postfix : " << postfix << endl;
					calculate(myOp, postfix);
				}
			}
		}
	}

	return 0;
}

int loadOperators(char(*_myOp)[10])
{
	ifstream file;

	file.open("operators.txt");

	if (file.is_open() == false)
		return -1;

	else
	{
		for (unsigned int i = 0; i < 3; i++)
		{
			for (unsigned int j = 0; j < 10; j++)
				file >> _myOp[i][j];
		}

		file.close();

		return 0;
	}
}

int priority(char _op)
{
	switch (_op)
	{
	case '@': case '#':
		return 0;

	case '&':	return 1;

	case '(':	return 2;

	default:	return -1;
	}
}

int checkBalance(const string & _expr)
{
	int count = 0;

	for (unsigned int i = 0; i < _expr.length(); i++)
	{
		switch (_expr[i])
		{
		case '(':
			count++;
			break;

		case ')':
			count--;
			break;
		}
	}

	if (count != 0)
		return -1;

	else
		return 0;
}

string makePostfix(const string & _expr)
{
	unsigned int i = 0;
	char temp;
	string postfix;
	stack<char> tempStack;

	while (i < _expr.length())
	{
		switch (_expr[i])
		{
		case 'a': case 'b': case 'c':
			postfix += _expr[i++];
			break;

		case '@': case '#': case '&':
			while (!tempStack.empty() && (tempStack.top() != '(') && (priority(tempStack.top()) >= priority(_expr[i])))
			{
				postfix += tempStack.top();
				tempStack.pop();
			}

			tempStack.push(_expr[i++]);
			break;

		case '(':
			tempStack.push(_expr[i++]);
			break;

		case ')':
			while (true)
			{
				temp = tempStack.top();
				tempStack.pop();

				if (temp == '(')
					break;

				else
					postfix += temp;
			}

			i++;
			break;
		}
	}

	while (!tempStack.empty())
	{
		postfix += tempStack.top();
		tempStack.pop();
	}

	return postfix;
}

void calculate(char(*_myOp)[10], const string & _postfix)
{
	unsigned int i = 0;
	int name, result;
	char left, right;
	stack<char> tempStack;

	while (i < _postfix.length())
	{
		switch (_postfix[i])
		{
		case 'a': case 'b': case 'c':
			tempStack.push(_postfix[i++]);
			break;

		case '@': case '#': case '&':
			right = tempStack.top();
			tempStack.pop();

			left = tempStack.top();
			tempStack.pop();

			switch (_postfix[i])
			{
			case '@':	name = 0;	break;

			case '#':	name = 1;	break;

			case '&':	name = 2;	break;
			}

			switch (left)
			{
			case 'a':
				switch (right)
				{
				case 'a':	result = 1;	break;

				case 'b':	result = 4; break;

				case 'c':	result = 7;	break;
				}

			case 'b':
				switch (right)
				{
				case 'a':	result = 2;	break;

				case 'b':	result = 5; break;

				case 'c':	result = 8;	break;
				}

			case 'c':
				switch (right)
				{
				case 'a':	result = 3;	break;

				case 'b':	result = 6; break;

				case 'c':	result = 9;	break;
				}
			}

			tempStack.push(_myOp[name][result]);
			i++;
		}
	}

	cout << ">>> Result : " << tempStack.top() << endl;
}