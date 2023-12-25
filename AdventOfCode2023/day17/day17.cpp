// day17.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 17/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node : public INode
{
	Node(char c='0')
		: cost(c-'0')
	{}

	virtual bool isBlocked() const override
	{
		return false;
	}

	virtual int getCost() const override
	{
		return cost;
	}

	int cost;
};

int g_minSpeed = 0;

struct Coord : public coord
{
	Coord()
		: coord()
		, dir(coord())
		, speed(0)
	{}

	Coord(const coord& pos, const coord& dir = coord(0, 0), const int speed = 0)
		: coord(pos)
		, dir(dir)
		, speed(speed)
	{}

	virtual void move(const coord& d) override
	{
		if (d == dir)
		{
			coord::move(d);
			++speed;
		}
		else if (speed < g_minSpeed && speed > 0)
		{
			// cannot change direction going under minspeed
			return;
		}
		else
		{
			coord back(-d.first, -d.second);
			if (dir != back)
			{
				coord::move(d);
				dir = d;
				speed = 1;
			}
		}
	}

	bool operator<(const Coord& other) const
	{
		if (other.first < first)
			return true;
		else if (other.first > first)
			return false;
		else if (other.second < second)
			return true;
		else if (other.second > second)
			return false;
		else if (other.dir < dir)
			return true;
		else if (other.dir > dir)
			return false;
		else if (other.speed < speed)
			return true;
		else 
			return false;
	}

	coord dir; // direction last moved
	int speed; // times moved same direction
};

using Map = std::map<Coord, Node>;

void Process1(const char* filename, int part, int minSpeed, int maxSpeed)
{
	g_minSpeed = minSpeed;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 score = 0;
	coord pos(0, 0);
	Map map;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				pos.first = 0;
				for (char c : line)
				{
					for (int d = 0; d < 4; ++d)
					{
						coord dir((d == 0) - (d == 1), (d == 2) - (d == 3));
						for (int s = 0; s <= maxSpeed; ++s)
						{
							Coord co(pos, dir, s);
							map[co] = Node(c);
						}
					}
					++pos.first;
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	coord size = pos;
	Coord start(coord(0, 0));
	coord goal(size.first - 1, size.second - 1);
	score = aStarSearch(start, goal, map);

//	assert(expectedPart1 == -1 || expectedPart1 == score);
	printf("%s: Part %d: %lld\n", filename, part, score);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Process1(filename, 1, 0, 3);
	Process1(filename, 2, 4, 10);
}

int main()
{
	Process("example.txt", 102, 94);
	Process("input.txt");

	return 0;
}
