#include <iostream>
#include <string>
#include <vector>

using namespace std;

class MyPuzzle
{
public:
	MyPuzzle();
	~MyPuzzle();
	string getPuzzle(void);
	void setPuzzle(int n);
	bool solvePuzzle(string puzzle, int hole);
	vector<int> getSolution(void);

private:
	int n;					// 입력 크기
	bool solve;				// 해결 여부
	string puzzle;			// 퍼즐
	string answer;			// 정답
	vector<int> solution;	// 풀이
};

int main(void)
{
	int count = 1;
	int n = 0;
	
	cout << ">>> Hello World!" << endl;

	while (true)
	{
		count = 1;

		cout << "<<< n = ";
		cin >> n;

		if ((n >= 1) && (n <= 12))
		{
			MyPuzzle myPuzzle = MyPuzzle();
			myPuzzle.setPuzzle(n);

			string puzzle = myPuzzle.getPuzzle();
			cout << ">>> Puzzle = " << puzzle << endl;

			cout << ">>> Solving... ";
			myPuzzle.solvePuzzle(puzzle, n);

			vector<int> solution = myPuzzle.getSolution();
			cout << ">>> Total = " << solution.size() << endl;
			cout << ">>> Move = ";
			for (auto i = solution.rbegin(); i < solution.rend(); i++)
			{
				cout << *i + 1 << " ";

				if (count == 20)
				{
					cout << endl;
					count = 1;
				}
				else
					count++;
			}
			cout << endl;
		}
		else if (n == 0)
		{
			cout << ">>> Bye!" << endl;
			break;
		}
		else
		{
			cout << ">>> Input Error!" << endl;
		}
	}

	return 0;
}

MyPuzzle::MyPuzzle()
{
	this->n = 0;
	this->puzzle = "";
	this->answer = "";
	this->solve = false;
}

MyPuzzle::~MyPuzzle()
{
	this->n = 0;
	this->puzzle.clear();
	this->answer.clear();
	this->solve = true;
	this->solution.clear();
}

string MyPuzzle::getPuzzle(void)
{
	return this->puzzle;
}

void MyPuzzle::setPuzzle(int n)
{
	this->n = n;

	for (int i = 0; i < (2 * n) + 1; i++)
	{
		if (i < n)
		{
			this->puzzle.append("W");
			this->answer.append("B");
		}
		else if (i == n)
		{
			this->puzzle.append(" ");
			this->answer.append(" ");
		}
		else
		{
			this->puzzle.append("B");
			this->answer.append("W");
		}
	}

	return;
}

bool MyPuzzle::solvePuzzle(string puzzle, int hole)
{
	if (puzzle == this->answer)
	{
		this->solve = true;
		cout << "Solved!" << endl;

		return true;
	}
	else if (this->solve)
	{
		return false;
	}
	else
	{
		/* 백돌의 옆이 구멍인 경우 - 이동 */
		if ((hole > 0) && (puzzle.at(hole - 1) == 'W'))
		{
			string temp = puzzle;
			temp[hole] = 'W';
			temp[hole - 1] = ' ';

			if (this->solvePuzzle(temp, hole - 1))
			{
				this->solution.push_back(hole - 1);

				return true;
			}
		}

		/* 백돌의 옆 옆이 구멍인 경우 - 점프 */
		if ((hole > 1) && (puzzle.at(hole - 2) == 'W'))
		{
			string temp = puzzle;
			temp[hole] = 'W';
			temp[hole - 2] = ' ';

			if (this->solvePuzzle(temp, hole - 2))
			{
				this->solution.push_back(hole - 2);

				return true;
			}
		}

		/* 흑돌의 옆이 구멍인 경우 - 이동 */
		if ((hole < (2 * this->n)) && (puzzle.at(hole + 1) == 'B'))
		{
			string temp = puzzle;
			temp[hole] = 'B';
			temp[hole + 1] = ' ';

			if (this->solvePuzzle(temp, hole + 1))
			{
				this->solution.push_back(hole + 1);

				return true;
			}
		}

		/* 백돌의 옆 옆이 구멍인 경우 - 점프 */
		if ((hole < (2 * this->n - 1)) && (puzzle.at(hole + 2) == 'B'))
		{
			string temp = puzzle;
			temp[hole] = 'B';
			temp[hole + 2] = ' ';

			if (this->solvePuzzle(temp, hole + 2))
			{
				this->solution.push_back(hole + 2);

				return true;
			}
		}
	}

	return false;
}

vector<int> MyPuzzle::getSolution(void)
{
	return this->solution;
}
