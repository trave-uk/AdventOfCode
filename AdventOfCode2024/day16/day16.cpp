// day16.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 16/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

const std::array<coord, 4> directionMap = {
	coord(0, -1),
	coord(1, 0),
	coord(0, 1),
	coord(-1, 0)
};

class Coord
{
public:
	Coord(int64 x=0, int64 y=0, Direction facing=EAST) : pos(x, y), facing(facing) {}
	bool operator<(const Coord& rhs) const
	{
		return pos < rhs.pos || (facing < rhs.facing && pos == rhs.pos);
	}
	bool operator==(const Coord& rhs) const
	{
		return facing == rhs.facing && pos == rhs.pos;
	}
	bool operator!=(const Coord& rhs) const
	{
		return facing != rhs.facing || pos != rhs.pos;
	}
	void turnLeft()
	{
		facing = static_cast<Direction>((static_cast<int>(facing) + 3) % 4);
	}
	void turnRight()
	{
		facing = static_cast<Direction>((static_cast<int>(facing) + 1) % 4);
	}
	void forward()
	{
		const coord& dir = directionMap[facing];
		pos += dir;
	}
	static int64 heuristic(Coord& start, coord& goal)
	{
		if (start.pos == goal)
			return 0;
		int64 distance = coord::heuristic(start.pos, goal);
		const coord& facingDir = directionMap[start.facing];
		coord direction = goal;
		direction -= start.pos;
		direction.first = sgn(direction.first);
		direction.second = sgn(direction.second);
		// facing directly towards goal along an axis: no extra cost
		if (direction == facingDir)
			return distance;
		int64 dot = facingDir.first * direction.first + facingDir.second * direction.second;
		if (dot >= 0)
			// facing towards goal (indirectly): one turn (+1000)
			// facing perpendicular to goal: one turn
			return distance + 1000;
		else
			// facing away from goal: two turns (+2000)
			return distance + 2000;
	}
	coord pos;
	Direction facing;
};

class Node : public INode
{
public:
	Node(char c = '.') : blocked(c == '#') {}
	virtual bool isBlocked() const override
	{
		return blocked;
	}
	bool blocked = false;
	coord dir;
};

using Map = std::map<Coord, Node>;

// Define ASTARDEBUG to output the entire grid on each iteration. Useful for spotting bad data. Only recommended for the example data.
//#define ASTARDEBUG

#ifdef ASTARDEBUG

template<class tNode> void print(Coord& start, coord& goal, std::map<Coord, tNode>& space, coord& max, std::set<Coord>& openSet, std::set<coord>* path = nullptr)
{
	// TODO: print the entire available space, highlighting the start, goal and contents of openSet.
	Coord pos;
	gotoxy(0, 0);
	printf("From (%lld,%lld) to (%lld,%lld)              \n", start.pos.first, start.pos.second, goal.first, goal.second);
	const char *dirs = "^>v<";
	for (pos.pos.second = 0; pos.pos.second <= max.second; ++pos.pos.second)
	{
		std::string line;
		for (pos.pos.first = 0; pos.pos.first <= max.first; ++pos.pos.first)
		{
			for (pos.facing = NORTH; static_cast<int>(pos.facing) < 4; pos.facing = static_cast<Direction>(static_cast<int>(pos.facing) + 1))
			{
				if (pos == start)
					line.append(1, 'S');
				else if (pos.pos == goal)
					line.append(1, 'E');
				else if (path && path->find(pos.pos) != path->end())
					line.append(1, 'O');
				else if (openSet.find(pos) != openSet.end())
					line.append(1, dirs[static_cast<int>(pos.facing)]);
				else if (!space.count(pos))
					line.append(1, ' ');
				else if (space[pos].isBlocked())
					line.append(1, '#');
				else if (space[pos].getCost() == 1)
					line.append(1, '.');
				else
					line.append(1, space[pos].getCost() + '0');
			}
		}
		printf("%s\n", line.c_str());
	}
	_putch('\n');
}
#endif

// A copy of the aStarSearch function from astar.h which has been modified to use a more complex key in the map, and to handle direction specifically for this puzzle. 
template<class tNode> int64 aStarSearchDay16(Coord& start, coord& goal, std::map<Coord, tNode>& space, std::set<coord>* pPath = nullptr)
{
#ifdef ASTARDEBUG
	// Find the maximum extents of space
	coord max;
	for (auto& node : space)
	{
		if (node.first.pos.first > max.first)
			max.first = node.first.pos.first;
		if (node.first.pos.second > max.second)
			max.second = node.first.pos.second;
	}
#endif
	std::set<Coord> openSet;
	openSet.insert(start);
	std::map<Coord, int64> gScore;
	gScore[start] = 0;
	std::map<Coord, int64> fScore;
	fScore[start] = Coord::heuristic(start, goal);

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<Coord, std::vector<Coord>> cameFrom;

	while (!openSet.empty())
	{
#ifdef ASTARDEBUG
		print(start, goal, space, max, openSet);
#endif
		int64 minF = INT_MAX;
		Coord current;
		for (const Coord& c : openSet)
		{
			if (fScore.count(c) && fScore[c] < minF)
			{
				minF = fScore[c];
				current = c;
			}
			else if (fScore.count(c) && fScore[c] == minF)
			{
				// another candidate, prefer non-goal candidates
				if (current.pos == goal)
					current = c;
			}
		}

		if (current.pos == goal)
		{
			if (pPath)
			{
				pPath->clear();
				std::stack<Coord> stack;
				stack.push(current);
				while (!stack.empty())
				{
					Coord next = stack.top();
					stack.pop();
					pPath->insert(next.pos);
					for (Coord parent : cameFrom[next])
					{
						stack.push(parent);
					}
				}
#ifdef ASTARDEBUG
				print(start, goal, space, max, openSet, pPath);
#endif
			}
			return fScore[current];
		}

		openSet.erase(current);
		for (int i = 0; i < 3; ++i)
		{
			Coord neighbour = current;
			Coord check = current;
			int64 cost = 1000;
			switch (i)
			{
			case 0: neighbour.forward(); check = neighbour; cost = 1; break;
			case 1: neighbour.turnLeft(); check = neighbour; check.forward(); break;
			case 2: neighbour.turnRight(); check = neighbour; check.forward(); break;
			}

			if (!space.count(check) || space[check].isBlocked())
				continue;

			// tentative_gScore is the distance from start to the neighbour through current
			int64 tentative_gScore = gScore[current] + cost;
			int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
			if (tentative_gScore < neighbour_gScore)
			{
				// This path to neighbour is better than any previous one. Record it!
				cameFrom[neighbour].clear();
				cameFrom[neighbour].push_back(current);
				gScore[neighbour] = tentative_gScore;
				fScore[neighbour] = tentative_gScore + Coord::heuristic(neighbour, goal);
				openSet.insert(neighbour);
			}
			else if (tentative_gScore == neighbour_gScore)
			{
				// This path to neighbour is equivalent to the best already found. Store an additional backlink
				cameFrom[neighbour].push_back(current);
			}
		}
	}
	return -1; // failed to find a path
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	int64 y = 0;
	Coord start;
	coord goal;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				int64 x = 0;
				for (char c : line)
				{
					if (c == 'S')
					{
						c = '.';
						start = Coord(x, y, EAST);
					}
					else if (c == 'E')
					{
						c = '.';
						goal = coord(x, y);
					}
					Node n(c);
					for (int i = 0; i < 4; ++i)
					{
						Coord c(x, y, static_cast<Direction>(i));
						map[c] = n;
					}
					++x;
				}
			}
			++y;
		}
	}
	fclose(fp);
	delete[] buffer;

	std::set<coord> path;
	part1 = aStarSearchDay16(start, goal, map, &path);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	part2 = path.size();

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example1.txt", 7036, 45);
	Process("example2.txt", 11048, 64);
	Process("example3.txt", 3022, 23);
	Process("input.txt");

	return 0;
}
