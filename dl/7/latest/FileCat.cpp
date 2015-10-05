/**********************
Project: FileCat
Author: Keuin
E-mail: keuinx@gmail.com
BuildTime: 2015-10-02 12:21:05
Version: 1.1
**********************/

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <cstdio>
#include <conio.h>
using namespace std;

COORD GetCurPos();
auto outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
bool MarkBook(string path, vector<string>::size_type pos);
void Notice(string _n);
vector<string>::size_type GetMark(string path);
int main(int argc ,char *argv[])
{
	SetConsoleTitle(_T("FileCat"));
	fstream f;
	string in;
	if (argc > 1) {
		in = argv[1];
	}
	else {
		cout << "Input file path:" << flush;
		while (!(cin >> in)) {}
	}
	cout << endl
	 	 << "Initiating resource from:"
	   	 << in;
	f.open(in);
	vector<string> l;
	string ln;
	while (getline(f, ln)) {
		l.push_back(ln);
	}
	cout << endl
	   	 << "Done!"
	 	 << endl;

	for (auto p = l.cbegin();p != l.cend();++p) {
		cout << *p
			 << endl;
	re:
		auto c = _getch();
		//cout << (int)c << flush;
		switch (c) {
		case '9':
			goto end;
			break;
		case '7':
			MarkBook(in,p - l.cbegin());
			Notice("Book marked.");
			//::MessageBoxA(0,"Bookmark saved.","Notice",MB_OK+MB_ICONINFORMATION);
			goto re;
			break;
		case '3':
			p = l.cbegin() + GetMark(in);
			if (p != l.cbegin())
				--p;
			cout << *p
				 << endl;
			Notice("Bookmark loaded.");
			break;
		}
	}
end:
	CloseHandle(outputHandle);
	f.close();
    return 0;
}

COORD GetCurPos() {
	CONSOLE_SCREEN_BUFFER_INFO l;
	GetConsoleScreenBufferInfo(outputHandle, &l);
	return l.dwCursorPosition;
}

bool MarkBook(string path, vector<string>::size_type pos) { //±Íº« È«©
	ofstream r(path+".bm");
	if (!r.good())
		return false;
	r << pos;
	r.close();
	return true;
}

vector<string>::size_type GetMark(string path) {
	vector<string>::size_type x;
	ifstream r(path + ".bm");
	r >> x;
	r.close();
	return x;
}

void Notice(string _n) {
	auto oldPos = GetCurPos();
	cout << _n << flush;
	SetConsoleCursorPosition(outputHandle, oldPos);
}