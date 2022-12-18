// day18.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 18/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Coord
{
	Coord() = default;
	Coord(const char *input)
	{
		int m = sscanf(input, "%d,%d,%d", &x, &y, &z);
		assert(m == 3);
	}
	bool operator<(const Coord& r) const
	{
		return (x < r.x || (x == r.x && y < r.y) || (x == r.x && y == r.y && z < r.z));
	}
	int x = 0;
	int y = 0;
	int z = 0;
};

using Cloud = std::set<Coord>;

int64 CountFaces(const Cloud& cloud)
{
	int64 count = 0;
	for (auto& c : cloud)
	{
		for (int i = 0; i < 6; ++i)
		{
			int dx = (i == 0) - (i == 1);
			int dy = (i == 2) - (i == 3);
			int dz = (i == 4) - (i == 5);
			Coord d(c);
			d.x += dx;
			d.y += dy;
			d.z += dz;
			if (!cloud.count(d))
				++count;
		}
	}
	return count;
}

using VolumeCollection = std::set<Cloud>;

Cloud FloodFill(Coord test, const Cloud& lava, const Coord& min, const Coord& max)
{
	Cloud pocket;
	pocket.clear();
	// Use a stack rather than a queue to make this depth-first so that if we're going out of the bounds we do so relatively quickly
	std::stack<Coord> points;
	points.push(test);
	while (!points.empty())
	{
		Coord c = points.top();
		points.pop();
		pocket.insert(c);
		for (int i = 0; i < 6; ++i)
		{
			int dx = (i == 0) - (i == 1);
			int dy = (i == 2) - (i == 3);
			int dz = (i == 4) - (i == 5);
			Coord d(c);
			d.x += dx;
			d.y += dy;
			d.z += dz;
			// If d is outside (min,max) then return an empty pocket
			if (d.x < min.x || d.y < min.y || d.z < min.z || d.x > max.x || d.y > max.y || d.z > max.z)
			{
				pocket.clear();
				return pocket;
			}
			if (!lava.count(d) && !pocket.count(d))
			{
				points.push(d);
			}
		}
	}
	return pocket;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Cloud lava;
	Coord minBounds("1000,1000,1000");
	Coord maxBounds;
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
				Coord c(thisLine);
				lava.insert(c);
				minBounds.x = min(c.x, minBounds.x);
				minBounds.y = min(c.y, minBounds.y);
				minBounds.z = min(c.z, minBounds.z);
				maxBounds.x = max(c.x, maxBounds.x);
				maxBounds.y = max(c.y, maxBounds.y);
				maxBounds.z = max(c.z, maxBounds.z);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = CountFaces(lava);

	if (expectedPart1 != -1)
	{
		printf("%s: Part 1 expected: %lld\n", filename, expectedPart1);
	}
	printf("%s: Part 1: %lld\n", filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	// Part 2. To find air pockets, for each exposed side of each cube, flood fill, canceling if going outside the cloud bounds
	// Store these in another set to remove duplicates
	// Then, for all unique such sets, count the outside faces and subtract this from the answer to part 1.

	VolumeCollection pockets;

	for (auto& c : lava)
	{
		for (int i = 0; i < 6; ++i)
		{
			int dx = (i == 0) - (i == 1);
			int dy = (i == 2) - (i == 3);
			int dz = (i == 4) - (i == 5);
			Coord d(c);
			d.x += dx;
			d.y += dy;
			d.z += dz;
			if (!lava.count(d))
			{
				Cloud pocket = FloodFill(d, lava, minBounds, maxBounds);
				if (!pocket.empty())
				{
					pockets.insert(pocket);
				}
			}
		}
	}

	int64 part2 = part1;
	for (auto& pocket : pockets)
	{
		part2 -= CountFaces(pocket);
	}

	if (expectedPart2 != -1)
	{
		printf("%s: Part 2 expected: %lld\n", filename, expectedPart2);
	}
	printf("%s: Part 2: %lld\n", filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 64, 58);
	Process("input.txt");

	return 0;
}
