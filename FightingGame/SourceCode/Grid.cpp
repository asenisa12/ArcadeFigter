#include "Grid.h"


size_t	grid::LocationHash::operator()(const Location &key)const
{
	size_t const hX(hashForInt()(key.X));
	size_t const hY(hashForInt()(key.Y));
	return (hX ^ (hY << 1));
}

bool grid::Equal::operator()(const Location &id1, const Location &id2) const
{
	return id1.X == id2.X && id1.Y == id2.Y;
}

grid::SquareGrid::SquareGrid(int length_, int hight_)
	:length(length_), hight(hight_)
{
	create_grid();
}


const std::vector<grid::Location*> grid::SquareGrid::neighbors(Location id)
{
	return std::get<NEIGHBORS_ID>(edges[id]);
}

int grid::SquareGrid::cost(Location id)
{
	return std::get<COST_ID>(edges[id]);
}

void grid::SquareGrid::change_cost(Location id, int cost)
{
	if (edges.count(id) > 0)
	{
		std::get<COST_ID>(edges[id]) = cost;
	}
}

void grid::SquareGrid::add_neighbors(int row, int col)
{
	std::vector<Location*> neigh;
	if (col < GRID_SIZE_X - 1)
	{
		neigh.push_back(&GridSquares[row][col + 1]);
	}
	if (col > 0)
	{
		neigh.push_back(&GridSquares[row][col - 1]);
	
	}
	if (row < GRID_SIZE_Y - 1)
	{
		neigh.push_back(&GridSquares[row + 1][col]);
	}
	if (row > 0)
	{
		neigh.push_back(&GridSquares[row - 1][col]);
	}
	edges.emplace(GridSquares[row][col], std::make_tuple(neigh, FREE_PRIORITY));

	return;
}

void grid::SquareGrid::create_grid()
{
	//starting Y position is 73% of the hight
	int y = hight * 0.73;
	startY = y;
	int x = 0;
	startX = x;
	int rowCount = 0;
	squareSize = length / GRID_SIZE_X;
	for (int i = 0;i<GRID_SIZE_Y; i++)
	{
		for (int j = 0; j < GRID_SIZE_X; j++)
		{
			GridSquares[i][j] = { x, y };
			add_neighbors(i, j);
			x += squareSize;
		}
		x = 0;
		y += squareSize;
		
	}
	return;
}

int grid::SquareGrid::getStartingY()
{
	return startY;
}

int grid::SquareGrid::getStartingX()
{
	return startX;
}


grid::Location grid::SquareGrid::getLocation(int Row, int Col)
{
	if (Row > GRID_SIZE_Y-1)
	{
		Row = GRID_SIZE_Y - 1;
	}else if (Row < 1)
	{
		Row = 0;
	}
	return GridSquares[Row][Col];
}
