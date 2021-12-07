// day07.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 7/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	std::vector<int64> numbers;
	int result = 0;
	int64 low = INT64_MAX;
	int64 high = 0;
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
				char* num = strtok(thisLine, ",");
				while (num)
				{
					int64 n = atoi(num);
					numbers.push_back(n);
					if (n < low)
						low = n;
					if (n > high)
						high = n;
					num = strtok(nullptr, ",");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 lowestTotal = INT64_MAX;
	for (int64 n = low; n <= high; ++n)
	{
		int64 total = 0;
		for (int64 m : numbers)
		{
			total += abs(n - m);
		}
		if (total < lowestTotal)
		{
			lowestTotal = total;
		}
	}
	printf("%s: Part 1: %lld\n", filename, lowestTotal);

	lowestTotal = INT64_MAX;
	for (int64 n = low; n <= high; ++n)
	{
		int64 total = 0;
		for (int64 m : numbers)
		{
			int64 top = abs(n - m);
			while (top)
			{
				total += top;
				--top;
			}
		}
		if (total < lowestTotal)
		{
			lowestTotal = total;
		}
	}
	printf("%s: Part 2: %lld\n", filename, lowestTotal);

}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
