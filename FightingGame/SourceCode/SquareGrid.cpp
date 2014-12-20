#include "SquareGrid.h"

size_t	LocationHash::operator()(const Location &key)const
{
	size_t const hX(int_hash()(key.X));
	size_t const hY(int_hash()(key.Y));
	size_t const hV(bool_hash()(key.visited));
	return (hX ^ (hY << 1)) ^ (hV << 2);
}

bool Equal::operator()(const Location &id1, const Location &id2) const
{
	return id1.X == id2.X && id1.Y == id2.Y && id1.visited == id2.visited;
}

SquareGrid::SquareGrid(int length_, int hight_)
	:length(length_), hight(hight_)
{
	create_grid();

}
const std::vector<Location*> SquareGrid::neighbors(Location id)
{
	return edges[id];
}
void SquareGrid::add_neighbors(int row, int col)
{
	std::vector<Location*> neigh;
	if (col < GRID_SIZE_X-1)
	{
		neigh.push_back(&squares[row][col + 1]);
		if (row != 0)
		{
			neigh.push_back(&squares[row + 1][col + 1]);
		}
		if (row < GRID_SIZE_Y - 1)
		{
			neigh.push_back(&squares[row - 1][col + 1]);
		}
	}
	if (col > 0)
	{
		neigh.push_back(&squares[row][col - 1]);
		if (row != 0)
		{
			neigh.push_back(&squares[row + 1][col - 1]);
		}
		if (row != GRID_SIZE_Y-1)
		{
			neigh.push_back(&squares[row - 1][col - 1]);
		}
	}
	if (row > 0)
	{
		neigh.push_back(&squares[row + 1][col]);
	}
	if (row < GRID_SIZE_Y - 1)
	{
		neigh.push_back(&squares[row - 1][col]);
	}
	edges.emplace(squares[row][col], neigh);

	return;
}

void SquareGrid::create_grid()
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

		squares[rowCount][i] = { x, y, false };
		add_neighbors(rowCount, i);
		printf("row: %d, col: %d, x:%d , Y:%d\n", rowCount, i, x, y);
		x += squareSize;
	}
	return;
}

//void SquareGrid::add_to_map()
//{
//	int rowCount = 0;
//	for (int i = 0;rowCount<GRID_SIZE_Y; i++)
//	{
//		if (i == GRID_SIZE_X)
//		{
//			i = 0;
//			rowCount++;
//		}
//		add_neighbors(rowCount, i);
//
//	}
//}