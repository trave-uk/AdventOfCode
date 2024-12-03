// day01.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 03/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using NumberList = std::vector<int>;

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	NumberList left, right;
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
				int leftNumber, rightNumber;
				int count = sscanf_s(thisLine, "%d %d", &leftNumber, &rightNumber);
				assert(count == 2);
				left.push_back(leftNumber);
				right.push_back(rightNumber);
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	assert(left.size() == right.size());
	std::sort(left.begin(), left.end());
	std::sort(right.begin(), right.end());
	auto l = left.begin();
	auto r = right.begin();
	while (l != left.end())
	{
		part1 += abs(*l - *r);
		++l;
		++r;
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	for (auto it = left.begin(); it != left.end(); ++it)
	{
		int number = *it;
		int count = 0;
		for (auto it_r = right.begin(); it_r != right.end(); ++it_r)
		{
			if (*it_r == number)
				++count;
		}
		part2 += number * count;
	}
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 11, 31);
	Process("input.txt");

	return 0;
}
