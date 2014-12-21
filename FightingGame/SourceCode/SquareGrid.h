#ifndef SQUARE_GRID
#define SQUARE_GRID

#include <algorithm>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <vector>
#include <stdio.h>

struct Location
{
	int X;
	int Y;
};

struct LocationHash
{
	typedef std::hash<int> int_hash;
	typedef std::hash<bool> bool_hash;
	size_t operator()(Location const& key)const;
	
};

struct Equal
{
	bool operator()(const Location &id1, const Location &id2) const;
};

class SquareGrid
{
	static const int GRID_SIZE_X = 32;
	static const int GRID_SIZE_Y = 5;

	int length, hight;
	int squareSize;
	Location squares[GRID_SIZE_Y][GRID_SIZE_X];
	std::unordered_map<Location, std::vector<Location*>, LocationHash, Equal> edges;
	void add_neighbors(int row, int col);
	void create_grid();
public:
	
	SquareGrid(int length_, int hight_);
	const std::vector<Location*> neighbors(Location id);
};

#endif