// day21.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 21/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::map<coord, char>;

void WrapCoord(coord& pos, const coord& size)
{
	pos.first %= size.first;
	if (pos.first < 0)
	{
		pos.first += size.first;
	}

	pos.second %= size.second;
	if (pos.second < 0)
	{
		pos.second += size.second;
	}
}

void PrintMap(const Map& map, const std::set<coord> points, const coord& size, const coord& topLeft, const coord &bottomRight)
{
	coord pos(0, 0);
	for (pos.second = topLeft.second; pos.second < bottomRight.second; ++pos.second)
	{
		for (pos.first = topLeft.first; pos.first < bottomRight.first; ++pos.first)
		{
			char c;
			if (points.count(pos) > 0)
			{
				c = 'O';
			}
			else
			{
				coord pos2 = pos;
				WrapCoord(pos2, size);
				c = map.at(pos2);
			}
			_putch(c);
		}
		_putch('\n');
	}
}

bool Passable(const Map& map, const coord& size, const coord& pos)
{
	coord pos2 = pos;
	WrapCoord(pos2, size);
	return map.at(pos2) != '#';
}

int64 CalculatePart1(coord start, Map& map, const coord& size, int64 maxSteps)
{
	// Flood-fill from start maxSteps times. Every even step, increment total.
	std::map<coord, int64> filled;
	std::set<coord> part1points;
	std::set<std::pair<coord, int64>> test;
	int64 steps = 0;
	test.insert(std::make_pair(start, steps));
	int64 parity = maxSteps % 2;
	coord topLeft(0,0);
	coord bottomRight(size);
	while (!test.empty())
	{
		auto t = test.begin();
		coord pos = t->first;
		steps = t->second;
		test.erase(t);
		if (filled.count(pos) > 0 && filled[pos] <= steps)
			continue;
		filled[pos] = steps;
		if (!pos.isContained(topLeft, bottomRight))
		{
			while (pos.first < topLeft.first) topLeft.first -= size.first;
			while (pos.first > bottomRight.first) bottomRight.first += size.first;
			while (pos.second < topLeft.second) topLeft.second -= size.second;
			while (pos.second > bottomRight.second) bottomRight.second += size.second;
		}
		if ((steps % 2) == parity)
		{
			part1points.insert(pos);
//			map[pos] = 'o';
		}
//		else
//		{
//			map[pos] = 'X';
//		}
		++steps;
		if (steps > maxSteps)
			continue;
		for (int d = 0; d < 4; ++d)
		{
			coord delta((d == 0) - (d == 1), (d == 2) - (d == 3));
			delta += pos;
			if (Passable(map, size, delta))
			{
				test.insert(std::make_pair(delta, steps));
			}
		}
	}
	if (maxSteps <= 100)
		PrintMap(map, part1points, size, topLeft, bottomRight);
	return part1points.size();
}

void Process(const char* filename, int64 part1steps, int64 part2steps, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	coord pos(0, 0);
	coord start(0, 0);
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				pos.first = 0;
				std::string line(thisLine);
				for (char c : line)
				{
					map[pos] = c;
					if (c == 'S')
						start = pos;
					++pos.first;
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	coord size = pos;

	Map part1map = map;
	part1 = CalculatePart1(start, part1map, size, part1steps);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	Map part2map = map;
	part2 = CalculatePart1(start, part2map, size, part2steps);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 6, 10, 16, 50);
	Process("example.txt", 50, 100, 1594, 6536);
	Process("example.txt", 500, 1000, 167004, 668697);
	Process("example.txt", 6, 5000, 16, 16733044);
	Process("input.txt", 64, 26501365, 3562);

	return 0;
}

// 2835 too low