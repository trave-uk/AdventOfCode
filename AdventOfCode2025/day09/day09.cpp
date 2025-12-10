// day09.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 09/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Tiles = std::vector<coord>;
using TileSet = std::set<coord>;

int64 LargestRectangle(const Tiles& tiles)
{
	int64 maxSize = 0;
	int64 numTiles = tiles.size();
	for (int64 i = 0; i < numTiles; ++i)
	{
		const coord& tile1 = tiles[i];
		for (int64 j = i + 1; j < numTiles; ++j)
		{
			const coord& tile2 = tiles[j];
			coord delta = tile1;
			delta -= tile2;
			int64 size = (delta.first + 1) * (delta.second + 1);
			if (size > maxSize)
				maxSize = size;
		}
	}
	return maxSize;
}

int CountGreenTilesInDirection(const coord& point, const coord& delta, const TileSet& greenTiles, const coord& maxPos)
{
	int count = 0;
	for (coord test = point;
		test.first <= maxPos.first && test.second <= maxPos.second && test.first >= 0 && test.second >= 0;
		test += delta)
	{
		if (greenTiles.count(test))
			++count;
	}
	return count;
}

bool PointWithinGreenTiles(const coord& point, const TileSet& greenTiles, const coord& maxPos)
{
	if (greenTiles.count(point))
		return true;
	return false;
/*	coord delta;
	int valid = 0;
	for (delta.first = -1; delta.first <= 1; ++delta.first)
	{ 
		for (delta.second = -1; delta.second <= 1; ++delta.second)
		{
			if (!delta.length())
				continue;
			int walls = CountGreenTilesInDirection(point, delta, greenTiles, maxPos);
			if ((walls % 2) == 1)
				++valid;
		}
	}
	return valid >= 6;*/
}

void FloodFill(TileSet& greenTiles, const coord& point)
{
	std::queue<coord> queue;
	queue.push(point);
	while (!queue.empty())
	{
		coord p = queue.front();
		queue.pop();
		if (greenTiles.count(p))
			continue;
		greenTiles.insert(p);
		queue.push(coord(p.first, p.second - 1));
		queue.push(coord(p.first, p.second + 1));
		queue.push(coord(p.first - 1, p.second));
		queue.push(coord(p.first + 1, p.second));
	}
}

int64 LargestRectangle2(const Tiles& tiles)
{
	TileSet greenTiles;
	coord prevTile = *tiles.rbegin();
	coord minPos(100000,100000), maxPos(0,0);
	for (const coord& tile : tiles)
	{
		if (tile.first < minPos.first)
			minPos.first = tile.first;
		if (tile.first > maxPos.first)
			maxPos.first = tile.first;
		if (tile.second < minPos.second)
			minPos.second = tile.second;
		if (tile.second > maxPos.second)
			maxPos.second = tile.second;
		coord delta = tile;
		delta -= prevTile;
		coord unit = delta;
		unit /= delta.length();
		for (coord greenTile = prevTile; greenTile != tile; greenTile += unit)
		{
			assert(!greenTiles.count(greenTile));
			greenTiles.insert(greenTile);
		}
		prevTile = tile;
	}
	// Flood fill from "centre" (guessing midpoint between minPos and maxPos)
	coord centre = maxPos;
	centre -= minPos;
	centre /= 2;
	coord offset = centre;
	offset /= 10;
	centre += offset;
	centre += minPos;
	FloodFill(greenTiles, centre);

	int64 maxSize = 0;
	int64 numTiles = tiles.size();
	for (int64 i = 0; i < numTiles; ++i)
	{
		const coord& tile1 = tiles[i];
		for (int64 j = i + 1; j < numTiles; ++j)
		{
			const coord& tile2 = tiles[j];
			coord delta = tile1;
			delta -= tile2;
			int64 size = (delta.first + 1) * (delta.second + 1);
			if (size > maxSize)
			{
				coord test;
				coord d;
				d.first = (tile2.first > tile1.first) - (tile1.first > tile2.first);
				d.second = (tile2.second > tile1.second) - (tile1.second > tile2.second);
				bool allow = true;
				test.second = tile1.second;
				for (test.first = tile1.first; test.first != tile2.first; test.first += d.first)
				{
					if (!PointWithinGreenTiles(test, greenTiles, maxPos))
					{
						allow = false;
						break;
					}
				}
				if (!allow)
					continue;
				test.second = tile2.second;
				for (test.first = tile1.first; test.first != tile2.first; test.first += d.first)
				{
					if (!PointWithinGreenTiles(test, greenTiles, maxPos))
					{
						allow = false;
						break;
					}
				}
				if (!allow)
					continue;
				test.first = tile1.first;
				for (test.second = tile1.second; test.second != tile2.second; test.second += d.second)
				{
					if (!PointWithinGreenTiles(test, greenTiles, maxPos))
					{
						allow = false;
						break;
					}
				}
				if (!allow)
					continue;
				test.first = tile2.first;
				for (test.second = tile1.second; test.second != tile2.second; test.second += d.second)
				{
					if (!PointWithinGreenTiles(test, greenTiles, maxPos))
					{
						allow = false;
						break;
					}
				}
				if (!allow)
					continue;
				maxSize = size;
			}
		}
	}
	return maxSize;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Tiles redTiles;
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
				coord tile;
				int count = sscanf(thisLine, "%lld,%lld", &tile.first, &tile.second);
				assert(count == 2);
				redTiles.push_back(tile);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = LargestRectangle(redTiles);
	
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	part2 = LargestRectangle2(redTiles);
	
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 50, 24);
	Process("input.txt");

	return 0;
}
