/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Windows.
 *
 * Last Updated : 2016/11/03 03:08
 */

#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

int priority(string op);
int checkBalance();
void splitFormula();
void makePostfix();
double calculate();

string formula;
vector<string> infix;
vector<string> postfix;

int main(void)
{
	while (true)
	{
		formula.clear();
		infix.clear();
		postfix.clear();

		cout << "<<< Input expressions : ";
		cin >> formula;

		if (formula.compare("EOI") == 0)
			return 0;

		else
		{
			switch (checkBalance())
			{
			case -1:
				cout << ">>> Error!" << endl;
				break;

			case 0:
				cout << ">>> Result : " << calculate() << endl;
			}
		}
	}

	return 0;
}

int priority(string op)
{
	if (op == "(")
		return 2;
	else if ((op == "*") || (op == "/"))
		return 1;
	else if ((op == "+") || (op == "-"))
		return 0;
	else
		return -1;
}

int checkBalance()
{
	int count = 0;

	for (unsigned int i = 0; i < formula.length(); i++)
	{
		switch (formula[i])
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

void splitFormula() {
	unsigned int i = 0;

	string temp;
	temp.clear();

	while (i < formula.length())
	{
		switch (formula.at(i)) {
		case '(':
			infix.push_back(temp);
			infix.push_back("(");
			temp.clear();
			break;

		case ')':
			infix.push_back(temp);
			infix.push_back(")");
			temp.clear();
			break;

		case '+':
			infix.push_back(temp);
			infix.push_back("+");
			temp.clear();
			break;

		case '-':
			infix.push_back(temp);
			infix.push_back("-");
			temp.clear();
			break;

		case '*':
			infix.push_back(temp);
			infix.push_back("*");
			temp.clear();
			break;

		case '/':
			infix.push_back(temp);
			infix.push_back("/");
			temp.clear();
			break;

		case '0': case '1': case '2':
		case '3': case '4': case '5':
		case '6': case '7': case '8':
		case '9': case '.':
			temp += formula.at(i);
			break;
		}

		i++;
	}

	infix.push_back(temp);
}

void makePostfix()
{
	unsigned int i = 0;
	string temp;
	stack<string> tempStack;

	while (i < infix.size())
	{
		if (infix.at(i) == "")
		{
			i++;
		}
		else if (infix.at(i) == "(")
		{
			tempStack.push(infix.at(i++));
		}
		else if (infix.at(i) == ")")
		{
			while (true)
			{
				temp = tempStack.top();
				tempStack.pop();

				if (temp == "(")
					break;
				else
					postfix.push_back(temp);
			}

			i++;
		}
		else if ((infix.at(i) == "+") || (infix.at(i) == "-") || (infix.at(i) == "*") || (infix.at(i) == "/"))
		{
			while (!tempStack.empty() && (tempStack.top() != "(") && (priority(tempStack.top()) >= priority(infix.at(i))))
			{
				postfix.push_back(tempStack.top());
				tempStack.pop();
			}

			tempStack.push(infix.at(i++));
		}
		else
		{
			postfix.push_back(infix.at(i++));
		}
	}

	while (!tempStack.empty())
	{
		postfix.push_back(tempStack.top());
		tempStack.pop();
	}
}

double calculate()
{
	unsigned int i = 0;
	double left = 0;
	double right = 0;
	stack<string> tempStack;

	infix.clear();
	postfix.clear();

	splitFormula();
	makePostfix();

	while (i < postfix.size())
	{
		if (postfix.at(i) == "+") {
			right = stod(tempStack.top());
			tempStack.pop();

			left = stod(tempStack.top());
			tempStack.pop();

			tempStack.push(to_string(left + right));
		}
		else if (postfix.at(i) == "-")
		{
			right = stod(tempStack.top());
			tempStack.pop();

			left = stod(tempStack.top());
			tempStack.pop();

			tempStack.push(to_string(left - right));
		}
		else if (postfix.at(i) == "*")
		{
			right = stod(tempStack.top());
			tempStack.pop();

			left = stod(tempStack.top());
			tempStack.pop();

			tempStack.push(to_string(left * right));
		}
		else if (postfix.at(i) == "/")
		{
			right = stod(tempStack.top());
			tempStack.pop();

			left = stod(tempStack.top());
			tempStack.pop();

			tempStack.push(to_string(left / right));
		}
		else
			tempStack.push(postfix.at(i));

		i++;
	}

	return stod(tempStack.top());
}