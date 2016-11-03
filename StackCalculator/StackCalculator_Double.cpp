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

class StackCalculator
{
private:
	string formula;
	vector<string> infix;
	vector<string> postfix;

public:
	StackCalculator();
	int priority(string op);
	int checkBracket();
	void setFormula(string input);
	void setInfix();
	void setPostfix();
	double calculate();
};

int main(void)
{
	string input;
	StackCalculator calculator = StackCalculator();

	while (true)
	{
		cout << "<<< Input : ";
		input.clear();
		cin >> input;

		if (input.compare("EOI") == 0)
			return 0;

		else
		{
			calculator.setFormula(input);

			switch (calculator.checkBracket())
			{
			case -1:
				cout << ">>> Error!" << endl;
				break;

			case 0:
				cout << ">>> Result : " << calculator.calculate() << endl;
			}
		}
	}

	return 0;
}

StackCalculator::StackCalculator()
{
	formula.clear();
	infix.clear();
	postfix.clear();
}

int StackCalculator::priority(string op)
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

int StackCalculator::checkBracket()
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

void StackCalculator::setFormula(string input)
{
	formula.clear();
	formula = input;
}

void StackCalculator::setInfix() {
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

void StackCalculator::setPostfix()
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

double StackCalculator::calculate()
{
	unsigned int i = 0;
	double left = 0;
	double right = 0;
	stack<string> tempStack;

	infix.clear();
	postfix.clear();

	setInfix();
	setPostfix();

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