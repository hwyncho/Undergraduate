#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>

using namespace std;
using namespace stdext;

int main(void)
{
	cout << ">>> Hello World!" << endl;
	
	string line;

	getline(cin, line);
	int n = stoi(line);

	getline(cin, line);
	string input = line;

	unordered_map<string, int> countMap;		// key = 문자열, value = 개수

	int length = input.length();
	string substr = "";

	cout << ">>> Counting... ";

	clock_t startTime = clock();

	for (int i = 0; i < (length - n + 1); i++)
	{
		substr.assign(input, i, n);		// .assign(문자열, 시작위치, 개수)
		countMap[substr] += 1;
	}

	int max = 0;
	string output = "";

	for (auto i = countMap.begin(); i != countMap.end(); i++)
	{
		if (max < i->second)
		{
			max = i->second;
			output = i->first;
		}
	}

	clock_t endTime = clock();

	cout << "Complete!" << endl;

	cout << ">>> Output = <" << output << ", " << max << ">" << endl;

	cout << ">>> " << (endTime - startTime) << "ms" << endl;
}