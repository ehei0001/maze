// randomMaze.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Maze.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(3 > argc)
	{
		std::cout << "Usage: randomMaze [width: 2 ~ 12] [height: 2 ~ 25]\n";
		return 0;
	}

	const SIZE mazeSize = {
		_ttoi(argv[1]),
		_ttoi(argv[2])};

	if(2 > mazeSize.cx ||
		12 < mazeSize.cx ||
		2 > mazeSize.cy ||		
		25 < mazeSize.cy)
	{
		std::cout << "Usage: randomMaze [width: 2 ~ 12] [height: 2 ~ 25]\n";
		return 0;
	}

	system(
		"cls");

	Maze maze(
		mazeSize);
	maze.Build();
	maze.Put();
	return 0;
}