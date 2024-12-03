// day02.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 03/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

bool safe(std::vector<int> numbers)
{
	int last = INT_MAX;
	int lastdiff = INT_MAX;
	for (int n : numbers)
	{
		if (last == INT_MAX)
		{
			last = n;
			continue;
		}
		int diff = n - last;
		if (diff == 0 || abs(diff) > 3)
			return false;
		if (lastdiff != INT_MAX && sgn(diff) != sgn(lastdiff))
			return false;
		lastdiff = diff;
		last = n;
	}
	return true;
}

bool safe2(std::vector<int> numbers)
{
	if (safe(numbers))
		return true;
	for (int i = 0; i < numbers.size(); ++i)
	{
		std::vector<int> numbers2(numbers.begin(), numbers.begin() + i);
		std::vector<int> numbers3(numbers.begin() + i + 1, numbers.end());
		numbers2.insert(numbers2.end(), numbers3.begin(), numbers3.end());
		if (safe(numbers2))
			return true;
	}
	return false;
}

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
				std::vector<int> numbers;
				char* tok = strtok(thisLine, " ");
				while (tok)
				{
					numbers.push_back(atoi(tok));
					tok = strtok(nullptr, " ");
				}
				if (safe(numbers))
					++part1;
				if (safe2(numbers))
					++part2;
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
	Process("example.txt", 2, 4);
	Process("input.txt");

	return 0;
}
