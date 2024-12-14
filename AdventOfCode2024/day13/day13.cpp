// day13.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 13/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct ClawMachine
{
	coord buttonA;
	coord buttonB;
	coord prize;

	int64 CalculateCost2() const
	{
		// Non-iterative method that solves the simultaneous equations using maths
		const int64 px = prize.first;
		const int64 py = prize.second;
		const int64 ax = buttonA.first;
		const int64 ay = buttonA.second;
		const int64 bx = buttonB.first;
		const int64 by = buttonB.second;

		// Find a and b such that:
		// X: (a * ax) + (b * bx) == px
		// Y: (a * ay) + (b * by) == py
		// 
		// X -> a == (px - (b * bx)) / ax
		// Y -> ay * (px - (b * bx)) / ax + (b * by) == py
		// -> (px - (b * bx)) * ay + (b * by) * ax == py * ax
		// -> (px * ay) - (b * bx * ay) + (b * by * ax) == (py * ax)
		// -> (b * by * ax) - (b * bx * ay) == (py * ax) - (px * ay) 
		// -> b * (by * ax - bx * ay) == (py * ax) - (px * ay)
		// -> b = (py * ax - px * ay) / (by * ax - bx * ay)
		int64 result = 0;
		int64 bTop = py * ax - px * ay;
		assert(bTop);
		int64 bBottom = by * ax - bx * ay;
		if ((bTop % bBottom) == 0)
		{
			int64 b = bTop / bBottom;
			int64 a = (px - (b * bx));
			if ((a % ax) == 0)
			{
				a /= ax;
				result = 3 * a + b;
			}
		}

		// ... -> a = (py * bx - px * by) / (ay * bx - ax * by)
		int64 result2 = 0;
		int64 aTop = py * bx - px * by;
		assert(aTop);
		int64 aBottom = ay * bx - ax * by;
		if ((aTop % aBottom) == 0)
		{
			int64 a = aTop / aBottom;
			int64 b = (px - (a * ax));
			if ((b % bx) == 0)
			{
				b /= bx;
				result2 = 3 * a + b;
			}
		}
		assert(result == result2);
		return result;
	}

	int64 CalculateCost() const
	{
		// iterative method that is impractical for astronomical prize locations
		int64 minCost = INT64_MAX;
		// Solve a*buttonA + b*buttonB == prize
		int64 maxA = min(prize.first / buttonA.first, prize.second / buttonA.second);
		//int64 maxB = min(prize.first / buttonB.first, prize.second / buttonB.second);
		for (int64 a = 0; a <= maxA; ++a)
		{
			coord A = buttonA;
			A *= a;
			coord B = prize;
			B -= A;
			coord modTest = B;
			modTest %= buttonB;
			coord modDiv = B;
			modDiv /= buttonB;
			if (modTest.first == 0 && modTest.second == 0 && modDiv.first == modDiv.second)
			{
				int64 b = modDiv.first;
				int cost = 3 * a + b;
				if (cost < minCost)
					minCost = cost;
			}
		}
		if (minCost == INT64_MAX)
			return 0;
		else
			return minCost;
	}
};

using ClawMachines = std::vector<ClawMachine>;

void Process(const char* filename, int64 expectedPart1 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");

	ClawMachines machines;
	ClawMachine m;
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
				// Button A: X+94, Y+34
				// Button B: X+22, Y+67
				// Prize: X=8400, Y=5400
				assert(line.length() > 7);
				switch (line[7])
				{
				case 'A':
				{
					int count = sscanf_s(thisLine, "Button A: X+%lld, Y+%lld", &m.buttonA.first, &m.buttonA.second);
					assert(count == 2);
					break;
				}
				case 'B':
				{
					int count = sscanf_s(thisLine, "Button B: X+%lld, Y+%lld", &m.buttonB.first, &m.buttonB.second);
					assert(count == 2);
					break;
				}
				case 'X':
				{
					int count = sscanf_s(thisLine, "Prize: X=%lld, Y=%lld", &m.prize.first, &m.prize.second);
					assert(count == 2);
					machines.push_back(m);
					break;
				}
				default:
					assert(false);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = 0;
	int64 part1a = 0;
	for (const ClawMachine& machine : machines)
	{
		part1 += machine.CalculateCost();
		part1a += machine.CalculateCost2();
		assert(part1 == part1a);
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	coord part2offset(10000000000000l, 10000000000000l);
	int64 part2 = 0;
	for (ClawMachine& machine : machines)
	{
		machine.prize += part2offset;
		part2 += machine.CalculateCost2();
	}
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 480);
	Process("input.txt");

	return 0;
}
