#include "PathFinding.h"

inline int heuristic(Location a, Location b)
{
	return abs(a.X - b.X) + abs(a.Y - b.Y);
}

void path_search(SquareGrid grid,Location start, Location goal,
	std::unordered_map<Location, Location, LocationHash, Equal> &came_from,
	std::unordered_map<Location, int, LocationHash, Equal> &cost_so_far)
{
	PriorityQueue frontier;
	frontier.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty())
	{
		Location current = frontier.get();
		if (current == goal)
		{
			break;
		}

		for (auto next : grid.neighbors(current))
		{
			int new_cost = cost_so_far[current] + grid.cost(current);
			if (!cost_so_far.count(*next) || new_cost < cost_so_far[*next])
			{
				cost_so_far[*next] = new_cost;
				int priority = new_cost + heuristic(*next, goal);
				frontier.put(*next, priority);
				came_from[*next] = current;
			}
		}
	}
}

std::vector<Location> reconstruct_path(Location start, Location goal,
	std::unordered_map<Location, Location, LocationHash, Equal>& came_from)
{
	std::vector<Location> path;
	Location current = goal;
	path.push_back(current);
	do
	{
		current = came_from[current];
		path.push_back(current);
	} while (current != start);
	return path;
}