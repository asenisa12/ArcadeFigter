#pragma once

#include <utility>
#include <queue>
#include <functional>
#include <vector>
#include "Grid.h"

using grid::Location;

typedef std::pair<int, Location> PQElement;

struct comparison
{
	bool operator()(const PQElement &el1, const PQElement &el2)const
	{
		return el1.first > el2.first;
	}
};

struct PriorityQueue
{

	std::priority_queue < PQElement, std::vector<PQElement>, comparison> elements;

	inline void put(grid::Location item, int priority)
	{
		elements.emplace(std::make_pair( priority, item));
	}

	inline grid::Location get()
	{
		Location best_item = elements.top().second;
		elements.pop();
		return best_item;
	}

	inline bool empty()
	{
		return elements.empty();
	}
};