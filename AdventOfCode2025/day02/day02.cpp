// day02.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 02/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Range
{
	int64 start;
	int64 end;
};

using Ranges = std::vector<Range>;

bool invalidId(int64 value, bool part2 = false)
{
	std::string str_value = std::format("{0}", value);
	int64 maxRepeats = part2 ? str_value.length() : 2;
	for (int64 repeats = 2; repeats <= maxRepeats; ++repeats)
	{
		if ((str_value.length() % repeats) != 0)
			continue;
		bool valid = false;
		int64 halfLength = str_value.length() / repeats;
		std::string left = str_value.substr(0, halfLength);
		for (int i = 1; i < repeats; ++i)
		{
			std::string right = str_value.substr(halfLength * i, halfLength);
			if (left != right)
			{
				valid = true;
				break;
			}
		}
		if (!valid)
		{
			return true;
		}
	}
	return false;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Ranges ranges;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				auto x = strtok(thisLine, ",");
				Range r;
				while (x)
				{
					int count = sscanf(x, "%lld-%lld", &r.start, &r.end);
					assert(count == 2);
					ranges.push_back(r);
					x = strtok(nullptr, ",");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (const Range& range : ranges)
	{
		for (int64 value = range.start; value <= range.end; ++value)
		{
//			if (invalidId(value))
//				part1 += value;
			if (invalidId(value, true))
				part2 += value;
		}
	}

//	assert(expectedPart1 == -1 || expectedPart1 == part1);
//	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 1227775554, 4174379265);
	Process("input.txt");

	return 0;
}
