// day05.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

bool isNice(std::string name)
{
	char last = 0;
	int vowels = 0;
	bool dupe = false;
	for (char c : name)
	{
		// It contains at least three vowels(aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
		if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
		{
			++vowels;
		}

		// It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
		if (c == last)
		{
			dupe = true;
		}

		// It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.
		std::string pair;
		pair.append(1, last);
		pair.append(1, c);
		if (pair == "ab" || pair == "cd" || pair == "pq" || pair == "xy")
		{
			return false;
		}

		last = c;
	}
	return dupe && vowels >= 3;
}

bool isNice2(std::string name)
{
	char last1 = 0;
	char last2 = 0;
	bool dupe = false;
	bool neighbours = false;
	std::map<std::string, int> pairs;
	int index = 0;
	for (char c : name)
	{
		// It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
		if (last1)
		{
			std::string pair;
			pair.append(1, last1);
			pair.append(1, c);
			if (pairs.count(pair) > 0)
			{
				if (index - pairs[pair] >= 2)
				{
					dupe = true;
				}
			}
			else
			{
				pairs[pair] = index;
			}
		}

		// It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.
		if (last2 == c)
		{
			neighbours = true;
		}

		last2 = last1;
		last1 = c;
		++index;
	}
	return dupe && neighbours;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
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
				if (isNice(thisLine))
				{
					++part1;
				}
				if (isNice2(thisLine))
				{
					++part2;
				}
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
	Process("example1.txt", 2);
	Process("example2.txt", -1, 2);
	Process("input.txt");

	return 0;
}
