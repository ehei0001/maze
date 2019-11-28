#pragma once



class Cell
{
public:
	enum class Side
	{
		SideNone,
		SideNorth,
		SideSouth,
		SideEast,
		SideWest,
	};

	inline void Add(Side side) { _wallContainer.insert(side); }
	inline void Remove(Side side) { _wallContainer.erase(side); }
	inline void RemoveAll() { _wallContainer.clear(); }
	inline bool IsWall(Side side) const { return _wallContainer.end() != _wallContainer.find(side); }
	inline bool IsDeadEnd() const { return 3 == _wallContainer.size(); }
	inline bool IsBlock() const { return 4 == _wallContainer.size(); }
	inline size_t GetWallCount() const { return _wallContainer.size(); }

private:
	std::set< Side > _wallContainer = { Side::SideNorth, Side::SideSouth, Side::SideEast, Side::SideWest };
};

class Maze
{
public:
	Maze(const SIZE&);
	void Build();
	const SIZE& GetSize() const { return _size; }
	const Cell* GetCell(int x, int y) const;
	void Put() const;

private:
	using Point = std::pair< int, int >;

	inline bool IsVisited(const Point& point) const { return _unvisitedPointContainer.end() == _unvisitedPointContainer.find(point); }

	Cell::Side GetDirection(const Point&);
	Cell& GetCell(const Point&);
	const Cell& GetCell(const Point&) const;
	void RemoveWall(const Point&, Cell::Side);
	Point GetNextPoint(const Point&, Cell::Side) const;
	void RemoveWallOfNeighbor(const Point&);
	void Sparsify(const Point&);
	Point GetNextDeadEnd(const Point&) const;
	Cell::Side GetOpenedDirection(const Point&) const;
	void LocateRoom();
	void LocateRoom(const Point&, const SIZE&);
	bool IsAdjustableRoom(const Point&, const SIZE&) const;
	void HoleDoor();
	bool FindNearSpace(const Point&, const SIZE&) const;

private:
	const SIZE _size;
	int _blockedCellSize = {};
	Cell::Side _previousDirection = {};

	using CellContainer = std::map< Point, Cell >;
	CellContainer _cellContainer;

	using PointContainer = std::set< Point > ;
	PointContainer _unvisitedPointContainer;

	using RoomDataContainer = std::map< Point, SIZE > ;
	RoomDataContainer _roomDataContainer;
};