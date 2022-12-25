// day25.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 25/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 FromSnafu(std::string input)
{
	int64 result = 0;
	for (char c : input)
	{
		result *= 5;
		if (isdigit(c))
		{
			result += c - '0';
		}
		else if (c == '-')
		{
			result -= 1;
		}
		else if (c == '=')
		{
			result -= 2;
		}
	}
	return result;
}

std::string ToSnafu(int64 num)
{
	// first, just a normal base 5 number. Then, do something with '3's and '4's
	std::string result = "";
	int64 x = num;
	while (x > 0)
	{
		int64 digit = x % 5;
		int64 carry = 0;
		char c;
		if (digit < 3)
		{
			c = digit + '0';
		}
		else
		{
			digit -= 5;
			carry = 1;
			c = (digit == -1) ? '-' : '=';
		}
		result = std::string(1, c) + result;
		x /= 5;
		x += carry;
	}
	return result;
}

void Process(const char* filename, std::string expectedPart1 = "")
{
	double start = GetMilliseconds();
	int64 total = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				total += FromSnafu(thisLine);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	std::string part1 = ToSnafu(total);
	assert(total == FromSnafu(part1));
	if (expectedPart1 != "")
	{
		printf("[%.2f] %s: Part 1 expected: %s\n", GetMilliseconds() - start, filename, expectedPart1.c_str());
	}
	printf("[%.2f] %s: Part 1: %s\n", GetMilliseconds() - start, filename, part1.c_str());
	assert(expectedPart1 == "" || expectedPart1 == part1);
}

int main()
{
	Process("example.txt", "2=-1=0");
	Process("input.txt");

	return 0;
}
