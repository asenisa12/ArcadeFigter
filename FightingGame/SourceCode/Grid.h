#ifndef SQUARE_GRID
#define SQUARE_GRID

#include <tuple>
#include <unordered_map>
#include <vector>

namespace grid
{
	enum { LEFT = 100, RIGHT = 101, UP = 102, DOWN = 103 };
	static const int OCCUPATION_PRIORITY = 5;
	static const int FREE_PRIORITY = 1;
	static const int GRID_SIZE_X = 32;
	static const int GRID_SIZE_Y = 5;

	static const int OCCUPATION_COST = 5;
	static const int FREE_COST = 1;

	struct Location
	{
		int X;
		int Y;
		bool operator==(Location const &b)const
		{
			return b.X == X && Y == b.Y;
		}
		bool operator!=(Location const &b)const
		{
			return b.X != X || Y != b.Y;
		}
		//Location(int X_, int Y_);
		
	};

	struct LocationHash
	{
		typedef std::hash<int> hashForInt;
		size_t operator()(Location const& key)const;

	};

	struct Equal
	{
		bool operator()(const Location &id1, const Location &id2) const;
	};

	class SquareGrid
	{

		enum{COST_ID=1, NEIGHBORS_ID=0};

		int startX, startY;

		int length, hight;
		int squareSize;
		Location GridSquares[GRID_SIZE_Y][GRID_SIZE_X];
		std::unordered_map<Location, std::tuple<std::vector<Location*>,int>, LocationHash, Equal> edges;
		void add_neighbors(int row, int col);
		void create_grid();
	public:
		SquareGrid(int length_, int hight_);
		//~SquareGrid();
		int cost(Location id);
		void change_cost(Location id, int cost);
		const std::vector<Location*> neighbors(Location id);
		int getStartingY();
		int getStartingX();
		Location getLocation(int Row, int Col);
		//Location **getSquares();
	};


}

#endif