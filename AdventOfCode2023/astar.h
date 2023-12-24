/* Algorithm from https://en.wikipedia.org/wiki/A*_search_algorithm

function reconstruct_path(cameFrom, current)
	total_path := {current}
	while current in cameFrom.Keys:
		current := cameFrom[current]
		total_path.prepend(current)
	return total_path

// A* finds a path from start to goal.
// h is the heuristic function. h(n) estimates the cost to reach goal from node n.
function A_Star(start, goal, h)
	// The set of discovered nodes that may need to be (re-)expanded.
	// Initially, only the start node is known.
	// This is usually implemented as a min-heap or priority queue rather than a hash-set.
	openSet : = { start }

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start
	// to n currently known.
	cameFrom: = an empty map

	// For node n, gScore[n] is the cost of the cheapest path from start to n currently known.
	gScore : = map with default value of Infinity
	gScore[start] : = 0

	// For node n, fScore[n] := gScore[n] + h(n). fScore[n] represents our current best guess as to
	// how short a path from start to finish can be if it goes through n.
	fScore : = map with default value of Infinity
	fScore[start] : = h(start)

	while openSet is not empty
		// This operation can occur in O(1) time if openSet is a min-heap or a priority queue
		current : = the node in openSet having the lowest fScore[] value
		if current = goal
			return reconstruct_path(cameFrom, current)

		openSet.Remove(current)
		for each neighbor of current
			// d(current,neighbor) is the weight of the edge from current to neighbor
			// tentative_gScore is the distance from start to the neighbor through current
			tentative_gScore : = gScore[current] + d(current, neighbor)
			if tentative_gScore < gScore[neighbor]
				// This path to neighbor is better than any previous one. Record it!
				cameFrom[neighbor] : = current
				gScore[neighbor] : = tentative_gScore
				fScore[neighbor] : = tentative_gScore + h(neighbor)
				if neighbor not in openSet
					openSet.add(neighbor)

		// Open set is empty but goal was never reached
		return failure
*/

// For Advent of Code, we'll (almost?) always be finding routes on grids, with four directions, so that's what I'll support here.
// I'll need a fairly standard way of describing the map. I think I'll make this a templated function so that calling code can provide some functions on the node object.
// The heuristic function will be the Manhattan distance. If anything needs a different heuristic this can be modified to take a callback or call a function on the templated node type.

#include <map>
#include <queue>

#ifndef _ASTAR_H
#define _ASTAR_H

using int64 = __int64;

template<typename tCoord = int64>
class coord_type : public std::pair<tCoord, tCoord>
{
public:
	coord_type<tCoord>(tCoord x = 0, tCoord y = 0)
	{
		this->first = x;
		this->second = y;
	}
	// default (only) heuristic: Manhattan distance
	static tCoord heuristic(coord_type<tCoord>& start, coord_type<tCoord>& goal)
	{
		return (abs(start.first - goal.first) + abs(start.second - goal.second));
	}
	void operator+=(const coord_type<tCoord>& rhs)
	{
		this->first += rhs.first;
		this->second += rhs.second;
	}
	void operator-=(const coord_type<tCoord>& rhs)
	{
		this->first -= rhs.first;
		this->second -= rhs.second;
	}
	void operator/=(tCoord rhs)
	{
		this->first /= rhs;
		this->second /= rhs;
	}
	void operator*=(tCoord rhs)
	{
		this->first *= rhs;
		this->second *= rhs;
	}
	tCoord length() const
	{
		return max(abs(this->first), abs(this->second));
	}
	bool isContained(const coord_type<tCoord>& min, const coord_type<tCoord>& max, tCoord epsilon = 0) const
	{
		return (this->first + epsilon) >= min.first && (this->first - epsilon) <= max.first && (this->second + epsilon) >= min.second && (this->second - epsilon) <= max.second;
	}
	virtual void move(const coord_type<tCoord>& dir)
	{
		*this += dir;
	}
	bool reached(const coord_type<tCoord>& goal) const
	{
		return *this == goal;
	}
};
using coord = coord_type<int64>;

// Define ASTARDEBUG to output the entire grid on each iteration. Useful for spotting bad data.
//#define ASTARDEBUG

#ifdef ASTARDEBUG

static void gotoxy(short x, short y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

template<class tNode> void print(coord& start, coord& goal, std::map<coord, tNode>& space, coord& max, std::set<coord>& openSet)
{
	// TODO: print the entire available space, highlighting the start, goal and contents of openSet.
	coord pos;
	gotoxy(0, 0);
	printf("From (%lld,%lld) to (%lld,%lld)              \n", start.first, start.second, goal.first, goal.second);
	for (pos.second = 0; pos.second <= max.second; ++pos.second)
	{
		for (pos.first = 0; pos.first <= max.first; ++pos.first)
		{
			if (pos == start)
				_putch('o');
			else if (pos == goal)
				_putch('*');
			else if (openSet.find(pos) != openSet.end())
				_putch('O');
			else if (!space.count(pos))
				_putch(' ');
			else if (space[pos].isBlocked())
				_putch('#');
			else if (space[pos].getCost() == 1)
				_putch('.');
			else
				_putch(space[pos].getCost() + '0');
		}
		_putch('\n');
	}
	_putch('\n');
}
#endif

// optional class to derive from for the tNode class, implementing the required interface
struct INode
{
	virtual bool isBlocked() const { return false; }
	virtual int getCost() const { return 1; }
};

// class tNode must have an "isBlocked" function thus:
//  bool tNode::isBlocked();
template<class tCoord, class tNode> int64 aStarSearch(tCoord& start, coord& goal, std::map<tCoord, tNode>& space, std::vector<tCoord>* pPath = nullptr)
{
#ifdef ASTARDEBUG
	// Find the maximum extents of space
	coord max;
	for (auto node : space)
	{
		if (node.first.first > max.first)
			max.first = node.first.first;
		if (node.first.second > max.second)
			max.second = node.first.second;
	}
#endif
	std::set<tCoord> openSet;
	openSet.insert(start);
	std::map<tCoord, int64> gScore;
	gScore[start] = 0;
	std::map<tCoord, int64> fScore;
	fScore[start] = tCoord::heuristic(start, goal);

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<tCoord, tCoord> cameFrom;

	while (!openSet.empty())
	{
#ifdef ASTARDEBUG
		print(start, goal, space, max, openSet);
#endif
		int64 minF = INT_MAX;
		tCoord current;
		for (const tCoord& c : openSet)
		{
			if (fScore.count(c) && fScore[c] < minF)
			{
				minF = fScore[c];
				current = c;
			}
		}

		if (current.reached(goal))
		{
			if (pPath)
			{
				pPath->clear();
				pPath->push_back(current);
				while (current != start)
				{
					current = cameFrom[current];
					pPath->push_back(current);
				}
				std::reverse(pPath->begin(), pPath->end());
			}
			return fScore[current];
		}

		openSet.erase(current);
		for (int dir = 0; dir < 4; ++dir)
		{
			tCoord neighbour = current;
			coord d((dir == 0) - (dir == 1), (dir == 2) - (dir == 3));
			neighbour.move(d);

			if (!space.count(neighbour) || space[neighbour].isBlocked())
				continue;

			// tentative_gScore is the distance from start to the neighbour through current
			int64 tentative_gScore = gScore[current] + space[neighbour].getCost();
			int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
			if (tentative_gScore < neighbour_gScore)
			{
				// This path to neighbour is better than any previous one. Record it!
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentative_gScore;
				fScore[neighbour] = tentative_gScore + tCoord::heuristic(neighbour, goal);
				openSet.insert(neighbour);
			}
		}
	}
	return -1; // failed to find a path
}

#endif // _ASTAR_H
