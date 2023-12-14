// day12.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 12/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Ints = std::vector<int64>;
using BitInt = int128;

struct Pattern
{
	static std::array<std::array<BitInt, 128>, 2> bitpatterns;
	static void init()
	{
		BitInt full = 0;
		for (int i = 0; i < 128; ++i)
		{
			bitpatterns[0][i] = 0;
			bitpatterns[1][i] = full;
			full <<= 1;
			full |= 1;
		}
	}

	Pattern(std::string input = "")
	{
		for (char c : input)
		{
			known <<= 1;
			mask <<= 1;
			if (c == '#')
			{
				known |= 1;
			}
			if (c != '?')
			{
				mask |= 1;
			}
			++length;
		}
	}

	void PushBits(int knownbit, int maskbit, int64 bits)
	{
		assert(bits < bitpatterns[0].size());
		known <<= bits;
		mask <<= bits;
		known |= bitpatterns[knownbit][bits];
		mask |= bitpatterns[maskbit][bits];
		length += bits;
	}

	Pattern(const Ints& gaps, const Ints& numbers, int64 extraSpaces)
	{
		auto n_iter = numbers.begin();
		for (int64 gap : gaps)
		{
			PushBits(0, 1, gap);
			if (n_iter != numbers.end())
			{
				int64 block = *n_iter++;
				PushBits(1, 1, block);
				if (n_iter != numbers.end())
				{
					PushBits(0, 1, 1);
				}
			}
		}
		while (n_iter != numbers.end())
		{
			extraSpaces += *n_iter++;
			if (n_iter != numbers.end())
				++extraSpaces;
		}
		known <<= extraSpaces;
		mask <<= extraSpaces;
	}

	void Expand()
	{
		Pattern part = *this;
		for (int i = 0; i < 4; ++i)
		{
			known <<= 1 + length;
			mask <<= 1 + length;
			known |= part.known;
			mask |= part.mask;
		}
		length *= 5;
		length += 4;
	}

	bool Matches(const Pattern& other) const
	{
		BitInt combined_mask = mask & other.mask;
		return (known & combined_mask) == (other.known & combined_mask);
	}

	BitInt known = 0;
	BitInt mask = 0;
	int64 length = 0;
};

std::array<std::array<BitInt, 128>, 2> Pattern::bitpatterns;

Pattern g_pattern;
Ints g_numbers;
int64 g_numPatterns = 0;

bool MatchPattern(const Pattern& pattern)
{
	Pattern p = pattern;
	int64 extraSpaces = g_pattern.length - p.length;
	p.PushBits(0, 0, extraSpaces);
	return g_pattern.Matches(p);
}

void CountPatterns(Pattern& p, int64 nextNumber, int64 extraSpaces, int64 gaps)
{
	if (extraSpaces == 0)
	{
		while (nextNumber < g_numbers.size())
		{
			p.PushBits(1, 1, g_numbers[nextNumber++]);
			if (nextNumber < g_numbers.size())
				p.PushBits(0, 1, 1);
		}
		if (MatchPattern(p))
			++g_numPatterns;
		return;
	}
	if (gaps == 1)
	{
		p.PushBits(0, 1, extraSpaces);
		if (MatchPattern(p))
			++g_numPatterns;
		return;
	}
	if (!MatchPattern(p))
		return;
	int64 nextNumber2 = nextNumber + 1;
	for (int64 thisGap = 0; thisGap <= extraSpaces; ++thisGap)
	{
		Pattern p2 = p;

		p2.PushBits(0, 1, thisGap);
		p2.PushBits(1, 1, g_numbers[nextNumber]);
		if (nextNumber2 < g_numbers.size())
			p2.PushBits(0, 1, 1);

		CountPatterns(p2, nextNumber2, extraSpaces - thisGap, gaps - 1);
	}
}

int64 CalculateScore(const Pattern& pattern, const std::vector<int64>& numbers, int64 size)
{
	// find all patterns that match the numbers
	int64 extraSpaces = pattern.length - size; // the number of extra spaces that could be between banks of '#'
	int64 gaps = numbers.size() + 1; // the number of places that extra spaces can be placed: either side of or between the banks of '#'.

	// so, for a list of ints size (gaps), find all the ways that the extra spaces can be distributed.
	g_numPatterns = 0;
	g_pattern = pattern;
	g_numbers = numbers;
	Pattern p;
	CountPatterns(p, 0, extraSpaces, gaps);

	return g_numPatterns;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Pattern::init();
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	int64 maxLength = 0;
	int64 line = 1;
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
				std::string pat(p);
				Pattern pattern(pat);
				maxLength = max(pattern.length, maxLength);
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

				// for part 2, brute force it
				Pattern pattern2 = pattern;
				pattern2.Expand();
				std::vector<int64> numbers2;
				for (int i = 0; i < 5; ++i)
				{
					numbers2.insert(numbers2.end(), numbers.begin(), numbers.end());
				}
				int64 size2 = size * 5 + 4;;
				
				int64 part2score = CalculateScore(pattern2, numbers2, size2);
				part2 += part2score;
				printf("line %lld: p1: %lld p2: %lld\n", line++, part1score, part2score);
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
