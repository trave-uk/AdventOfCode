// day09.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 09/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void ReadInts(char* thisLine, std::vector<int64>& output)
{
	char* tok = strtok(thisLine, " ");
	while (tok)
	{
		output.push_back(atoll(tok));
		tok = strtok(nullptr, " ");
	}
}

std::pair<int64, int64> Predict(std::vector<int64>& history)
{
	std::vector<int64> diffs;
	int64 length = history.size();
	bool allZero = true;
	for (int64 i = 1; i < length; ++i)
	{
		int64 diff = history[i] - history[i - 1];
		diffs.push_back(diff);
		if (diff)
			allZero = false;
	}
	int64 newDiffFront = 0;
	int64 newDiffBack = 0;
	if (!allZero)
	{
		auto x = Predict(diffs);
		newDiffFront = x.first;
		newDiffBack = x.second;
	}
	int64 first = history.front() - newDiffFront;
	int64 last = history.back() + newDiffBack;
	return std::make_pair(first, last);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
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
				std::vector<int64> history;
				ReadInts(thisLine, history);
				auto x = Predict(history);
				part1 += x.second;
				part2 += x.first;
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
	Process("example.txt", 114, 2);
	Process("input.txt");

	return 0;
}
