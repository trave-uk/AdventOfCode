// day05.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 05/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct FreshRange
{
	int64 start;
	int64 end;
};

using FreshRanges = std::vector<FreshRange>;

bool IngredientIsFresh(int64 ingredient, const FreshRanges& fresh)
{
	for (const FreshRange& f : fresh)
	{
		if (ingredient >= f.start && ingredient <= f.end)
			return true;
	}
	return false;
}

int64 CountFreshIngredients(const FreshRanges& fresh)
{
	// sort ranges by start
	FreshRanges sortedFresh = fresh;
	std::sort(sortedFresh.begin(), sortedFresh.end(), [](const FreshRange& a, const FreshRange& b) { return a.start < b.start; });
	int64 highest = 1;
	int64 total = 0;
	for (const FreshRange& f : sortedFresh)
	{
		int64 start = max(highest, f.start);
		if (f.end >= start)
			total += f.end + 1 - start;
		if (highest < f.end + 1)
			highest = f.end + 1;
	}
	return total;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	bool available = false;
	FreshRanges fresh;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (!available)
				{
					FreshRange f;
					int count = sscanf(thisLine, "%lld-%lld", &f.start, &f.end);
					assert(count == 2);
					fresh.push_back(f);
				}
				else
				{
					int64 ingredient = atoll(thisLine);
					if (IngredientIsFresh(ingredient, fresh))
					{
						++part1;
					}
				}
			}
			else
			{
				available = true;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part2 = CountFreshIngredients(fresh);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 3, 14);
	Process("input.txt");

	return 0;
}
