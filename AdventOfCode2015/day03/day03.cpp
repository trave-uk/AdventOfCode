// day03.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int expectedPart1 = -1, int expectedPart2 = -1)
{
	FILE *fp = fopen(filename, "rt");
	std::string line;
	if (!fp)
	{
		line = filename;
	}
	else
	{
		char* buffer = new char[65536];
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (*thisLine)
				{
					line = thisLine;
				}
			}
		}
		fclose(fp);
		delete[] buffer;
	}

	{
		std::set<coord> houses;
		coord pos;
		houses.insert(pos);
		for (char c : line)
		{
			switch (c)
			{
			case '>':
				++pos.first;
				break;
			case '<':
				--pos.first;
				break;
			case 'v':
				++pos.second;
				break;
			case '^':
				--pos.second;
				break;
			}
			houses.insert(pos);
		}
		int part1 = houses.size();
		assert(expectedPart1 == -1 || expectedPart1 == part1);
		printf("%s: Part 1: %d\n", filename, part1);
	}
	{
		std::set<coord> houses;
		coord pos[2];
		houses.insert(pos[0]);
		int index = 0;
		for (char c : line)
		{
			index = 1 - index;
			switch (c)
			{
			case '>':
				++pos[index].first;
				break;
			case '<':
				--pos[index].first;
				break;
			case 'v':
				++pos[index].second;
				break;
			case '^':
				--pos[index].second;
				break;
			}
			houses.insert(pos[index]);
		}
		int part2 = houses.size();
		assert(expectedPart2 == -1 || expectedPart2 == part2);
		printf("%s: Part 2: %d\n", filename, part2);
	}
}

int main()
{
	Process(">", 2);
	Process("^v", -1, 3);
	Process("^>v<", 4, 3);
	Process("^v^v^v^v^v", 2, 11);
	Process("input.txt");

	return 0;
}
