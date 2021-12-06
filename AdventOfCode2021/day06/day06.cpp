// day06.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 6/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void tick2(std::array<int64, 9>& fishBuckets)
{
	std::array<int64, 9> fishBuckets2;
	fishBuckets2.fill(0);
	for (int i = 8; i >= 0; --i)
	{
		int j = (i > 0) ? (i - 1) : 6;
		fishBuckets2[j] += fishBuckets[i];
	}
	fishBuckets2[8] = fishBuckets[0];
	fishBuckets = fishBuckets2;
}

// This works, but has enormous memory requirements for billions of fish.
void tick(std::vector<int>& fish)
{
	int newFish = 0;
	for (int& f : fish)
	{
		if (!f)
		{
			f = 6;
			++newFish;
		}
		else
		{
			--f;
		}
	}
	while (newFish)
	{
		fish.push_back(8);
		--newFish;
	}
}

void Process(const char* filename, int days)
{
	std::vector<int> fish;
	std::array<int64, 9> fishBuckets;
	fishBuckets.fill(0);
	int result = 0;
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
				char* f = strtok(thisLine, ",");
				while (f)
				{
					int iFish = atoi(f);
 					fish.push_back(iFish);
					fishBuckets[iFish]++;
					f = strtok(nullptr, ",");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int d = 0; d < days; ++d)
	{
		if (days < 100)
		{
			// Part 1 implementation (inefficient memory use)
			tick(fish);
		}
		// Part 2 implementation (efficient memory use, and also works for part 1)
		tick2(fishBuckets);
	}
	int64 total = 0;
	for (int64 f : fishBuckets)
	{
		total += f;
	}
	if (days < 100)
	{
		assert(fish.size() == total);
	}
	printf("%s: %d days: %llu\n", filename, days, total);

}

int main()
{
	Process("example.txt", 18);
	Process("example.txt", 80);
	Process("input.txt", 80);
	Process("example.txt", 256);
	Process("input.txt", 256);

	return 0;
}
