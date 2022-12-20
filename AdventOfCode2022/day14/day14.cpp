// day14.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 14/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct line
{
	line() = default;
	line(coord f, coord t) :
		from(f),
		to(t)
	{
	}
	coord from;
	coord to;
};

using Space = std::map<coord, char>;

void DrawLine(Space& space, const line& l)
{
	coord dc;
	dc.first = sgn(l.to.first - l.from.first);
	dc.second = sgn(l.to.second - l.from.second);
	for (coord c = l.from; c != l.to; c += dc)
	{
		space.insert(std::make_pair(c, '#'));
	}
	space.insert(std::make_pair(l.to, '#'));
}

bool TestCoord(Space& space, const coord& c)
{
	return (space.count(c) != 0); 
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	coord max;
	Space space;
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// e.g. "498,4 -> 498,6 -> 496,6"
				char* tok = strtok(thisLine, " ->");
				line l;
				while (tok)
				{
					coord point(tok);
					if (point.first > max.first) max.first = point.first;
					if (point.second > max.second) max.second = point.second;

					if (!l.from)
					{
						l.from = point;
					}
					else
					{
						l.to = point;
						DrawLine(space, l);
						l.from = l.to;
					}
					tok = strtok(nullptr, " ->");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	coord baseLeft(max);
	coord baseRight(max);
	const coord farLeft(-1000, 2);
	const coord farRight(1000, 2);
	baseLeft += farLeft;
	baseRight += farRight;
	line base(baseLeft, baseRight);
	DrawLine(space, base);
	// sand starts at 500,0
	bool intoTheVoid = false;
	const coord d(0, 1);
	const coord dl(-1, 1);
	const coord dr(1, 1);
	coord testPlug(500, 0);
	while (!TestCoord(space, testPlug))
	{
		coord sand(500, 0);
		while (true)
		{
			coord test1(sand);
			test1 += d;
			coord test2(sand);
			test2 += dl;
			coord test3(sand);
			test3 += dr;
			if (!TestCoord(space, test1))
			{
				sand = test1;
			}
			else if (!TestCoord(space, test2))
			{
				sand = test2;
			}
			else if (!TestCoord(space, test3))
			{
				sand = test3;
			}
			else
			{
				if (sand.second < max.second && !intoTheVoid)
					++part1;
				++part2;
				space.insert(std::make_pair(sand, 'o'));
				break;
			}
		}
		if (sand.second >= max.second)
			intoTheVoid = true;
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 24, 93);
	Process("input.txt");

	return 0;
}
