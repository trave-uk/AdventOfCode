// day10.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 10/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::vector<std::string>;
using CoordStore = std::set<coord>;
using Trail = std::vector<coord>;
using TrailStore = std::set<Trail>;

char PeekMap(const Map& map, const coord& pos)
{
	const int64 width = map[0].length();
	const int64 height = map.size();
	if (pos.first < 0 || pos.first >= width || pos.second < 0 || pos.second >= height)
		return '.';
	return map[pos.second][pos.first];
}

std::array<coord, 4> dirs = {
	coord(-1, 0),
	coord(1, 0),
	coord(0, 1),
	coord(0, -1)
};

int64 CountPaths(const Map& map, int part = 1)
{
	int64 result = 0;
	const int64 width = map[0].length();
	const int64 height = map.size();
	// For each trailhead (0s), find all paths to a 9. Store where the 9s are, to count them.
	coord pos;
	for (pos.second = 0; pos.second < height; ++pos.second)
	{
		for (pos.first = 0; pos.first < width; ++pos.first)
		{
			if (PeekMap(map, pos) == '0')
			{
				CoordStore heights[10];
				heights[0].insert(pos);
				TrailStore trails[10];
				Trail start;
				start.push_back(pos);
				trails[0].insert(start);
				for (int h = 1; h <= 9; ++h)
				{
					char tile = h + '0';
					for (auto i : trails[h - 1])
					{
						for (coord dir : dirs)
						{
							coord newPos = i.back();
							newPos += dir;
							if (PeekMap(map, newPos) == tile)
							{
								heights[h].insert(newPos);
								Trail j = i;
								j.push_back(newPos);
								trails[h].insert(j);
							}
						}
					}
				}
				int64 score = 0;
				if (part == 1)
				{
					score = heights[9].size();
				}
				else
				{
					score = trails[9].size();
				}
				result += score;
			}
		}
	}
	return result;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				std::string line(thisLine);
				map.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = CountPaths(map);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	part2 = CountPaths(map, 2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 36, 81);
	Process("input.txt");

	return 0;
}
