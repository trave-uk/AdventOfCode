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
using int64 = long long;

class coord : public std::pair<int64, int64>
{
public:
	coord(int64 x = 0, int64 y = 0)
	{
		first = x;
		second = y;
	}
	// default (only) heuristic: Manhattan distance
	static int64 heuristic(coord& start, coord& goal)
	{
		return (abs(start.first - goal.first) + abs(start.second - goal.second));
	}
	void operator+=(const coord& rhs)
	{
		first += rhs.first;
		second += rhs.second;
	}
	void operator-=(const coord& rhs)
	{
		first -= rhs.first;
		second -= rhs.second;
	}
	void operator/=(int64 rhs)
	{
		first /= rhs;
		second /= rhs;
	}
	int64 length()
	{
		return max(abs(first), abs(second));
	}
};

// class tNode must have an "isBlocked" function thus:
//  bool tNode::isBlocked();
template<class tNode> int64 aStarSearch(coord& start, coord& goal, std::map<coord, tNode>& space, std::vector<coord>* pPath = nullptr)
{
	std::set<coord> openSet;
	openSet.insert(start);
	std::map<coord, int64> gScore;
	gScore[start] = 0;
	std::map<coord, int64> fScore;
	fScore[start] = coord::heuristic(start, goal);

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<coord, coord> cameFrom;

	while (!openSet.empty())
	{
		int64 minF = INT_MAX;
		coord current;
		for (const coord& c : openSet)
		{
			if (fScore.count(c) && fScore[c] < minF)
			{
				minF = fScore[c];
				current = c;
			}
		}

		if (current == goal)
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
			return fScore[goal];
		}

		openSet.erase(current);
		for (int dir = 0; dir < 4; ++dir)
		{
			coord neighbour = current;
			neighbour.first += (dir == 0) - (dir == 1);
			neighbour.second += (dir == 2) - (dir == 3);

			if (!space.count(neighbour) || space[neighbour].isBlocked())
				continue;

			// tentative_gScore is the distance from start to the neighbour through current
			int64 tentative_gScore = gScore[current] + 1;
			int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
			if (tentative_gScore < neighbour_gScore)
			{
				// This path to neighbour is better than any previous one. Record it!
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentative_gScore;
				fScore[neighbour] = tentative_gScore + coord::heuristic(neighbour, goal);
				openSet.insert(neighbour);
			}
		}
	}
	return -1; // failed to find a path
}