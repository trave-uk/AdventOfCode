// day09.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 9/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"


void Process(const char* filename)
{
	std::vector<std::string> heightmap;
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
				heightmap.push_back(thisLine);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	const int w = heightmap[0].length();
	const int h = heightmap.size();
	int64 part1 = 0;
	int count = 0;
	std::vector<coord> lowPoints;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			char here = heightmap[y][x];
			bool bLow = true;
			for (int i = 0; i < 4; ++i)
			{
				int dx = x + (i == 0) - (i == 1);
				int dy = y + (i == 2) - (i == 3);
				if (dx < 0 || dx >= w || dy < 0 || dy >= h)
					continue;
				char there = heightmap[dy][dx];
				if (there <= here)
				{
					bLow = false;
					break;
				}
			}
			if (bLow)
			{
				++count;
				part1 += here - '0' + 1;
				lowPoints.push_back(coord(x, y));
			}
		}
	}
	printf("%s: Part 1: %lld\n", filename, part1);
	std::vector<int64> basinSizes;
	for (coord pos : lowPoints)
	{
		// I'm not certain that each basin has just one low point. The puzzle seems to say so, but it doesn't sound right
		if (heightmap[pos.second][pos.first] == '9')
		{
			// This lowpoint was in a basin with another one, so skip it.
			assert(false);
			continue;
		}
		// Floodfill the heightmap from this point, filling in with '9'.
		int64 basinSize = 0;
		std::queue<coord> openSet;
		openSet.push(pos);
		while (!openSet.empty())
		{
			coord p = openSet.front();
			openSet.pop();
			char& v = heightmap[p.second][p.first];
			if (v < '9')
			{
				++basinSize;
				v = '9';
				for (int i = 0; i < 4; ++i)
				{
					coord q = p;
					q.first += (i == 0) - (i == 1);
					q.second += (i == 2) - (i == 3);
					if (q.first < 0 || q.first >= w || q.second < 0 || q.second >= h)
						continue;
					openSet.push(q);
				}
			}
		}
		basinSizes.push_back(basinSize);
	}
	std::sort(basinSizes.rbegin(), basinSizes.rend());
	int64 part2 = basinSizes[0] * basinSizes[1] * basinSizes[2];
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
