// day24.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 24/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Direction
{
	NORTH = '^',
	SOUTH = 'v',
	WEST = '<',
	EAST = '>'
};

struct Blizzard : public coord
{
	Blizzard(coord c, char d)
		: coord(c)
		, dir(Direction(d))
	{
		delta.first = (dir == EAST) - (dir == WEST);
		delta.second = (dir == SOUTH) - (dir == NORTH);
	}

	void step(int64 steps, const coord& topLeft, const coord& bottomRight)
	{
		steps %= size;
		coord d(delta);
		d *= steps;
		first += d.first;
		second += d.second;
		if (dir == WEST && first < 1)
			first += size;
		if (dir == NORTH && second < 1)
			second += size;
		if (dir == EAST && first > size)
			first -= size;
		if (dir == SOUTH && second > size)
			second -= size;
	}

	Direction dir;
	coord delta;
	int64 size;
};

struct Space;

struct Node : public INode
{
	Node() = default;
	Node(coord p, Space& s);
	virtual bool isBlocked(coord& from) const override;

	coord pos;
	Space* space = nullptr;
};

struct Space : public std::map<coord, Node>
{
	Space() = default;

	Space(const Space& other)
	{
		Space& This = *this;
		This = other;
		for (auto& it : *this)
		{
			it.second.space = this;
		}
	}

	std::vector<Blizzard> blizzards;

	int64 step(int64 steps)
	{
		for (Blizzard& b : blizzards)
		{
			b.step(steps, topLeft, bottomRight);
		}
		return steps;
	}

	bool canWait(coord c) { return !isBlocked(c); }

	bool isBlocked(coord c) const
	{
		for (const auto& b : blizzards)
		{
			if (coord(b) == c)
			{
				return true;
			}
		}
		return false;
	}

	void finalize()
	{
		int64 width = bottomRight.first - topLeft.first + 1;
		int64 height = bottomRight.second - topLeft.second - 1;
		for (Blizzard& b : blizzards)
		{
			b.size = (b.dir == NORTH || b.dir == SOUTH) ? height : width;
		}
	}

	coord topLeft = { INT_MAX, INT_MAX };
	coord bottomRight = { 0, 0 };
};

Node::Node(coord p, Space& s)
	: pos(p)
	, space(&s)
{
}

bool Node::isBlocked(coord& from) const
{
	return space && space->isBlocked(pos);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Space space;
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");

	coord pos;
	pos.second = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int64 len = strlen(thisLine);
				for (pos.first = 0; pos.first < len; ++pos.first)
				{
					if (thisLine[pos.first] != '#')
					{
						space.topLeft.first = min(pos.first, space.topLeft.first);
						space.topLeft.second = min(pos.second, space.topLeft.second);
						space.bottomRight.first = max(pos.first, space.bottomRight.first);
						space.bottomRight.second = max(pos.second, space.bottomRight.second);

						Node n(pos, space);
						space.insert(std::make_pair(pos, n));
						if (thisLine[pos.first] != '.')
						{
							Blizzard b(pos, thisLine[pos.first]);
							space.blizzards.push_back(b);
						}
					}
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	space.finalize();

	part1 = aStarSearch<Node>(space.topLeft, space.bottomRight, space);
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	// part 2
	space.step(part1);
	int64 back = aStarSearch<Node>(space.bottomRight, space.topLeft, space);
	space.step(back);
	int64 again = aStarSearch<Node>(space.topLeft, space.bottomRight, space);
	part2 = part1 + back + again;
	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 18, 54);
	Process("input.txt");

	return 0;
}
