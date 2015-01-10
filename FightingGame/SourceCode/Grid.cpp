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
	/*GridSquares = new Location*[GRID_SIZE_Y];
	for (int i = 0; i < GRID_SIZE_Y; i++)
		GridSquares[i] = new Location[GRID_SIZE_X];*/

	create_grid();
}

//grid::SquareGrid::~SquareGrid()
//{
//	for (int i = 0; i<GRID_SIZE_Y; i++)
//		delete[] GridSquares[i];
//	delete[] GridSquares;
//}

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
	if (edges.count(id) > 0){
		printf("Changing cost to: ");
		printf("current square X %d Y %d\n\n", id.X, id.Y);

		std::get<COST_ID>(edges[id]) = cost;
	}
	else
	{
		printf("Error there is not such ID in the map!!!");
		printf("current square X %d Y %d\n\n", id.X, id.Y);
	}
}

void grid::SquareGrid::add_neighbors(int row, int col)
{
	std::vector<Location*> neigh;
	if (col < GRID_SIZE_X - 1)
	{
		neigh.push_back(&GridSquares[row][col + 1]);
		/*if (row < GRID_SIZE_Y - 1)
		{
			neigh.push_back(&squares[row + 1][col + 1]);
		}
		if (row > 0)
		{
			neigh.push_back(&squares[row - 1][col + 1]);
		}*/
	}
	if (col > 0)
	{
		neigh.push_back(&GridSquares[row][col - 1]);
		/*if (row < GRID_SIZE_Y - 1)
		{
			neigh.push_back(&squares[row + 1][col - 1]);
		}
		if (row > 0)
		{
			neigh.push_back(&squares[row - 1][col - 1]);
		}*/
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
			//printf("aaaa--x%d, y%d\n", x, y);
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

//grid::Location **grid::SquareGrid::getSquares()
//{
//	return GridSquares;
//}

grid::Location grid::SquareGrid::getLocation(int Row, int Col)
{
	return GridSquares[Row][Col];
}
