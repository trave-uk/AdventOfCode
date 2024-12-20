// day20.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 20/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Coord = std::pair<int, int>;
const std::array<Coord, 4> directionMap = {
	std::make_pair(0, -1),
	std::make_pair(1, 0),
	std::make_pair(0, 1),
	std::make_pair(-1, 0)
};

using Map = std::set<Coord>;

using DistanceMap = std::map<Coord, int>;

void CalculateDistances(DistanceMap& distanceMap, Map& map, const Coord& from)
{
	int distance = 0;
	Coord search = from;
	Coord last = std::make_pair(0, 0);
	for(;;)
	{
		distanceMap[search] = distance++;
		Coord next = search;
		for (int d = 0; d < 4; ++d)
		{
			Coord dir = directionMap[d];
			Coord newPos = search;
			newPos.first += dir.first;
			newPos.second += dir.second;
			if (map.count(newPos) && newPos != last)
			{
				next = newPos;
				break;
			}
		}
		last = search;
		if (next == search)
			break;
		search = next;
	}
}

void CountCheatingPaths(const DistanceMap& fromStart, const int minimumSaving, int& max2count, int& max20count)
{
	for (const auto& here : fromStart)
	{
		for (const auto& there : fromStart)
		{
			const int distance = abs(here.first.first - there.first.first) + abs(here.first.second - there.first.second);
			const int saving = there.second - here.second - distance;
			if (saving >= minimumSaving)
			{
				if (distance <= 20)
					++max20count;
				if (distance <= 2)
					++max2count;
			}
		}
	}
}

void Process(const char* filename, int minimumSaving, int expectedPart1 = -1, int expectedPart2 = -1)
{
	Map map;
	Coord start;
	Coord goal;

	Coord pos;
	pos.second = 0;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
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
					if (c == 'S')
					{
						start = pos;
					}
					else if (c == 'E')
					{
						goal = pos;
					}
					if (c != '#')
						map.insert(pos);
					++pos.first;
				}
			}
			++pos.second;
		}
	}
	fclose(fp);
	delete[] buffer;

	// Find the distances for each point from the start
	DistanceMap fromStart;
	CalculateDistances(fromStart, map, start);

	int part1 = 0;
	int part2 = 0;
	CountCheatingPaths(fromStart, minimumSaving, part1, part2);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %d\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %d\n", filename, part2);
}

int main()
{
	Process("example.txt", 50, 1, 285);
	Process("input.txt", 100);

	return 0;
}
