// day03.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 03/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

char findSymbol(std::vector<std::string>& map, int64 x1, int64 y1, int64 x2, int64 y2, coord &position)
{
	const int64 height = map.size();
	for (int64 y = y1; y <= y2; ++y)
	{
		if (y < 0 || y >= height)
			continue;
		std::string& line = map[y];
		for (int64 x = x1; x <= x2; ++x)
		{
			const int64 width = line.length();
			if (x < 0 || x >= width)
				continue;
			char c = line[x];
			if (!isdigit(c) && c != '.')
			{
				position = coord(x, y);
				return c;
			}
		}
	}
	return '\0';
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<std::string> map;
	std::map<coord, std::vector<int>> gears; // potential gears. Only valid when exactly two number are adjacent
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				map.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// In each line, find numbers. When a number found, look around the number (previous, next line) for a symbol
	int64 height = map.size();
	for (int64 y = 0; y < height; ++y)
	{
		std::string& line = map[y];
		int64 x = 0;
		bool digit = false;
		int64 numstart = -1;
		int64 numend = -1;
		int number;
		for (char c : line)
		{
			bool newDigit = isdigit(c);
			if (newDigit)
			{
				if (!digit)
				{ // number starting
					numstart = x;
					number = c - '0';
				}
				else
				{
					number *= 10;
					number += c - '0';
				}
			}
			else if (digit)
			{
				numend = x - 1;
				coord pos;
				char symbol = findSymbol(map, numstart - 1, y - 1, numend + 1, y + 1, pos);
				if (symbol)
				{
					part1 += number;
				}
				if (symbol == '*')
				{
					if (gears.count(pos))
					{
						gears[pos].push_back(number);
					}
					else
					{
						std::vector<int> newvector;
						newvector.push_back(number);
						gears[pos] = newvector;
					}
				}
			}
			digit = newDigit;

			++x;
		}
		if (digit)
		{
			numend = x;
			coord pos;
			char symbol = findSymbol(map, numstart - 1, y - 1, numend + 1, y + 1, pos);
			if (symbol)
			{
				part1 += number;
			}
			if (symbol == '*')
			{
				if (gears.count(pos))
				{
					gears[pos].push_back(number);
				}
				else
				{
					std::vector<int> newvector;
					newvector.push_back(number);
					gears[pos] = newvector;
				}
			}
		}
	}

	for (const auto &gear : gears)
	{
		int count = gear.second.size();
		if (count == 2)
		{
			int product = gear.second[0] * gear.second[1];
			part2 += product;
		}
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 4361, 467835);
	Process("input.txt");

	return 0;
}
