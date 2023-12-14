// day14.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 14/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 CalculateLoad(const std::set<coord>& rocks, coord size)
{
	int64 load = 0;
	for (const coord& rock : rocks)
	{
		load += size.second - rock.second;
	}
	return load;
}

void Slide(std::set<coord>& rocks, const std::set<coord>& blocks, coord size, coord dir)
{
	std::vector<coord> rocksVector(rocks.begin(), rocks.end());
	for (;;)
	{
		bool anyMoved = false;
		for (coord& rock : rocksVector)
		{
			for (;;)
			{
				bool blocked = false;
				coord test = rock;
				test += dir;
				if (test.first < 0 || test.first >= size.first || test.second < 0 || test.second >= size.second)
				{
					break;
				}
				for (const coord otherRock : rocksVector)
				{
					if (otherRock == test)
						blocked = true;
				}
				if (blocks.count(test))
				{
					blocked = true;
				}
				if (blocked)
				{
					break;
				}
				anyMoved = true;
				rock = test;
			}
		}
		if (!anyMoved)
			break;
	}
	rocks = std::set<coord>(rocksVector.begin(), rocksVector.end());
}

void Print(const std::set<coord>& rocks, const std::set<coord>& blocks, coord size)
{
	coord pos;
	for (pos.second = 0; pos.second < size.second; ++pos.second)
	{
		for (pos.first = 0; pos.first < size.first; ++pos.first)
		{
			if (rocks.count(pos))
			{
				_putch('O');
			}
			else if (blocks.count(pos))
			{
				_putch('#');
			}
			else
			{
				_putch('.');
			}
		}
		_putch('\n');
	}
	_putch('\n');
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::set<coord> rocks;
	std::set<coord> blocks;
	coord size;
	int64& y = size.second = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				int64& x = size.first = 0;
				for (char c : line)
				{
					coord pos(x, y);
					if (c == '#')
						blocks.insert(pos);
					else if (c == 'O')
						rocks.insert(pos);
					++x;
				}
				++y;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	Slide(rocks, blocks, size, coord(0, -1));
//	Print(rocks, blocks, size);
	part1 = CalculateLoad(rocks, size);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	// repeat N, W, S, E 1000000000 times; first "N" does nothing as they already slid N for part 1.
	std::set<coord> history = rocks;
	int64 date = 0;
	bool skipped = false;
	int64 limit = 200;
	for (int64 i = 0; i < 1000000000; ++i)
	{
		Slide(rocks, blocks, size, coord(0, -1));
		Slide(rocks, blocks, size, coord(-1, 0));
		Slide(rocks, blocks, size, coord(0, 1));
		Slide(rocks, blocks, size, coord(1, 0));
//		if (i < 3)
//		{
//			printf("After %lld cycles:\n", i + 1);
//			Print(rocks, blocks, size);
//		}
		if (i == limit)
		{
			history = rocks;
			date = i;
		}
		else if (!skipped && i > limit && rocks == history)
		{
			int64 period = i - date;
			int64 left = 1000000000 - 1 - i;
			int64 periods = left / period;
			i += period * periods;
			skipped = true;
		}
	}
	part2 = CalculateLoad(rocks, size);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 136, 64);
	Process("input.txt");

	return 0;
}
