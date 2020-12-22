// day09.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = int64;

int process(const char *filename, int preamble)
{
	std::vector<Value> numbers;
	FILE *fp = fopen(filename, "rt");
	char* buffer = new char[65536];
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				numbers.push_back(atoll(thisLine));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	std::vector<int> candidates;
	for (int i = 0; i < preamble; ++i)
	{
		for (int j = 0; j < preamble; ++j)
		{
			if (i == j)
				continue;
			candidates.push_back(numbers[i] + numbers[j]);
		}
	}

	Value part1 = -1;
	for (int i = preamble; i < numbers.size(); ++i)
	{
		if (i > preamble)
		{
			// now update candidates
			const int oldIndex = i - 1 - preamble;
			const Value old = numbers[oldIndex];
			const Value diff = numbers[i - 1] - old;
			for (int j = 1; j < preamble; ++j)
			{
				Value sum = old + numbers[oldIndex + j];
				auto x = std::find(candidates.begin(), candidates.end(), sum);
				assert(x != candidates.end());
				*x += diff;
			}
		}

		auto x = std::find(candidates.begin(), candidates.end(), numbers[i]);
		if (x == candidates.end())
		{
			part1 = numbers[i];
			printf("Part 1: %lld\n", part1);
			break;
		}
	}

	for (int i = 0; i < numbers.size(); ++i)
	{
		if (numbers[i] >= part1)
			continue;

		Value sum = 0;
		int j;
		for (j = i; j < numbers.size() && sum < part1; ++j)
		{
			sum += numbers[j];
		}
		if (sum == part1)
		{
			Value lowest = part1;
			Value highest = 0;
			for (int k = i; k < j; ++k)
			{
				if (numbers[k] > highest)
				{
					highest = numbers[k];
				}
				if (numbers[k] < lowest)
				{
					lowest = numbers[k];
				}
			}
			Value part2 = highest + lowest;
			printf("Part 2: %lld\n", part2);
		}
	}

	return -1;
}

int main()
{
//	process("example.txt", 5);
	process("input.txt", 25);

	return 0;
}
