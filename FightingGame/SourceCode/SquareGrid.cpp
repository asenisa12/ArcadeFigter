#include "SquareGrid.h"

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
	return std::get<0>(edges[id]);
}

void grid::SquareGrid::add_neighbors(int row, int col)
{
	std::vector<Location*> neigh;
	if (col < GRID_SIZE_X-1)
	{
		neigh.push_back(&squares[row][col + 1]);
		if (row < GRID_SIZE_Y - 1)
		{
			neigh.push_back(&squares[row + 1][col + 1]);
		}
		if (row > 0)
		{
			neigh.push_back(&squares[row - 1][col + 1]);
		}
	}
	if (col > 0)
	{
		neigh.push_back(&squares[row][col - 1]);
		if (row < GRID_SIZE_Y - 1)
		{
			neigh.push_back(&squares[row + 1][col - 1]);
		}
		if (row > 0)
		{
			neigh.push_back(&squares[row - 1][col - 1]);
		}
	}
	if (row < GRID_SIZE_Y - 1)
	{
		neigh.push_back(&squares[row + 1][col]);
	}
	if (row > 0)
	{
		neigh.push_back(&squares[row - 1][col]);
	}
	edges.emplace(squares[row][col], std::make_tuple(neigh,false));

	return;
}

void grid::SquareGrid::create_grid()
{
	int x = 0;
	int y = hight * 0.73;
	int rowCount = 0;
	squareSize = length / 32;

	for (int i = 0;; i++)
	{
		if (i == GRID_SIZE_X)
		{
			x = 0;
			i = 0;
			y += squareSize;
			rowCount++;
		}
		if (rowCount == GRID_SIZE_Y)
			break;

		squares[rowCount][i] = { x, y };
		add_neighbors(rowCount, i);
		x += squareSize;
	}
	return;
}
