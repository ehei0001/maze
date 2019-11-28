// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <map>
#include <set>
#include <iostream>
#include <list>

// TODO: reference additional headers your program requires here
#define GREEN	2
#define CYAN	3
#define RED	4
#define PURPLE	5
#define BROWN	6
#define WHITE	7
#define GRAY	8
#define LBLUE	9
#define LGREEN	10
#define LCYAN	11
#define LRED	12
#define LPURPLE	13
#define YELLOW	14
#define BWHITE	15

void setForeColor(int color);
void setBackColor(int color);