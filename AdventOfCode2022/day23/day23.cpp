// day23.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 23/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Elf
{
	Elf() = default;
	Elf(coord p)
		: pos(p)
		, nextPos(p)
	{}
	coord pos;
	coord nextPos;
};

using Elves = std::map<coord, Elf>;

void Bounds(Elves& elves, coord& _min, coord& _max)
{
	_min = coord(INT64_MAX, INT64_MAX);
	_max = coord(INT64_MIN, INT64_MIN);
	for (auto& it : elves)
	{
		coord pos = it.first;
		_min.first = min(pos.first, _min.first);
		_min.second = min(pos.second, _min.second);
		_max.first = max(pos.first, _max.first);
		_max.second = max(pos.second, _max.second);
	}
}

int64 Size(Elves& elves)
{
	coord _min, _max;
	Bounds(elves, _min, _max);
	coord bounds = _max;
	bounds -= _min;
	return (bounds.first + 1) * (bounds.second + 1);
}

void Print(Elves elves)
{
	coord _min, _max;
	Bounds(elves, _min, _max);
	coord pos;
	for (pos.second = _min.second; pos.second <= _max.second; ++pos.second)
	{
		for (pos.first = _min.first; pos.first <= _max.first; ++pos.first)
		{
			if (elves.count(pos) > 0)
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
}

coord look[4][3] =
{
	{ coord(-1, -1), coord(0, -1), coord(1, -1) }, // NW, N, NE
	{ coord(-1, 1), coord(0, 1), coord(1, 1) }, // SW, S, SE
	{ coord(-1, -1), coord(-1, 0), coord(-1, 1) }, // NW, W, SW
	{ coord(1, -1), coord(1, 0), coord(1, 1) } // NE, E, SE
};

int64 ProcessElves(Elves elves, int part)
{
	int64 steps = (part == 1) ? 10 : 1000000;
	int64 step;
	for (step = 0; step < steps; ++step)
	{
		// Each elf chooses a position to move to
		int look1 = step % 4;
		bool bMoved = false;
		for (auto& it : elves)
		{
			Elf& elf = it.second;
			elf.nextPos = elf.pos;

			// First, are there any neighbouring elves?
			int count = 0;
			coord d;
			for (d.first = -1; d.first <= 1; ++d.first)
			{
				for (d.second = -1; d.second <= 1; ++d.second)
				{
					coord pos = elf.pos;
					pos += d;
					count += elves.count(pos);
				}
			}
			// No neighbours (1 = self) so don't move
			if (count == 1)
			{
				continue;
			}

			for (int i = 0; i < 4; ++i)
			{
				int li = (look1 + i) % 4;
				int count = 0;
				for (int lj = 0; lj < 3; ++lj)
				{
					coord test = elf.pos;
					test += look[li][lj];
					count += elves.count(test);
				}
				if (count == 0)
				{
					elf.nextPos = elf.pos; 
					elf.nextPos += look[li][1];
					break;
				}
			}
		}

		// Check for collisions
		for (auto& it1 : elves)
		{
			bool bCollision = false;
			Elf& elf1 = it1.second;
			for (auto& it2 : elves)
			{
				Elf& elf2 = it2.second;
				if (elf1.pos == elf2.pos)
					continue;
				if (elf1.nextPos == elf2.nextPos)
				{
					bCollision = true;
					elf2.nextPos = elf2.pos;
				}
			}
			if (bCollision)
				elf1.nextPos = elf1.pos;
		}

		// Every elf moves
		Elves newElves;
		for (auto& it : elves)
		{
			Elf& elf = it.second;
			if (elf.pos != elf.nextPos)
				bMoved = true;
			elf.pos = elf.nextPos;
			newElves.insert(std::make_pair(elf.pos, elf));
		}
		elves = newElves;

		if (!bMoved)
			break;
	}
	if (part == 1)
		return Size(elves) - elves.size();
	
	return step + 1;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Elves elves;
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos;
	pos.second = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				for (pos.first = 0; pos.first < strlen(thisLine); ++pos.first)
				{
					if (thisLine[pos.first] == '#')
					{
						Elf elf(pos);
						elves.insert(std::make_pair(elf.pos, elf));
					}
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = ProcessElves(elves, 1);

	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	part2 = ProcessElves(elves, 2);

	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 110, 20);
	Process("input.txt");

	return 0;
}
