// day25.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 25/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct KeyOrLock
{
	std::vector<std::string> block;
	bool bKey;
	std::array<int, 5> teeth;

	void CountTeeth()
	{
		if (bKey)
		{
			std::reverse(block.begin(), block.end());
		}
		teeth.fill(-1);
		for (std::string line : block)
		{
			int index = 0;
			for (char col : line)
			{
				teeth[index++] += col == '#';
			}
		}
		if (bKey)
		{
			std::reverse(block.begin(), block.end());
		}
	}

	bool Fits(const KeyOrLock& other) const
	{
		for (int i = 0; i < 5; ++i)
		{
			if (teeth[i] + other.teeth[i] > 5)
				return false;
		}
		return true;
	}
};

using Blocks = std::vector<KeyOrLock>;

int64 CountFittingKeysAndLocks(const Blocks& keys, const Blocks& locks)
{
	int64 count = 0;
	for (const KeyOrLock& key : keys)
	{
		for (const KeyOrLock& lock : locks)
		{
			if (lock.Fits(key))
				++count;
		}
	}
	return count;
}

void Process(const char* filename, int64 expectedPart1 = -1)
{
	Blocks keys;
	Blocks locks;
	KeyOrLock block;

	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				assert(line.length() == 5);
				block.block.push_back(line);
			}
			else
			{
				block.bKey = block.block[0][0] == '.';
				block.CountTeeth();
				if (block.bKey)
					keys.push_back(block);
				else
					locks.push_back(block);

				block = KeyOrLock();
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = CountFittingKeysAndLocks(keys, locks);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);
}

int main()
{
	Process("example.txt", 3);
	Process("input.txt");

	return 0;
}
