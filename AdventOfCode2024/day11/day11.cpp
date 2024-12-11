// day11.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 11/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"
using Stones = std::list<int64>;

void Report(const Stones& stones)
{
	printf("%lld:", stones.size());
	for (int64 n : stones)
	{
		printf(" %lld", n);
	}
	_putch('\n');
	_putch('\n');
}

using StonesDict = std::map<int64, Stones>;

StonesDict knowledge;

int64 Blink(Stones& s, int64 blinks)
{
	for (int64 i = 0; i < blinks; ++i)
	{
		for (Stones::iterator it = s.begin(); it != s.end(); ++it)
		{
			int64 stone = *it;
			if (knowledge.count(stone) != 0)
			{
				Stones t = knowledge[stone];
				*it = t.back();
				t.pop_back();
				s.insert(it, t.begin(), t.end());
				continue;
			}
			if (stone == 0)
			{
				*it = 1;
				Stones x;
				x.push_back(1);
				knowledge[0] = x;
			}
			else
			{
				char n[256];
				sprintf_s(n, "%lld", stone);
				int len = strlen(n);
				if ((len % 2) == 0)
				{
					int64 rhs = atoll(n + len / 2);
					n[len / 2] = '\0';
					int64 lhs = atoll(n);
					s.insert(it, lhs);
					*it = rhs;
					Stones x;
					x.push_back(lhs);
					x.push_back(rhs);
					knowledge[stone] = x;
				}
				else
				{
					Stones x;
					x.push_back(stone * 2024);
					knowledge[stone] = x;
					*it *= 2024;
				}
			}
		}
	}
	return s.size();
}

StonesDict knowledge25;

int64 BlinkBulk(const Stones& stones, int64 blinks)
{
	assert(blinks == 75);
	int64 total = 0;
	for (int64 n : stones)
	{
		if (knowledge25.count(n) == 0)
		{
			Stones x;
			x.push_back(n);
			Blink(x, 25);
			knowledge25[n] = x;
		}
		for (int64 m : knowledge25[n])
		{
			if (knowledge25.count(m) == 0)
			{
				Stones y;
				y.push_back(m);
				Blink(y, 25);
				knowledge25[m] = y;
			}
			for (int64 l : knowledge25[m])
			{
				if (knowledge25.count(l) == 0)
				{
					Stones z;
					z.push_back(l);
					Blink(z, 25);
					knowledge25[l] = z;
				}
				total += knowledge25[l].size();
			}
		}
	}
	return total;
}

void Process(const char* filename, int64 expectedPart1 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Stones stones;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				std::string line(thisLine);
				char* tok = strtok(thisLine, " ");
				while (tok)
				{
					stones.push_back(atoll(tok));
					tok = strtok(nullptr, " ");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	Stones copy = stones;
	part1 = Blink(copy, 25);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	// No answer given for the example data for part 2, so only do part 2 on the actual input data
	if (expectedPart1 == -1)
	{
		part2 = BlinkBulk(stones, 75);

		printf("%s: Part 2: %lld\n", filename, part2);
	}
}

int main()
{
	Process("example.txt", 55312);
	Process("input.txt");

	return 0;
}
