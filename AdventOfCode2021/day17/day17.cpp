// day17.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 17/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Step(coord& pos, coord&vel)
{
	pos += vel;
	vel.first -= sgn(vel.first);
	vel.second -= 1;
}

bool Simulate(const coord &minTarget, const coord& maxTarget, int64 dx, int64 dy, int64& maxY)
{
	maxY = 0;
	// Keep going until y < minTarget.y or x > maxTarget.x
	coord pos(0, 0);
	coord vel(dx, dy);
	while (pos.second >= minTarget.second && pos.first <= maxTarget.first)
	{
		Step(pos, vel);
		if (pos.second > maxY)
			maxY = pos.second;
		if (pos.isContained(minTarget, maxTarget))
			return true;
	}
	return false;
}

void Process(const char* filename)
{
	coord minTarget;
	coord maxTarget;
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
				//              tx1..tx2, ty1..ty2
				// target area: x=20..30, y=-10..-5
				assert(minTarget.length() == 0 && maxTarget.length() == 0);
				int matches = sscanf(thisLine, "target area: x=%lld..%lld, y=%lld..%lld", &minTarget.first, &maxTarget.first, &minTarget.second, &maxTarget.second);
				assert(matches == 4);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 maxY = 0;
	int64 found = 0;
	// In order to reach the target area, the minimum dx should match dx*dx/2 >= tx1
	// So: min dx = sqrtf(2*tx)
	int64 min_dx = int64(floor(sqrt(2.0 * minTarget.first)));
	// The maximum value for dx is the far position. This won't be the answer but gives us a bound
	int64 max_dx = maxTarget.first;
	for (int64 dx = min_dx; dx <= max_dx; ++dx)
	{
		// Using -dx as minimum dy, but actually if the target were far far down, min_dy could be much lower.
		// On the other hand, 1000 seems like its much too high for most targets. However, these arbitrarily-chosen bounds work fine
		for (int64 dy = -dx; dy < 1000; ++dy)
		{
			int64 thisMaxY;
			if (Simulate(minTarget, maxTarget, dx, dy, thisMaxY))
			{
				++found;
				if (thisMaxY > maxY)
					maxY = thisMaxY;
			}
		}
	}

	printf("%s: Part 1: %lld\n", filename, maxY);
	printf("%s: Part 2: %lld\n", filename, found);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
