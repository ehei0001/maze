#include "StdAfx.h"
#include "Maze.h"

#include <algorithm>
#include <list>
#include <random>


Maze::Maze(const SIZE& size) :
	_size{ size }
{
	srand(
		GetTickCount());
}

void Maze::Build()
{
	for(auto row = 0; row < _size.cy; ++row)
	{
		for(auto column = 0; column < _size.cx; ++column)
		{
			_unvisitedPointContainer.insert(
				{ column, row });
		}
	}

	PointContainer::iterator pointIterator = _unvisitedPointContainer.begin();
	std::advance(
		pointIterator,
		rand() % _unvisitedPointContainer.size());

	auto point = *pointIterator;

	// 임의의 장소부터 시작해서 통로를 만든다. 통로는 양 옆에 벽이 있다. 방문할 곳이 없을 때까지 통로를 만든다
	while(! _unvisitedPointContainer.empty())
	{
		_unvisitedPointContainer.erase(
			point);

		const auto direction = GetDirection(
			point);

		// 더 방문할 지점이 없다
		if(Cell::Side::SideNone == direction)
		{
			// 임의로 방문한 지점을 찾는다. 벽에 구멍을 내서 거기서부터 통로를 만들도록 한다
			for(auto row = 0; row < _size.cy; ++row)
			{
				for(auto column = 0; column < _size.cx; ++column)
				{
					const Point checkedPoint = {
						column,
						row 
					};

					if(false == IsVisited(checkedPoint))
					{
						continue;
					}
					// 막다른 곳
					else if(Cell::Side::SideNone == GetDirection(checkedPoint))
					{
						continue;
					}
					 
					RemoveWallOfNeighbor(
						checkedPoint);

					point = checkedPoint;
					row = _size.cy;
					break;
				}
			}
			
			continue;
		}

		RemoveWall(
			point,
			direction);
		point = GetNextPoint(
			point,
			direction);

		//this->Put();
	}

	//Put();
	//system("pause");

	// remove dead-ends
	for(int row = 0; _size.cy > row; ++row)
	{
		for(int column = 0; _size.cx > column; ++column)
		{
			Sparsify(
				{ column, row } );
		}
	}

	//Put();
	//system("pause");
	LocateRoom();
	//Put();
	//system("pause");
	HoleDoor();
	//Put();
}

void Maze::HoleDoor()
{
	for ( const auto& roomData : _roomDataContainer )
	{
		const Point& roomPoint = roomData.first;
		const SIZE& roomSize = roomData.second;
		auto isFoundUpper = false;
		auto isFoundLower = false;
		auto isFoundLeft = false;
		auto isFoundRight = false;
		
		for(int column = roomPoint.first + 1; roomPoint.first + roomSize.cx - 1 > column; ++column)
		{
			// 상측면 조사
			if(0 < roomPoint.second &&
				false == isFoundUpper)
			{
				Cell& cell = GetCell(
					Point(column, roomPoint.second - 1));
			
				if(true == cell.IsBlock())
				{
					continue;
				}

				cell.RemoveAll();				

				//Put();
				//system("pause");

				isFoundUpper = true;
			}

			// 하측면 조사
			if(roomPoint.second + roomSize.cy < _size.cy &&
				false == isFoundLower)
			{
				Cell& cell = GetCell(
					Point(column, roomPoint.second + roomSize.cy));
			
				if(true == cell.IsBlock())
				{
					continue;
				}

				cell.RemoveAll();

				//Put();
				//system("pause");

				isFoundLower = true;
			}
		}
	
		for(int row = roomPoint.second + 1; roomPoint.second + roomSize.cy - 1 > row; ++row)
		{
			// 좌측면 조사
			if(0 < roomPoint.first &&
				false == isFoundLeft)
			{
				Cell& cell = GetCell(
					Point(roomPoint.first - 1, row));

				if(true == cell.IsBlock())
				{
					continue;
				}

				cell.RemoveAll();

				Put();
				//system("pause");

				isFoundLeft = true;
			}
			
			// 우측면 조사
			if(roomPoint.first + roomSize.cx < _size.cx &&
				false == isFoundRight)
			{
				Cell& cell = GetCell(
					Point(roomPoint.first + roomSize.cx, row));

				if(true == cell.IsBlock())
				{
					continue;
				}

				cell.RemoveAll();

				Put();
				//system("pause");

				isFoundRight = true;
			}
		}
	}
}

void Maze::LocateRoom()
{
	std::vector< SIZE >  _roomSizeContainer = {
		{2, 2},
		{2, 3},
		{1, 1},
		{1, 2},
		{2, 2},
		{2, 2},
		{1, 1},
		{3, 2},
		{4, 2},
		{2, 2},
		{2, 2},
	};
	auto range = std::default_random_engine{};
	std::shuffle(_roomSizeContainer.begin(), _roomSizeContainer.end(), range);

	for(int row = 0; _size.cy > row; ++row)
	{
		for(int column = 0; _size.cx > column; ++column)
		{
			if(true == _roomSizeContainer.empty())
			{
				return;
			}

			const Point roomPoint{
				column,
				row };
			const auto roomSize = _roomSizeContainer.back();

			if(false == IsAdjustableRoom( roomPoint, roomSize ))
			{
				continue;
			}

			LocateRoom(
				roomPoint,
				roomSize);
				
			_roomSizeContainer.pop_back();
			_roomDataContainer[roomPoint] = roomSize;

			//Put();
			//system("pause");
		}
	}
}

bool Maze::FindNearSpace(const Point& point, const SIZE& size) const
{
	// 상측면 조사
	for(int column = point.first; point.first + size.cx > column; ++column)
	{
		if(0 < point.second)
		{
			const Cell& cell = GetCell(
				Point(column, point.second - 1));
			
			if(false == cell.IsBlock())
			{
				return true;
			}
		}

		if(point.second + size.cy < _size.cy)
		{
			const Cell& cell = GetCell(
				Point(column, point.second + size.cy));
			
			if(false == cell.IsBlock())
			{
				return true;
			}
		}
	}
	
	for(int row = point.second; point.second + size.cy > row; ++row)
	{
		// 좌측면 조사
		if(0 < point.first)
		{
			const Cell& cell = GetCell(
				Point(point.first - 1, row));

			if(false == cell.IsBlock())
			{
				return true;
			}
		}
			
		// 우측면 조사
		if(point.first + size.cx < _size.cx)
		{
			const Cell& cell = GetCell(
				Point(point.first + size.cx, row));

			if(false == cell.IsBlock())
			{
				return true;
			}
		}
	}

	return false;
}

void Maze::LocateRoom(const Point& point, const SIZE& size)
{
	for(int row = point.second; point.second + size.cy > row; ++row)
	{
		for(int column = point.first; point.first + size.cx > column; ++column)
		{
			Cell& cell = GetCell(
				Point(column, row));

			if(false == cell.IsBlock())
			{
				return;
			}
			else if(false == FindNearSpace(Point(column, row), size))
			{
				continue;
			}

			cell.RemoveAll();

			if(row == point.second)
			{
				cell.Add(
					Cell::Side::SideNorth);
			}

			if(row == point.second + size.cy - 1)
			{
				cell.Add(
					Cell::Side::SideSouth);
			}

			if(column == point.first)
			{
				cell.Add(
					Cell::Side::SideWest);
			}

			if(column == point.first + size.cx - 1)
			{
				cell.Add(
					Cell::Side::SideEast);
			}
		}
	}
}

bool Maze::IsAdjustableRoom(const Point& point, const SIZE& size) const
{
	if(point.second + size.cy > _size.cy)
	{
		return false;
	}
	else if(point.first + size.cx > _size.cx)
	{
		return false;
	}

	for(int row = point.second; point.second + size.cy > row; ++row)
	{
		for(int column = point.first; point.first + size.cx > column; ++column)
		{
			const Cell& cell = GetCell(
				Point(column, row));

			if(false == cell.IsBlock())
			{
				return false;
			}
		}
	}

	return true;
}

void Maze::Sparsify(const Point& point)
{
	constexpr float sparseness = 0.3f;
	const int cellSize = _size.cx * _size.cy;
	auto currentPoint = point;	

	while(sparseness > (float(_blockedCellSize) / float(cellSize)))
	{
		const auto nextPoint = GetNextDeadEnd(
			currentPoint);

		if(Point() == nextPoint)
		{
			break;
		}
		
		//Put();

		const auto openedDirection = GetOpenedDirection(
			currentPoint);

		{
			auto& cell = GetCell(
				currentPoint);
			cell.Add(
				openedDirection);
		}
		
		{
			auto& cell = GetCell(
				nextPoint);
			
			switch(openedDirection)
			{
			case Cell::Side::SideWest:
				{
					cell.Add(
						Cell::Side::SideEast);
					break;
				}
			case Cell::Side::SideEast:
				{
					cell.Add(
						Cell::Side::SideWest);
					break;
				}
			case Cell::Side::SideNorth:
				{
					cell.Add(
						Cell::Side::SideSouth);
					break;
				}
			case Cell::Side::SideSouth:
				{
					cell.Add(
						Cell::Side::SideNorth);
					break;
				}
			}
		}

		++_blockedCellSize;
		currentPoint = nextPoint;
	}
}

Cell::Side Maze::GetOpenedDirection(const Point& point) const
{
	const Cell& cell = GetCell(
		point);

	if(false == cell.IsWall(Cell::Side::SideWest))
	{
		return Cell::Side::SideWest;
	}
	else if(false == cell.IsWall(Cell::Side::SideEast))
	{
		return Cell::Side::SideEast;
	}
	else if(false == cell.IsWall(Cell::Side::SideNorth))
	{
		return Cell::Side::SideNorth;
	}	
	else if(false == cell.IsWall(Cell::Side::SideSouth))
	{
		return Cell::Side::SideSouth;
	}

	return {};
}

Maze::Point Maze::GetNextDeadEnd(const Point& point) const
{
	const Cell& cell = GetCell(
		point);

	if(false == cell.IsDeadEnd())
	{
		return Point();
	}
	else if(true == cell.IsBlock())
	{
		return Point();
	}
	else if(false == cell.IsWall(Cell::Side::SideWest))
	{
		if(0 < point.first)
		{
			return Point(point.first - 1, point.second);
		}
	}
	else if(false == cell.IsWall(Cell::Side::SideEast))
	{
		if(_size.cx - 1 > point.first)
		{
			return Point(point.first + 1, point.second);
		}
	}	
	else if(false == cell.IsWall(Cell::Side::SideSouth))
	{
		if(_size.cy - 1 > point.second)
		{
			return Point(point.first, point.second + 1);
		}
	}
	else if(false == cell.IsWall(Cell::Side::SideNorth))
	{
		if(0 < point.second)
		{
			return Point(point.first, point.second - 1);
		}
	}

	return Point();
}

void Maze::RemoveWallOfNeighbor(const Point& point)
{
	const Point pointArray[] = {
		Point(point.first - 1, point.second),
		Point(point.first + 1, point.second),
		Point(point.first, point.second	- 1),
		Point(point.first, point.second + 1),
	};

	for(size_t i = 0; i < _countof(pointArray); ++i)
	{
		const Point& checkedPoint = pointArray[i];

		if(0 > checkedPoint.first ||
			0 > checkedPoint.second)
		{
			continue;
		}
		else if(_size.cx <= checkedPoint.first ||
			_size.cy <= checkedPoint.second)
		{
			continue;
		}
		else if(false == IsVisited(checkedPoint))
		{
			continue;
		}

		Cell::Side direction = Cell::Side::SideNone;

		if(checkedPoint.first == point.first &&
			checkedPoint.second < point.second)
		{
			direction = Cell::Side::SideSouth;
		}
		else if(checkedPoint.first == point.first &&
			checkedPoint.second > point.second)
		{
			direction = Cell::Side::SideNorth;
		}
		else if(checkedPoint.first < point.first &&
			checkedPoint.second == point.second)
		{
			direction = Cell::Side::SideEast;
		}
		else if(checkedPoint.first > point.first &&
			checkedPoint.second == point.second)
		{
			direction = Cell::Side::SideWest;
		}

		RemoveWall(
			checkedPoint,
			direction);
		break;
	}
}

Maze::Point Maze::GetNextPoint(const Point& point, Cell::Side direction) const
{
	switch(direction)
	{
	case Cell::Side::SideSouth:
		{
			if((_size.cy - 1) == point.second)
			{
				break;
			}

			return Point(point.first, point.second + 1);
		}
	case Cell::Side::SideNorth:
		{
			if(0 == point.second)
			{
				break;
			}

			return Point(point.first, point.second - 1);
		}
	case Cell::Side::SideWest:
		{
			if(0 == point.first)
			{
				break;
			}

			return Point(point.first - 1, point.second);
		}
	case Cell::Side::SideEast:
		{
			if((_size.cx - 1) == point.first)
			{
				break;
			}

			return Point(point.first + 1, point.second);
		}
	}

	return Point(0, 0);
}

void Maze::RemoveWall(const Point& point, Cell::Side direction)
{
	Cell& cell = GetCell(
		point);

	switch(direction)
	{
	case Cell::Side::SideSouth:
		{
			if((_size.cy - 1) > point.second)
			{
				cell.Remove(
					Cell::Side::SideSouth);

				Cell& neighborCell = GetCell(
					Point(point.first, point.second + 1));
				neighborCell.Remove(
					Cell::Side::SideNorth);
			}

			break;
		}
	case Cell::Side::SideNorth:
		{
			if(0 < point.second)
			{
				cell.Remove(
					Cell::Side::SideNorth);

				Cell& neighborCell = GetCell(
					Point(point.first, point.second - 1));
				neighborCell.Remove(
					Cell::Side::SideSouth);
			}

			break;
		}
	case Cell::Side::SideWest:
		{
			if(0 < point.first)
			{
				cell.Remove(
					Cell::Side::SideWest);

				Cell& neighborCell = GetCell(
					Point(point.first - 1, point.second));
				neighborCell.Remove(
					Cell::Side::SideEast);
			}

			break;
		}
	case Cell::Side::SideEast:
		{
			if((_size.cx - 1) > point.first)
			{
				cell.Remove(
					Cell::Side::SideEast);

				Cell& neighborCell = GetCell(
					Point(point.first + 1, point.second));
				neighborCell.Remove(
					Cell::Side::SideWest);
			}

			break;
		}
	}

	//Put();
}

Cell::Side Maze::GetDirection(const Point& point)
{
	using DirectionContainer = std::set< Cell::Side >;
	DirectionContainer directionContainer = { Cell::Side::SideSouth, Cell::Side::SideNorth, Cell::Side::SideWest, Cell::Side::SideEast };

	// 임의의 방향을 얻고 그 지점으로 조사한다. 유효한 범위이거나 미방문 지점이면 그 방향을 돌려준다
	while(false == directionContainer.empty())
	{
		DirectionContainer::const_iterator iterator = directionContainer.begin();
		std::advance(
			iterator,
			rand() % directionContainer.size());

		const Cell::Side direction = *iterator;
		directionContainer.erase(
			iterator);

		switch(direction)
		{
		case Cell::Side::SideSouth:
			{
				if((_size.cy - 1) == point.second)
				{
					continue;
				}
				else if(IsVisited(Point(point.first, point.second + 1)))
				{
					continue;
				}

				return direction;
			}
		case Cell::Side::SideNorth:
			{
				if(0 == point.second)
				{
					continue;
				}
				else if(IsVisited(Point(point.first, point.second - 1)))
				{
					continue;
				}

				return direction;
			}
		case Cell::Side::SideWest:
			{
				if(0 == point.first)
				{
					continue;
				}
				else if(IsVisited(Point(point.first - 1, point.second)))
				{
					continue;
				}

				return direction;
			}
		case Cell::Side::SideEast:
			{
				if((_size.cx - 1) == point.first)
				{
					continue;
				}
				else if(IsVisited(Point(point.first + 1, point.second)))
				{
					continue;
				}

				return direction;
			}
		}
	}

	return {};
}

const Cell& Maze::GetCell(const Point& point) const
{
	const CellContainer::const_iterator iterator = _cellContainer.find(
		point);

	if(_cellContainer.end() == iterator)
	{
		static const Cell emptyCell;
		
		return emptyCell;
	}

	return iterator->second;
}

Cell& Maze::GetCell(const Point& point)
{
	return _cellContainer[point];
}

const Cell* Maze::GetCell(int x, int y) const
{
	const CellContainer::const_iterator iterator = _cellContainer.find(
		Point(x, y));

	if(_cellContainer.end() == iterator)
	{
		return 0;
	}

	return &(iterator->second);
}

void Maze::Put() const
{
	constexpr int tileSize = 3;
	using TextContainer = std::map< Point, std::string >;
	TextContainer textContainer;

	for(int row = 0; row < _size.cy; ++row)
	{
		for(int column = 0; column < _size.cx; ++column)
		{
			const Cell* const cell = GetCell(
				column, row);

			if(nullptr == cell)
			{
				continue;
			}

			// 전부 막힘
			if(cell->IsBlock())
			{
				for(int i = 0; i < tileSize; ++i)
				{
					for(int j = 0; j < tileSize; ++j)
					{
						textContainer[Point(column * tileSize + i, row * tileSize + j)] = "■";
					}
				}

				continue;
			}

			if(cell->IsWall(Cell::Side::SideEast))
			{
				for(int i = 0; i < tileSize; ++i)
				{
					textContainer[Point(column * tileSize + tileSize - 1, row * tileSize + i)] = "■";
				}
			}

			if(cell->IsWall(Cell::Side::SideWest))
			{
				for(int i = 0; i < tileSize; ++i)
				{
					textContainer[Point(column * tileSize, row * tileSize + i)] = "■";
				}
			}

			if(cell->IsWall(Cell::Side::SideNorth))
			{
				for(int i = 0; i < tileSize; ++i)
				{
					textContainer[Point(column * tileSize + i, row * tileSize)] = "■";
				}
			}

			if(cell->IsWall(Cell::Side::SideSouth))
			{
				for(int i = 0; i < tileSize; ++i)
				{
					textContainer[Point(column * tileSize + i, row * tileSize + tileSize - 1)] = "■";
				}
			}
		}
	}

	// http://www.gamedev.net/community/forums/topic.asp?topic_id=308393
	HANDLE screen_buffer_handle = GetStdHandle(
		STD_OUTPUT_HANDLE);
	COORD coord = {0};
	SetConsoleCursorPosition(
		screen_buffer_handle,
		coord);
	setForeColor(
		CYAN);
	setBackColor(
		BWHITE);

	for(int row = 0; row < _size.cy * tileSize; ++row)
	{
		for(int column = 0; column < _size.cx * tileSize; ++column)
		{
			const TextContainer::const_iterator iterator = textContainer.find(
				Point(column, row));

			if(textContainer.end() == iterator)
			{
				std::cout << "  ";
			}
			else
			{
				std::cout << iterator->second.c_str();
			}
		}

		std::cout << "\n";
	}

	std::cout << "\n";

	setForeColor(
		WHITE);
	setBackColor(
		0);
}