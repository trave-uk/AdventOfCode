// day10.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 10/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

/*struct IndicatorLights
{
	int numBits;
	int64 bits;
};*/
using IndicatorLights = int64;
using ButtonSchematic = int64;
using ButtonSchematics = std::vector<ButtonSchematic>;
using Buttons2 = std::set<int64>;
using Buttons2s = std::vector<Buttons2>;
using Joltages = std::vector<int64>;

struct Machine
{
	IndicatorLights il = 0;
	ButtonSchematics bs;
	Buttons2s bs2;
	Joltages jr;
};
using Machines = std::vector<Machine>;
using Presses = std::vector<int64>;

// part 2: 
void TestPressesPart2(const Machine& machine, const Presses& maxPresses, Joltages counters, int64& bestSolution, int64 buttonIndex = 0, int64 pressesSoFar = 0)
{
	const int64 numCounters = counters.size();
	// if there is only one button left then see whether it can be used
	if (maxPresses.size() == buttonIndex + 1)
	{
		int64 presses = -1;
		for (int64 i = 0; i < numCounters; ++i)
		{
			if (counters[i] > machine.jr[i])
				return;
			if (machine.bs2[buttonIndex].count(i))
			{
				int64 requiredPresses = machine.jr[i] - counters[i];
				if (presses != -1 && presses != requiredPresses)
					return;
				presses = requiredPresses;
			}
			else if (counters[i] != machine.jr[i])
				return;
		}
		// here reached: need to press last button (presses) times
		int64 totalPresses = pressesSoFar + presses;
		if (totalPresses < bestSolution)
			bestSolution = totalPresses;
		return;
	}
	else
	{
		// create recursion objects
		for (int64 presses = 0; presses <= maxPresses[buttonIndex]; ++presses)
		{
			Joltages c2 = counters;
			bool invalid = false;
			for (int i = 0; i < numCounters; ++i)
			{
				if (machine.bs2[buttonIndex].count(i))
				{
					c2[i] += presses;
					if (c2[i] > machine.jr[i])
					{
						// Cannot press this many times, so can early out
						return;
					}
				}
			}
			TestPressesPart2(machine, maxPresses, c2, bestSolution, buttonIndex + 1, pressesSoFar + presses);
		}
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Machines machines;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			// [.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
			// [indicator lights] (button wiring schematics)+ {joltage requirements}
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				Machine m;
				char* saveptr1;
				char* token = strtok_s(thisLine, " ", &saveptr1);
				while (token)
				{
					switch (token[0])
					{
					case '[':
					{
						// [.##.]
						assert(m.il == 0);
						for (int i = strlen(token) - 2; i > 0; --i)
						{
							char light = token[i];
							m.il <<= 1;
							m.il |= (light == '#');
						}
						break;
					}
					case '(':
					{
						// (2,3)
						ButtonSchematic b = 0;
						Buttons2 b2;
						char* saveptr2;
						char* tok2 = strtok_s(token, "(,)", &saveptr2);
						while (tok2)
						{
							int bit = atoi(tok2);
							b |= (1ll << bit);
							b2.insert(bit);
							tok2 = strtok_s(nullptr, "(,)", &saveptr2);
						}
						m.bs.push_back(b);
						m.bs2.push_back(b2);
						break;
					}
					case '{':
					{
						// {3,5,4,7}
						assert(m.jr.size() == 0);
						char* saveptr2;
						char* tok2 = strtok_s(token, "{,}", &saveptr2);
						while (tok2)
						{
							int64 joltage = atoll(tok2);
							m.jr.push_back(joltage);
							tok2 = strtok_s(nullptr, "{,}", &saveptr2);
						}
						break;
					}
					default:
						assert(false);
					}
					token = strtok_s(nullptr, " ", &saveptr1);
				}
				machines.push_back(m);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (const Machine& m : machines)
	{
		// test all combinations of buttons
		int numCombinations = 1 << m.bs.size();
		int minCount = INT_MAX;
		for (int i = 0; i < numCombinations; ++i)
		{
			int64 test = 0;
			int pattern = i;
			int count = 0;
			int button = 0;
			while (pattern)
			{
				if (pattern & 1)
				{
					test ^= m.bs[button];
					++count;
				}
				pattern >>= 1;
				++button;
			}
			if (test == m.il)
			{
				if (count < minCount)
					minCount = count;
			}
		}
		assert(minCount != INT_MAX);
		part1 += minCount;
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	// For part 2, buttons can be pressed >1 times
	for (const Machine& m : machines)
	{
		int64 minCount = INT64_MAX;
		// find the max number of times each button _could_ be pressed to limit search
		// this will be the lowest required joltage level of the counters this button affects.
		Presses maxPresses;
		for (auto bs : m.bs2)
		{
			int64 minJoltage = INT64_MAX;
			for (auto b : bs)
			{
				if (m.jr[b] < minJoltage)
					minJoltage = m.jr[b];
			}
			assert(minJoltage != INT64_MAX);
			maxPresses.push_back(minJoltage);
		}
		// Now, each button can be pressed between 0 and maxPresses[button] times
		Joltages counters;
		for (auto j : m.jr)
		{
			counters.push_back(0);
		}
		TestPressesPart2(m, maxPresses, counters, minCount);

		assert(minCount != INT64_MAX);
		part2 += minCount;
	}

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 7, 33);
	Process("input.txt");

	return 0;
}
