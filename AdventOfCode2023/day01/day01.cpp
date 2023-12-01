// day01.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 01/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::vector<std::string> digits = { "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
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
				int pos = 0;
				std::string line(thisLine);
				int first = -1, last = -1;
				int first2 = -1, last2 = -1;
				int index = 0;
				for (char c : line)
				{
					int digit = -1;
					if (isdigit(c))
					{
						digit = c - '0';
						if (first == -1)
							first = digit;
						last = digit;
					}
					for (int digit2 = 0; digit2 < 10; ++digit2)
					{
						std::string d = digits[digit2];
						std::string test = line.substr(index, d.length());
						if (test == d)
							digit = digit2;
					}
					if (digit != -1)
					{
						if (first2 == -1)
							first2 = digit;
						last2 = digit;
					}
					++index;
				}
				int calibration = first * 10 + last;
				part1 += calibration;
				int calibration2 = first2 * 10 + last2;
				part2 += calibration2;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 142);
	Process("example2.txt", -1, 281);
	Process("input.txt");

	return 0;
}
