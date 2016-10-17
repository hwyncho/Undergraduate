#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Child
{
private:
	int id;				// 아이의 ID
	double choice;		// 아이가 선택한 무게
	double allot;		// 아이가 받을 무게

public:
	Child();
	Child(int _id, int _weight);
	int getId(void);					// 아이의 ID를 반환
	double getChoice(void);				// 아이가 선택한 무게를 반환
	double getAllot(void);				// 아이가 받을 무게를 반환
	void setAllot(double _weight);		// 아이가 받을 무게를 설정
	void showInfo(string str);			// 아이의 정보를 출력
};

class Weight
{
private:
	double weight;		// 무게
	int count;			// 무게별 선택한 인원수

public:
	Weight();
	Weight(double _weight, int _count);
	double getWeight(void);		// 무게를 반환
	int getCount(void);			// 인원수를 반환
};

void setChildList(vector<Child> & _childList);								// 아이의 ID와 무게를 선택
vector<Weight> setWeightList(vector<Child> & _childList);					// 무게별 선택한 인원수를 조사
void handOut(vector<Child> & _childList, vector<Weight> & _weightList);		// 아이에게 초콜릿을 나눠줌
bool byChoice(Child child_1, Child child_2);								// 정렬 기준 : 아이가 선택한 무게
bool byAllot(Child child_1, Child child_2);									// 정렬 기준 : 아이가 받을 무게

int main(void)
{
	vector<Child> childList;		// 아이의 정보를 담는 리스트
	vector<Weight> weightList;		// 무게별 선택한 인원수를 담는 리스트
	
	// childList 생성
	setChildList(childList);

	// childList를 선택한 무게를 기준으로 오름차순 정렬
	sort(childList.begin(), childList.end(), byChoice);
	
	// 선택한 무게별 인원수 검사
	weightList = setWeightList(childList);
	
	handOut(childList, weightList);

	// childList를 받은 무게를 기준으로 오름차순 정렬
	sort(childList.begin(), childList.end(), byAllot);

	cout << endl << "[결과]" << endl;

	for (size_t i = 0; i < childList.size(); i++)
		childList[i].showInfo("allot");

	return 0;
}

/* Child 클래스 멤버 정의 - start */
Child::Child()
{
	id = 0;
	choice = 0;
	allot = 0;
}

Child::Child(int _id, int _weight)
{
	id = _id;
	choice = _weight;
	allot = 0;
}

int Child::getId(void)
{
	return id;
}

double Child::getChoice(void)
{
	return choice;
}

double Child::getAllot(void)
{
	return allot;
}

void Child::setAllot(double _weight)
{
	allot = _weight;
}

void Child::showInfo(string str)
{
	cout << "ID : " << id;
	cout << ", ";

	if (str == "choice")
		cout << "선택한 무게(g) : " << choice;
	else if (str == "allot")
		cout << "받은 무게(g) : " << allot;

	cout << endl;
}
/* Child 클래스 멤버 정의 - end */

/* Weight 클래스 멤버 정의 - start */
Weight::Weight()
{
	weight = 0;
	count = 0;
}

Weight::Weight(double _weight, int _count)
{
	weight = _weight;
	count = _count;
}

double Weight::getWeight(void)
{
	return weight;
}

int Weight::getCount(void)
{
	return count;
}
/* Weight 클래스 멤버 정의 - end */

// childList 생성 함수
void setChildList(vector<Child> & _childList)
{
	int num = 0;
	int id = 0;
	int weight = 0;

	cout << "아이들은 몇 명? ";
	cin >> num;

	/* 아이들의 ID와 선택 */
	for (int i = 0; i < num; i++)
	{
		cout << "ID, 무게 선택(1 ~ 1000000g) : ";
		cin >> id >> weight;

		_childList.push_back(Child(id, weight));
	}
}

// weightList 생성 함수
vector<Weight> setWeightList(vector<Child> & _childList)
{
	size_t i = 0;
	size_t j = 0;

	double weight = 0;		// 현재 선택된 무게
	int count = 0;			// 현재 선택된 무게의 인원수
	int total = 0;			// 총 인원수
	vector<Weight> weightList;

	/*
	 * 작동 방식
	 * 입력 : _childList는 선택한 무게가 적은 순으로 정렬되어 있음
	 * 무게를 선택하고, 리스트에서 그 무게와 같은 무게가 있을 때마다 count를 증가
	 * weightList에 무게와 그 무게의 인원수를 순서대로 저장
	 * 이전 무게 인원수만큼, 인덱스를 건너뜀
	 */
	while ((i < _childList.size()) && (total < _childList.size()))
	{
		weight = _childList[i].getChoice();		// 무게를 선택
		count = 0;								// 선택된 무게의 인원수를 0으로 초기화

		j = i;

		// 리스트에서 같은 무게가 있는지 검사
		while (j < _childList.size())
		{
			// 같은 무게가 있으면 count 증가
			if (weight == _childList[j].getChoice())
				count++;

			j++;
		}

		// weightList에 기록
		weightList.push_back(Weight(weight, count));

		i = count;
		total += count;
	}
	
	return weightList;
}

void handOut(vector<Child> & _childList, vector<Weight> & _weightList)
{
	double allot = 0;		// 받을 무게
	double weight = 0;		// 무게
	int count = 0;			// 무게별 인원수
	int check = 0;			// 받은 인원수

	size_t j = 0;

	for (size_t i = 0; i < _childList.size(); i++)
	{
		weight = _weightList[j].getWeight();
		count = _weightList[j].getCount();
		allot = weight / count;
		
		_childList[i].setAllot(allot);		// 아이에게 초콜릿 선물

		check++;							// 현재 무게를 받은 인원수 증가

		// 현재 무게를 받을 인원수와 받은 인원수가 같아지면
		// 다음 무게로 넘어가고, 받은 인원수를 0으로 초기화
		if (check == count)
		{
			j++;
			check = 0;
		}
	}
}

bool byChoice(Child child_1, Child child_2)
{
	return child_1.getChoice() < child_2.getChoice();
}

bool byAllot(Child child_1, Child child_2)
{
	return child_1.getAllot() < child_2.getAllot();
}