// day18.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 18/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::set<coord>;

coord ORG = coord(0, 0);
coord UP = coord(0, -1);
coord DOWN = coord(0, 1);
coord LEFT = coord(-1, 0);
coord RIGHT = coord(1, 0);
const coord& Direction(char d)
{
	switch (d)
	{
	case 'U':
		return UP;
	case 'D':
		return DOWN;
	case 'L':
		return LEFT;
	case 'R':
		return RIGHT;
	}
	assert(false);
	return ORG;
}

std::string Directions("RDLU");

int64 Measure(const Map& map, const coord& lower, const coord& upper)
{
	int64 size = 0;
	coord pos = lower;
	for (pos.second = lower.second; pos.second <= upper.second; ++pos.second)
	{
		bool in = false;
		bool lastOn = false;
		for (pos.first = lower.first; pos.first <= upper.first; ++pos.first)
		{
			// #..#..###...#
			// 1111  1111111
			bool on = map.count(pos) > 0;
			if (on && on != lastOn)
			{
				in = !in;
			}
			if (in || on)
			{
				++size;
			}
			lastOn = on;
		}
	}
	return size;
}

struct Line
{
	Line(char dir, int64 dist, std::string colour)
		: dir(dir)
		, dist(dist)
		, colour(colour)
	{
	}

	void ConvertPart2()
	{
		dir = Directions[colour[5]-'0'];
		std::string hex = colour.substr(0, 5);
		dist = strtoll(hex.c_str(), nullptr, 16);
	}

	char dir;
	int64 dist;
	std::string colour;
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");

	std::vector<Line> part1Lines;
	std::vector<Line> part2Lines;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// R 6 (#70c710)
				char dir;
				int64 dist;
				char col[7];
				int found = sscanf(thisLine, "%c %lld (#%[0-9a-f]s)", &dir, &dist, col);
				assert(found == 3);

				Line l(dir, dist, std::string(col));
				part1Lines.push_back(l);

				l.ConvertPart2();
				part2Lines.push_back(l);
			}
		}
	}
	fclose(fp);
	delete[] buffer;


	// part 1
	int64 part1 = 0;
	Map map;
	coord pos(0, 0);
	coord lower(0, 0);
	coord upper(0, 0);

	for (Line l : part1Lines)
	{
		const coord& direction = Direction(l.dir);
		for (int64 i = 0; i < l.dist; ++i)
		{
			pos += direction;
			assert(map.count(pos) == 0);
			map.insert(pos);

			upper.first = max(pos.first, upper.first);
			upper.second = max(pos.second, upper.second);
			lower.first = min(pos.first, lower.first);
			lower.second = min(pos.second, lower.second);
		}
	}
	assert(pos.length() == 0);

	// Measure the space within the shape.
	part1 = Measure(map, lower, upper);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);


	// part 2
	// Need to measure between the outer corners of the path (assuming clockwise:)
	// RIGHT, DOWN -> top right           ^>>
	// LEFT, DOWN -> bottom right         ^#v
	// RIGHT, UP -> top left          ^>>>>#v>>>>
	// LEFT, UP -> bottom left        ^#####<<<<v
	// DOWN, RIGHT -> top right       <<^###v
	// DOWN, LEFT -> bottom right       ^###v
	// UP, RIGHT -> top left            ^###v
	// UP, LEFT -> bottom left          <<<<v

#define DIR2(c, d) (int(c) * 256 + d)
	// First collect the positions the path moves between
	pos = coord(0, 0);
	std::vector<coord> vertices;
	char prevDir = part2Lines.back().dir;
	for (const Line& l : part2Lines)
	{
		int corner = DIR2(prevDir, l.dir);
		coord offset(0, 0);
		switch (corner)
		{
		case DIR2('R', 'D'):
		case DIR2('D', 'R'):
			offset.first = 1;
			break;
		case DIR2('L', 'D'):
		case DIR2('D', 'L'):
			offset.first = offset.second = 1;
			break;
		case DIR2('L', 'U'):
		case DIR2('U', 'L'):
			offset.second = 1;
		}
		offset += pos;
		vertices.push_back(offset);

		coord direction = Direction(l.dir);
		direction *= l.dist;
		pos += direction;
		prevDir = l.dir;
	}

	// Now use the trapezoid formula to calculate the area. https://en.wikipedia.org/wiki/Shoelace_formula#Trapezoid_formula
	int64 part2 = 0;
	pos = vertices.back();
	for (const coord& v : vertices)
	{
		coord start = pos;
		coord end = v;
		int64 width = start.second - end.second;
		int64 height2 = start.first + end.first;
		assert((width % 2) == 0 || (height2 % 2) == 0);
		part2 -= width * height2 / 2;

		pos = end;
	}
	// If the path was anticlockwise, the total would be negative.
	// Note though that also the offsets would be to the inner corner so the answer would be wrong.
	// TODO: spot this and recalculate the vertices assuming anticlockwise.
	part2 = abs(part2);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 62, 952408144115ll);
	Process("input.txt");

	return 0;
}
