// day11.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 11/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Galaxy
{
	Galaxy(int64 x, int64 y)
		: pos(x, y)
		, offset(0, 0)
	{}
	coord pos;
	coord offset;

	coord getPosition(int64 expansionMultiplier = 2) const
	{
		coord newpos = offset;
		newpos *= (expansionMultiplier - 1);
		newpos += pos;
		return newpos;
	}
};
bool operator<(const Galaxy& s1, const Galaxy& s2)
{
	return s1.pos < s2.pos;
}

using Map = std::set<Galaxy>;

void PrepareExpansion(Map& map, int64 size)
{
	// find empty rows and columns and work out how much to offset them.
	std::set<int64> rows;
	std::set<int64> cols;
	int64 offset = 0;
	for (int64 y = 0; y < size; ++y)
	{
		int64 galaxies = 0;
		for (int64 x = 0; x < size; ++x)
		{
			Galaxy s(x, y);
			int64 exists = map.count(s);
			galaxies += exists;
			if (exists && offset > 0)
			{
				Map::iterator found = map.find(s);
				Map::iterator hint = found;
				hint++;
				s = *found;
				map.erase(found);
				s.offset.second += offset;
				map.insert(hint, s);
			}
		}
		if (!galaxies)
		{
			rows.insert(y);
			++offset;
		}
	}
	offset = 0;
	for (int64 x = 0; x < size; ++x)
	{
		int64 galaxies = 0;
		for (int64 y = 0; y < size; ++y)
		{
			Galaxy s(x, y);
			int64 exists = map.count(s);
			galaxies += exists;
			if (exists && offset > 0)
			{
				Map::iterator found = map.find(s);
				Map::iterator hint = found;
				hint++;
				s = *found;
				map.erase(found);
				s.offset.first += offset;
				map.insert(hint, s);
			}
		}
		if (!galaxies)
		{
			cols.insert(x);
			++offset;
		}
	}
}

void Process(const char* filename, int64 part2times, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	int64 y = 0;
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
				int64 x = 0;
				for (char c : line)
				{
					if (c == '#')
					{
						Galaxy pos(x, y);
						map.insert(pos);
					}
					++x;
				}
				++y;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	// assume input is square
	int64 size = y;

	PrepareExpansion(map, size);

	for (const Galaxy& s1 : map)
	{
		for (const Galaxy& s2 : map)
		{
			if (s2.pos <= s1.pos)
			{
				continue;
			}
			coord from = s1.getPosition();
			coord to = s2.getPosition();
			// The heuristic function from the astar solver uses the Manhattan distance
			part1 += coord::heuristic(from, to);
		}
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	for (const Galaxy& s1 : map)
	{
		for (const Galaxy& s2 : map)
		{
			if (s2.pos <= s1.pos)
			{
				continue;
			}
			coord from = s1.getPosition(part2times);
			coord to = s2.getPosition(part2times);
			part2 += coord::heuristic(from, to); // Manhattan distance
		}
	}
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s Part 2 (%lld times): %lld\n", filename, part2times, part2);
}

int main()
{
	Process("example.txt", 10, 374, 1030);
	Process("example.txt", 100, 374, 8410);
	Process("input.txt", 1000000);

	return 0;
}
