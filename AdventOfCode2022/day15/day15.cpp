// day15.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 15/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct sensor
{
	sensor() = default;
	sensor(std::string input);
	coord here;
	coord closestBeacon;
	int64 radius;
};

sensor::sensor(std::string input)
{
	// e.g.:
	// "Sensor at x=2, y=18: closest beacon is at x=-2, y=15"
	int m = sscanf(input.c_str(), "Sensor at x=%lld, y=%lld: closest beacon is at x=%lld, y=%lld", &here.first, &here.second, &closestBeacon.first, &closestBeacon.second);
	assert(m == 4);
	int64 dx = here.first - closestBeacon.first;
	int64 dy = here.second - closestBeacon.second;
	radius = abs(dx) + abs(dy);
}

// StartStops stores ranges of exclusions
struct StartStops : public std::set<std::pair<int64, int64>>
{
	void Merge(int64 x1, int64 x2)
	{
		// StartStops contains ranges of x that are blocked. Merging in new block from x1 to x2.
		auto range = std::make_pair(x1, x2);
		bool repeat;
		do
		{
			repeat = false;
			for (auto& p : *this)
			{
				// If p entirely contains range, there is nothing to do
				if (p.first <= x1 && p.second >= x2)
				{
					return;
				}
				// If these two ranges (p and range) overlap, remove p from set and merge p into range
				if ((p.first <= x1 && p.second >= x1) ||
					(p.first <= x2 && p.second >= x2) ||
					(x1 <= p.first && x2 >= p.first) ||
					(x1 <= p.second && x2 >= p.second))
				{
					range.first = min(range.first, p.first);
					range.second = max(range.second, p.second);
					erase(p);
					repeat = true;
					break;
				}
			}
		} while (repeat);
		insert(range);
	}

	bool RowIsFull(int64 part2max)
	{
		return (size() == 1 && begin()->first <= 0 && begin()->second >= part2max);
	}

	int64 Count()
	{
		int64 total = 0;
		for (auto& p : *this)
		{
			total += p.second + 1 - p.first;
		}
		return total;
	}
};



void Process(const char* filename, int64 part1testY, int64 part2max, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<sensor> sensors;

	// This maps x positions to whether there's a beacon.
	StartStops part1row;
	std::set<int64> part1beacons;

	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				sensor s(thisLine);
				sensors.push_back(s);

				// part 1
				// only considering the one row where y = part1testY, 
				if (s.closestBeacon.second == part1testY)
				{
					part1beacons.insert(s.closestBeacon.first);
				}
				if (abs(part1testY - s.here.second) <= s.radius)
				{
					int64 radius = s.radius - abs(part1testY - s.here.second);
					part1row.Merge(s.here.first - radius, s.here.first + radius);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = part1row.Count() - part1beacons.size();
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	// part 2
	// oh god the search space is 4000000 squared, so 16,000,000,000,000 tiles. No brute force!
	// test from the middle out (10, 9, 11, 8, etc.), testing one row at a time.
	// don't care about the other beacons, so just need to store ranges of x per line that cannot be our beacon.
	StartStops startStops;
	for (int64 y = part1testY, dy = -1; y < part2max; y += dy, dy += sgn(dy), dy = -dy)
	{
		startStops.clear();
		for (sensor& s : sensors)
		{
			if (abs(y - s.here.second) > s.radius)
				continue;
			int64 radius = s.radius - abs(y - s.here.second);
			const int64 x1 = s.here.first - radius;
			const int64 x2 = s.here.first + radius;
			startStops.Merge(x1, x2);
			// Check if we've filled this row
			if (startStops.RowIsFull(part2max))
				break;
		}
		if (!startStops.RowIsFull(part2max))
		{
			// found it!
			// assume not at the ends
			assert(startStops.size() == 2);
			int64 x = startStops.begin()->second + 1;
			part2 = x * 4000000 + y;
			break;
		}
		
	}
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 10, 20, 26, 56000011);
	Process("input.txt", 2000000, 4000000);

	return 0;
}
