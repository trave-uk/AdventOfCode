// day10.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 10/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void TestPart1(int64& part1, int64 X, int cycle)
{
	if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220)
	{
		part1 += cycle * X;
	}
}

void TestPart2(std::array<std::array<char, 41>, 6>& part2, int64 X, int cycle)
{
	int x = (cycle - 1) % 40;
	int y = (cycle - 1) / 40;
	if (abs(x - X) <= 1)
	{
		part2[y][x] = '#';
	}
	else
	{
		part2[y][x] = '.';
	}
}

void Process(const char* filename, int64 expectedPart1 = -1)
{
	int64 part1 = 0;

	std::array<std::array<char, 41>, 6> part2;
	memset(&part2[0][0], ' ', 41 * 6);
	for (auto& row : part2)
	{
		row[40] = '\0';
	}

	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int cycle = 0;
	int64 X = 1;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				char op[80];
				int value;
				int matches = sscanf(thisLine, "%s %d", op, &value);
				std::string Op(op);
				if (Op == "noop")
				{
					assert(matches == 1);
					++cycle;
					TestPart1(part1, X, cycle);
					TestPart2(part2, X, cycle);
				}
				else if (Op == "addx")
				{
					assert(matches == 2);
					++cycle;
					TestPart1(part1, X, cycle);
					TestPart2(part2, X, cycle);
					++cycle;
					TestPart1(part1, X, cycle);
					TestPart2(part2, X, cycle);
					X += value;
				}
				else
				{
					assert(false);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	printf("%s: Part 2:\n", filename);
	for (auto row : part2)
	{
		printf("%s\n", &row.front());
	}
}

int main()
{
	Process("example.txt", 13140);
	Process("input.txt");

	return 0;
}
