// day25.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 25/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Direction
{
	NONE,
	EAST,
	SOUTH
};
struct Cucumber
{
	Cucumber(coord _pos=coord(), Direction _dir=NONE)
		: pos(_pos)
		, dir(_dir)
	{}
	coord pos;
	Direction dir;
};

std::map<coord, Cucumber> map;

coord bottomRight;

bool Step()
{
	bool bAnyMoved = false;

	// Move east-facing cucumbers
	std::vector<Cucumber> movedEast;
	for (auto it : map)
	{
		Direction dir = it.second.dir;
		if (dir != EAST)
			continue;
		coord pos = it.first;
		coord nextPos = pos;
		++nextPos.first;
		if (nextPos.first > bottomRight.first)
			nextPos.first = 0;
		if (map.count(nextPos) == 0)
		{
			movedEast.push_back(it.second);
			bAnyMoved = true;
		}
	}
	for (Cucumber c: movedEast)
	{
		map.erase(c.pos);
		++c.pos.first;
		if (c.pos.first > bottomRight.first)
			c.pos.first = 0;
		map.insert_or_assign(c.pos, c);
	}

	// Move south-facing cucumbers
	std::vector<Cucumber> movedSouth;
	for (auto it : map)
	{
		Direction dir = it.second.dir;
		if (dir != SOUTH)
			continue;
		coord pos = it.first;
		coord nextPos = pos;
		++nextPos.second;
		if (nextPos.second > bottomRight.second)
			nextPos.second = 0;
		if (map.count(nextPos) == 0)
		{
			movedSouth.push_back(it.second);
			bAnyMoved = true;
		}
	}
	for (Cucumber c : movedSouth)
	{
		map.erase(c.pos);
		++c.pos.second;
		if (c.pos.second > bottomRight.second)
			c.pos.second = 0;
		map.insert_or_assign(c.pos, c);
	}

	return bAnyMoved;
}
void Process(const char* filename)
{
	map.clear();
	bottomRight = coord();

	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				for (pos.first = 0; pos.first < strlen(thisLine); ++pos.first)
				{
					char c = thisLine[pos.first];
					if (pos.first > bottomRight.first)
						bottomRight.first = pos.first;
					if (pos.second > bottomRight.second)
						bottomRight.second = pos.second;
					
					if (c == '.')
					{
						continue;
					}
					Direction dir = (c == '>') ? EAST : SOUTH;
					Cucumber cum(pos, dir);
					map.insert_or_assign(pos, cum);
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int step = 0;
	bool bAnyMoved = true;
	while (bAnyMoved)
	{
		++step;
		bAnyMoved = Step();
	}
	printf("%s: Part 1: %d\n", filename, step);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
