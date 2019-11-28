// stdafx.cpp : source file that includes just the standard includes
// randomMaze.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

// http://www.gamedev.net/community/forums/topic.asp?topic_id=308393
void setForeColor(int c)
{	
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);	
	WORD wAttrib = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;	
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	wAttrib = csbi.wAttributes;	
	wAttrib &= ~(FOREGROUND_BLUE |
				FOREGROUND_GREEN |
				FOREGROUND_RED |
				FOREGROUND_INTENSITY);
	if (c & 1)		wAttrib |= FOREGROUND_BLUE;
	if (c & 2)		wAttrib |= FOREGROUND_GREEN;
	if (c & 4)		wAttrib |= FOREGROUND_RED;
	if (c & 8)		wAttrib |= FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, wAttrib);
}

// http://www.gamedev.net/community/forums/topic.asp?topic_id=308393
void setBackColor(int c)
{	
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD wAttrib = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole,&csbi);
	wAttrib = csbi.wAttributes;
	wAttrib &= ~(BACKGROUND_BLUE |
				BACKGROUND_GREEN | 
				BACKGROUND_RED |
				BACKGROUND_INTENSITY);
	if (c & 1)		wAttrib |= BACKGROUND_BLUE;
	if (c & 2)		wAttrib |= BACKGROUND_GREEN;
	if (c & 4)		wAttrib |= BACKGROUND_RED;
	if (c & 8)		wAttrib |= BACKGROUND_INTENSITY;
	SetConsoleTextAttribute(hConsole, wAttrib);
}