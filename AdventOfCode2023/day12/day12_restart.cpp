// day18.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 18/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using key2 = std::tuple<std::string, int64, bool>;
std::map<key2, bool> cache2;

bool Matches(std::string pat, int64 number, bool lastNumber)
{
	key2 k = std::make_tuple(pat, number, lastNumber);
	if (cache2.count(k))
		return cache2[k];

	for (char c : pat)
	{
		if (!number)
		{
			if (c == '#')
			{
				cache2[k] = false;
				return false;
			}
			else if (!lastNumber)
			{
				cache2[k] = true;
				return true;
			}
		}
		else
		{
			if (c == '.')
			{
				cache2[k] = false;
				return false;
			}
			--number;
		}
	}
	bool result = (!number && lastNumber);

	cache2[k] = result;
	return result;
}

using key = std::pair<std::string, std::vector<int64>>;
std::map<key, int64> cache;

int64 CalculateScore(std::string pat, std::vector<int64> numbers)
{
	key k = std::make_pair(pat, numbers);
	if (cache.count(k))
		return cache[k];

	int64 result = 0;
	int64 number = numbers.front();
	numbers.erase(numbers.begin());
	bool lastNumber = numbers.empty();

	// Find places that the first number of # could happen. For each one, recurse to find places the next number could go
	while (pat.length())
	{
		while (pat[0] == '.')
		{
			pat = pat.substr(1);
		}
		if (Matches(pat, number, lastNumber))
		{
			if (lastNumber)
			{
				++result;
			}
			else
			{
				std::string pat2 = pat.substr(number+1);
				result += CalculateScore(pat2, numbers);
			}
		}
		if (pat[0] == '?')
		{
			pat = pat.substr(1);
		}
		else if (pat[0] == '#')
		{
			break;
		}
	}

	cache[k] = result;
	return result;
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
				std::string pat(p);
				std::vector<int64> numbers;
				char* number = strtok(nullptr, ",");
				while (number)
				{
					int64 num = atoll(number);
					numbers.push_back(num);
					number = strtok(nullptr, ",");
				}
				int64 part1score = CalculateScore(pat, numbers);
				part1 += part1score;

				std::string pat2 = pat;
				std::vector<int64> numbers2 = numbers;
				for (int i = 0; i < 4; ++i)
				{
					pat2.append("?", 1);
					pat2.append(pat);
					numbers2.insert(numbers2.end(), numbers.begin(), numbers.end());
				}
				int64 part2score = CalculateScore(pat2, numbers2);
				part2 += part2score;
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
	Process("example2.txt", 10, 506250);
	Process("input632.txt", 177);

	Process("example.txt", 21, 525152);
	Process("input.txt");

	return 0;
}
