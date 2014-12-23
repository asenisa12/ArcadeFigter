#pragma once

#include "Grid.h"
#include "PriorityQueue.h"
#include <unordered_map>
#include <cmath>
using namespace grid;

inline int heuristic(Location a, Location b);

void path_search(SquareGrid grid, Location start, Location goal,
	std::unordered_map<Location, Location, LocationHash, Equal> &came_from,
	std::unordered_map<Location, int, LocationHash, Equal> &cost_so_far);

std::vector<Location> reconstruct_path(Location start, Location goal,
	std::unordered_map<Location, Location, LocationHash, Equal>& came_from);