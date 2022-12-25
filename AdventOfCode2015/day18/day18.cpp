// day18.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 25/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

bool verbose = false;
using LightMap = std::set<coord>;

void Print(const LightMap& lights, const coord& bottomRight)
{
	coord pos;
	for (pos.second = 0; pos.second <= bottomRight.second; ++pos.second)
	{
		for (pos.first = 0; pos.first <= bottomRight.first; ++pos.first)
		{
			if (lights.count(pos) > 0)
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

int64 ProcessLights(LightMap lights, const coord& bottomRight, int64 steps, int part)
{
	LightMap nextLights;
	coord pos;
	const coord topLeft(0, 0);
	const coord topRight(bottomRight.first, 0);
	const coord bottomLeft(0, bottomRight.second);
	for (int64 step = 0; step < steps; ++step)
	{
		if (part == 2)
		{
			lights.insert(topLeft);
			lights.insert(topRight);
			lights.insert(bottomLeft);
			lights.insert(bottomRight);
		}
		nextLights.clear();
		for (pos.second = 0; pos.second <= bottomRight.second; ++pos.second)
		{
			for (pos.first = 0; pos.first <= bottomRight.first; ++pos.first)
			{
				coord test;
				int count = 0;
				for (test.second = pos.second - 1; test.second <= pos.second + 1; ++test.second)
				{
					for (test.first = pos.first - 1; test.first <= pos.first + 1; ++test.first)
					{
						count += lights.count(test);
					}
				}
				// A light which is on stays on when 2 or 3 neighbors are on, and turns off otherwise.
				if (lights.count(pos) > 0)
				{
					if (count == 3 || count == 4) // count includes this light
					{
						nextLights.insert(pos);
					}
				}
				// A light which is off turns on if exactly 3 neighbors are on, and stays off otherwise.
				else
				{
					if (count == 3)
					{
						nextLights.insert(pos);
					}
				}
			}
		}
		lights = nextLights;
		if (verbose)
		{
			printf("\nAfter step %lld:\n", step + 1);
			Print(lights, bottomRight);
		}
	}
	if (part == 2)
	{
		lights.insert(topLeft);
		lights.insert(topRight);
		lights.insert(bottomLeft);
		lights.insert(bottomRight);
	}
	return lights.size();
}

void Process(const char* filename, int64 stepsPart1, int64 stepsPart2, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	LightMap lights;
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos;
	coord bottomRight(0, 0);
	pos.second = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (pos.second > bottomRight.second)
					bottomRight.second = pos.second;
				int64 len = strlen(thisLine);
				for (pos.first = 0; pos.first < len; ++pos.first)
				{
					if (pos.first > bottomRight.first)
						bottomRight.first = pos.first;
					char c = thisLine[pos.first];
					if (c == '#')
					{
						lights.insert(pos);
					}
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = ProcessLights(lights, bottomRight, stepsPart1, 1);

	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	part2 = ProcessLights(lights, bottomRight, stepsPart2, 2);

	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 4, 5, 4, 17);
	Process("input.txt", 100, 100);

	return 0;
}
