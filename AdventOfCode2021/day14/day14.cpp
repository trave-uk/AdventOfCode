// day14.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 14/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::array<int64, 26> initialCounts;
std::array<int64, 26> counts;
std::map<std::string, char> rules;

// For part 1, this brute force approach worked, but need a different approach for part 2
void ApplyRulesToPolymer(std::list<char>& polymer)
{
	for (auto it = polymer.begin(); it != polymer.end();)
	{
		auto it2 = it;
		++it2;
		if (it2 != polymer.end())
		{
			std::string pair;
			pair.append(1, *it);
			pair.append(1, *it2);
			assert(rules.count(pair) != 0);
			char insert = rules[pair];
			assert(isalpha(insert));
			polymer.insert(it2, insert);

			++counts[insert - 'A'];
		}
		it = it2;
	}
}

// for part 2, keep a list of counts of each type of pair. The order of the pairs doesn't matter
// then, each iteration, generate a new list of counts.
std::map<std::string, int64> pair_counts;

void ApplyRulesToPairs()
{
	std::map<std::string, int64> new_pair_counts;
	for (auto it : pair_counts)
	{
		const std::string& pair = it.first;
		int64 count = it.second;
		assert(rules.count(pair) != 0);

		char insert = rules[pair];
		std::string pair1 = pair;
		pair1[1] = insert;
		new_pair_counts[pair1] += count;
		std::string pair2 = it.first;
		pair2[0] = insert;
		new_pair_counts[pair2] += count;

		counts[insert- 'A'] += count;
	}
	pair_counts = new_pair_counts;
}

int64 GetResult()
{
	int64 min = INT64_MAX;
	int64 max = 0;
	for (int64 count : counts)
	{
		if (!count)
			continue;
		if (count < min)
			min = count;
		if (count > max)
			max = count;
	}
	return max - min;
}

void Process(const char* filename)
{
	std::string start;

	initialCounts.fill(0);
	pair_counts.clear();
	rules.clear();
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
				if (start.length() == 0)
				{
					start = std::string(thisLine);

					// (part 2) Create pairs from start
					char lastc = 0;
					for (char c : start)
					{
						++initialCounts[c - 'A'];
						if (lastc)
						{
							std::string pair;
							pair.append(1, lastc);
							pair.append(1, c);
							pair_counts[pair]++;
						}
						lastc = c;
					}
				}
				else
				{
					char from[3];
					char to;
					int matches = sscanf(thisLine, "%s -> %c", from, &to);
					assert(matches == 2);
					rules.insert(std::make_pair(std::string(from), to));
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// Use a list, to save on reallocation in part 2 (probably)
	std::list<char> polymer;
	counts = initialCounts;
	for (char c : start)
	{
		polymer.push_back(c);
	}
	for (int step = 1; step <= 10; ++step)
	{
		ApplyRulesToPolymer(polymer);
	}
	int64 part1 = GetResult();
	printf("%s: Part 1: %lld\n", filename, part1);

	// Start again for part 2, using the more efficient algorithm (not actually using a list)
	counts = initialCounts;
	for (int step = 1; step <= 40; ++step)
	{
		ApplyRulesToPairs();
		if (step == 10)
		{
			assert(part1 == GetResult());
		}
	}
	printf("%s: Part 2: %lld\n", filename, GetResult());
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
