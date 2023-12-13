// day13.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 13/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::set<coord>;

int64 CalculatePart1(const Map& map, int64 width, int64 height, int64 ignore = -1)
{
	for (int64 vert = 1; vert < width; ++vert)
	{
		Map left;
		Map _right;
		for (const coord& pos : map)
		{
			if (pos.first < vert)
			{
				if (pos.first >= vert - (width - vert))
					left.insert(pos);
			}
			else
			{
				if (pos.first < vert + vert)
				{
					coord reflected = pos;
					reflected.first = vert - (pos.first - vert) - 1;
					_right.insert(reflected);
				}
			}
		}
		if (left == _right)
		{
			if (vert != ignore)
				return vert;
		}
	}
	for (int64 horz = 1; horz < height; ++horz)
	{
		Map above;
		Map _below;
		for (const coord& pos : map)
		{
			if (pos.second < horz)
			{
				if (pos.second >= horz - (height - horz))
					above.insert(pos);
			}
			else
			{
				if (pos.second < horz + horz)
				{
					coord reflected = pos;
					reflected.second = horz - (pos.second - horz) - 1;
					_below.insert(reflected);
				}
			}
		}
		if (above == _below)
		{
			if (ignore != 100 * horz)
				return 100 * horz;
		}
	}
	return 0;
}

int64 CalculatePart2(const Map& map, int64 width, int64 height)
{
	int64 part1 = CalculatePart1(map, width, height);
	for (int64 y = 0; y < height; ++y)
	{
		for (int64 x = 0; x < width; ++x)
		{
			coord pos(x, y);
			Map copy = map;
			if (copy.count(pos))
			{
				copy.erase(pos);
			}
			else
			{
				copy.insert(pos);
			}
			int64 part2 = CalculatePart1(copy, width, height, part1);
			if (part2 != 0)
			{
				return part2;
			}
		}
	}
	assert(false);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	int64 y = 0;
	int64 x = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (!*thisLine)
			{
				// blank line between maps
				part1 += CalculatePart1(map, x, y);
				part2 += CalculatePart2(map, x, y);
				y = 0;
				map = Map();
			}
			if (*thisLine)
			{
				std::string line(thisLine);
				x = 0;
				for (char c : line)
				{
					if (c == '#')
					{
						coord pos(x, y);
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
	part1 += CalculatePart1(map, x, y);
	part2 += CalculatePart2(map, x, y);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 405, 400);
	Process("input.txt");

	return 0;
}
