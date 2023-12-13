// day12.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 12/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Gaps = std::vector<int64>;

std::string g_pattern;
std::vector<int64> g_numbers;

bool MatchPartialPattern(const Gaps& gaps)
{
	auto p_iter = g_pattern.begin();
	auto n_iter = g_numbers.begin();
	for (int64 gap : gaps)
	{
		while (gap)
		{
			char ch = *p_iter++;
			if (ch != '.' && ch != '?')
				return false;
			--gap;
		}
		if (n_iter != g_numbers.end())
		{
			int64 block = *n_iter++;
			while (block)
			{
				char ch = *p_iter++;
				if (ch != '#' && ch != '?')
					return false;
				--block;
			}
			if (n_iter != g_numbers.end())
			{
				char ch = *p_iter++;
				if (ch != '.' && ch != '?')
					return false;
			}
		}
	}
	// count the remaining blocks in the pattern to see whether they could match the remaining numbers
	int64 blocks = 0, unknown = 0;
	while (p_iter != g_pattern.end())
	{
		char ch = *p_iter++;
		if (ch == '#') ++blocks;
		else if (ch == '?') ++unknown;
	}
	int64 expecting = 0;
	while (n_iter != g_numbers.end())
	{
		expecting += *n_iter++;
	}
	if (expecting > (blocks + unknown))
		return false;
	if (expecting < blocks)
		return false;
	return true;
}

void CountPatterns(int64& numPatterns, Gaps g, int64 extraSpaces, int64 gaps)
{
	if (extraSpaces == 0)
	{
		while (gaps)
		{
			g.push_back(0);
			--gaps;
		}
		if (MatchPartialPattern(g))
			++numPatterns;
		return;
	}
	if (gaps == 1)
	{
		g.push_back(extraSpaces);
		if (MatchPartialPattern(g))
			++numPatterns;
		return;
	}
	if (!MatchPartialPattern(g))
		return;
	for (int64 thisGap = 0; thisGap <= extraSpaces; ++thisGap)
	{
		Gaps g2 = g;
		g2.push_back(thisGap);
		CountPatterns(numPatterns, g2, extraSpaces - thisGap, gaps - 1);
	}
}

int64 CalculateScore(const std::string& pattern, const std::vector<int64>& numbers, int64 size)
{
	// find all patterns that match the numbers
	int64 extraSpaces = pattern.length() - size; // the number of extra spaces that could be between banks of '#'
	int64 gaps = numbers.size() + 1; // the number of places that extra spaces can be placed: either side of or between the banks of '#'.

	// so, for a list of ints size (gaps), find all the ways that the extra spaces can be distributed.
	int64 numPatterns = 0;
	g_pattern = pattern;
	g_numbers = numbers;
	CountPatterns(numPatterns, Gaps(), extraSpaces, gaps);

	return numPatterns;
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
				// ????.######..#####. 1,6,5
				char* p = strtok(thisLine, " ");
				std::string pattern(p);
				std::vector<int64> numbers;
				char* number = strtok(nullptr, ",");
				int64 size = -1;
				while (number)
				{
					int64 num = atoll(number);
					numbers.push_back(num);
					number = strtok(nullptr, ",");
					size += num + 1;
				}
				int64 part1score = CalculateScore(pattern, numbers, size);
				part1 += part1score;

				// for part 2, initially brute force it
				std::string pattern2;
				std::vector<int64> numbers2;
				for (int i = 0; i < 5; ++i)
				{
					pattern2.append(pattern);
					if (i < 4)
						pattern2.append(1, '?');
					numbers2.insert(numbers2.end(), numbers.begin(), numbers.end());
				}
				int64 size2 = size * 5 + 4;;
				
				int64 part2score = CalculateScore(pattern2, numbers2, size2);
				part2 += part2score;
				printf("1: %lld 2: %lld\n", part1score, part2score);
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
	Process("example.txt", 21, 525152);
	Process("input.txt");

	return 0;
}
