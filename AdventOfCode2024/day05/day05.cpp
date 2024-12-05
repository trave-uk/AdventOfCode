// day05.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 05/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Rule = coord;
using Rules = std::vector<Rule>;
using Update = std::vector<int>;

size_t getIndexOf(int64 value, const Update& u)
{
	auto it = std::find(u.begin(), u.end(), value);
	if (it == u.end())
		return -1;
	return it - u.begin();
}

int64 CheckUpdate(const Update& u, const Rules& rules)
{
	for (const Rule& r : rules)
	{
		size_t left = getIndexOf(r.first, u);
		size_t right = getIndexOf(r.second, u);
		if (left == -1 || right == -1) continue;
		if (right < left) return 0;
	}
	return u[u.size() / 2];
}

int64 FixUpdate(Update& u, const Rules& rules)
{
	bool repeat = true;
	while (repeat)
	{
		repeat = false;
		for (const Rule& r : rules)
		{
			size_t left = getIndexOf(r.first, u);
			size_t right = getIndexOf(r.second, u);
			if (left == -1 || right == -1) continue;
			if (right < left)
			{
				int temp = u[left];
				u[left] = u[right];
				u[right] = temp;
				repeat = true;
				break;
			}
		}
	}

	return CheckUpdate(u, rules);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Rules rules;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (thisLine[2] == '|')
				{
					Rule r;
					int count = sscanf_s(thisLine, "%lld|%lld", &r.first, &r.second);
					assert(count == 2);
					rules.push_back(r);
				}
				else
				{
					Update u;
					char* tok = strtok(thisLine, ",");
					while (tok)
					{
						u.push_back(atoi(tok));
						tok = strtok(nullptr, ",");
					}
					int64 result = CheckUpdate(u, rules);
					if (result)
						part1 += result;
					else
						part2 += FixUpdate(u, rules);
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
	Process("example.txt", 143, 123);
	Process("input.txt");

	return 0;
}
