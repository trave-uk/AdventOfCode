// day12.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 12/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// To find each region, use a flood fill. Then, for each plot which has been filled, 
// count neighbouring plots of the same plants and the amount of fence required will
// be (4 - number of neighbours).

const coord RIGHT(1, 0);
const coord LEFT(-1, 0);
const coord UP(0, -1);
const coord DOWN(0, 1);

struct Map : public std::vector<std::string>
{
	int64 width = 0;
	int64 height = 0;

	void push_back(const std::string& value)
	{
		std::vector<std::string>::push_back(value);
		++height;
		width = value.length();
	}

	char& operator[](const coord& pos)
	{
		static char outside = '.';
		if (pos.second < 0 || pos.second >= height || pos.first < 0 || pos.first >= width)
			return outside;
		std::string& line = this->at(pos.second);
		return line[pos.first];
	}
};

using Fences = std::set<coord>;

int64 countConcurrentFences(const Fences& fences, const coord& back, const coord& forward)
{
	int64 count = 0;
	Fences searchFences = fences;
	// find neighbouring fences and remove them, each group counting as one fence
	while (!searchFences.empty())
	{
		coord f = *searchFences.begin();
		// up and down from f
		while (searchFences.count(f))
		{
			f += back;
		}
		f += forward;
		while (searchFences.count(f))
		{
			searchFences.erase(f);
			f += forward;
		}
		++count;
	}
	return count;
}

struct Region
{
	char type;
	std::set<coord> plots;
	int64 area = 0;
	int64 perimeter = 0;

	void floodFill(Map& map, coord pos)
	{
		if (map[pos] == type)
		{
			++area;
			map[pos] = tolower(type);
			plots.insert(pos);
			floodFill(map, coord(pos.first - 1, pos.second));
			floodFill(map, coord(pos.first + 1, pos.second));
			floodFill(map, coord(pos.first, pos.second - 1));
			floodFill(map, coord(pos.first, pos.second + 1));
		}
	}

	void measurePerimeter(int part = 1)
	{
		Fences leftFences;
		Fences rightFences;
		Fences upFences;
		Fences downFences;
		perimeter = 0;
		for (coord plot : plots)
		{
			coord left = plot; left += LEFT;
			if (!plots.count(left))
				leftFences.insert(plot);
			coord right = plot; right += RIGHT;
			if (!plots.count(right))
				rightFences.insert(plot);
			coord up = plot; up += UP;
			if (!plots.count(up))
				upFences.insert(plot);
			coord down = plot; down += DOWN;
			if (!plots.count(down))
				downFences.insert(plot);
		}
		if (part == 1)
		{
			perimeter = leftFences.size() + rightFences.size() + upFences.size() + downFences.size();
		}
		if (part == 2)
		{
			perimeter += countConcurrentFences(leftFences, UP, DOWN);
			perimeter += countConcurrentFences(rightFences, UP, DOWN);
			perimeter += countConcurrentFences(upFences, LEFT, RIGHT);
			perimeter += countConcurrentFences(downFences, LEFT, RIGHT);
		}
	}
};

using Regions = std::vector<Region>;

int64 CalculateScore(const Map& m, int part = 1)
{
	Map map = m;
	Regions regions;
	coord pos;
	for (pos.second = 0; pos.second < map.height; ++pos.second)
	{
		for (pos.first = 0; pos.first < map.width; ++pos.first)
		{
			if (isupper(map[pos]))
			{
				Region r;
				r.type = map[pos];
				r.floodFill(map, pos);
				if (part == 1)
					r.measurePerimeter(1);
				else
					r.measurePerimeter(2);
				regions.push_back(r);
			}
		}
	}

	int64 total = 0;
	for (const Region& r : regions)
	{
		total += r.area * r.perimeter;
	}
	return total;
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

	part1 = CalculateScore(map);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	part2 = CalculateScore(map, 2);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example1.txt", 140, 80);
	Process("example2.txt", 772, 436);
	Process("example3.txt", 1930);
	Process("example4.txt", -1, 236);
	Process("example5.txt", -1, 368);

	Process("input.txt");

	return 0;
}
