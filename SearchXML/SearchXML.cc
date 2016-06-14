/*
 * StudentID : 2014726096
 * Name : HwiYeon Cho
 *
 * This code was written in Ubuntu.
 *
 * Last Updated : 2016/04/04 02:13
 */

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

using namespace std;

class myXML
{
public:
	void loadXML(void);
	void searching(void);

private:	
	vector<string> state;
	vector<string> city;
	vector<string> address;	
};

void setScr(void);

WINDOW * search_window;
WINDOW * address_window;
WINDOW * city_window;
WINDOW * state_window;

int main(void)
{
	myXML data;
	
	setScr();
		
	data.loadXML();
	data.searching();
	
	endwin();
	
	return 0;
}

void myXML::loadXML(void)
{
	int start, length;
	ifstream file;
	string search;
	
	file.open("zipcode.xml");
	
	if (file.is_open() == false)
	{
		cout << ">>> zipcode.xml is not exist!" << endl;
		
		exit(1);
	}
	
	while(file.eof() != true)
	{
		getline(file, search);
		
		if (search.find("<ZIP_x0020_Code>") != string::npos)
		{
			start = search.find("<ZIP_x0020_Code>") + 16;
			length = search.find("</ZIP_x0020_Code>") - start;
			address.push_back(search.substr(start, length));
		}
		
		else if (search.find("<City>") != string::npos)
		{
			start = search.find("<City>") + 6;
			length = search.find("</City>") - start;
			city.push_back(search.substr(start, length));
		}
		
		else if (search.find("<State_x0020_Abbreviation>") != string::npos)
		{
			start = search.find("<State_x0020_Abbreviation>") + 26;
			length = search.find("</State_x0020_Abbreviation>") - start;
			state.push_back(search.substr(start, length));
		}
	}
}

void myXML::searching(void)
{
	char ch;
	string input;
	
	while (true)
	{
		werase(search_window);
		mvwprintw(search_window, 0, 0, "<<< City : ");
		mvwprintw(search_window, 0, 11, (char *)input.c_str());
		wrefresh(search_window);
		
		ch = wgetch(search_window);
		
		switch (ch)
		{
		case 127:
			if (input.size() > 0)
				input.erase(input.size() - 1);
			break;
			
		default:
			if (isalpha(ch))
				input += toupper(ch);
			break;
		}
		
		werase(address_window);
		werase(city_window);
		werase(state_window);
		
		for (int i = 0; i < city.size(); i++)
		{
			if (input.size() > 0 && city[i].find(input) != string::npos)
			{
				wprintw(address_window, "%s\n", (char *)address[i].c_str());			
				wprintw(city_window, "%s\n", (char *)city[i].c_str());
				wprintw(state_window, "%s\n", (char *)state[i].c_str());
			}
		}
		
		wrefresh(address_window);
		wrefresh(city_window);
		wrefresh(state_window);
	}
}

void setScr(void)
{
	initscr();
	
	if (has_colors() == FALSE)
	{
		printf(">>> Terminal does not support colors!\n");
		endwin();
		exit(1);
	}
	
	start_color();
	init_pair(1, COLOR_BLUE, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLUE);
	
	noecho();
	refresh();
	
	search_window = newwin(1, COLS, 0, 0);
	address_window = newwin(LINES, COLS / 3, 1, 0);
	city_window = newwin(LINES, COLS / 3 + 1, 1, COLS / 3);
	state_window = newwin(LINES, COLS / 3 + 1, 1, COLS / 3 * 2 + 1);
	
	wbkgd(search_window, COLOR_PAIR(2));
	wbkgd(address_window, COLOR_PAIR(1));
	wbkgd(city_window, COLOR_PAIR(1));
	wbkgd(state_window, COLOR_PAIR(1));
	
	wrefresh(search_window);
	wrefresh(address_window);
	wrefresh(city_window);
	wrefresh(state_window);
}
