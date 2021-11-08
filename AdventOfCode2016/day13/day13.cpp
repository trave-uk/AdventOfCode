// day13.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"


using coord = std::pair<int64, int64>;

using maze = std::map<coord, int64>;

using test = std::pair<coord, int64>;

using tests = std::queue<test>;

maze distances;
tests Tests;

int64 input = 10;
coord target;

bool IsWall(coord& pos)
{
	int64 x = pos.first;
	int64 y = pos.second;
	if (x < 0 || y < 0)
		return true;

	int64 value = x * x + 3 * x + 2 * x*y + y + y * y + input;

	bool bIsWall = false;
	while (value != 0)
	{
		if (value % 2)
		{
			bIsWall = !bIsWall;
		}
		value /= 2;
	}
	return bIsWall;
}

void process(bool bPart2)
{
	test T = Tests.front();
	Tests.pop();

	coord pos = T.first;
	int64 distance = T.second;

	distances.insert_or_assign(pos, distance);

	if (bPart2 && distance == 50)
		return;

	for (int i = 0; i < 4; ++i)
	{
		coord pos2;
		if (i < 2)
		{
			pos2 = std::make_pair(pos.first, pos.second + i * 2 - 1);
		}
		else
		{
			pos2 = std::make_pair(pos.first + (i - 2) * 2 - 1, pos.second);
		}
		if (IsWall(pos2))
			continue;
		auto found = distances.find(pos2);
		if (found != distances.end() && found->second <= distance)
			continue;

		test T = std::make_pair(pos2, distance + 1);
		Tests.push(T);
	}
}

int64 flood(coord pos, int64 distance, bool bPart2 = false)
{
	distances.clear();
	while (!Tests.empty()) Tests.pop();

	test T = std::make_pair(pos, distance);
	Tests.push(T);

	if (!bPart2)
	{
		while (distances.find(target) == distances.end())
		{
			process(bPart2);
		}
		auto found = distances.find(target);
		return found->second;
	}
	else
	{
		while (Tests.size() > 0)
		{
			process(bPart2);
		}
		return distances.size();
	}
}

int main()
{
	input = 10;
	coord pos = std::make_pair(1, 1);
	target = std::make_pair(7, 4);
	int64 distance = flood(pos, 0);
	printf("Example: %lld\n", distance);

	input = 1362;
	pos = std::make_pair(1, 1);
	target = std::make_pair(31, 39);
	distance = flood(pos, 0);
	printf("Part 1: %lld\n", distance);

	int64 count = flood(pos, 0, true);
	printf("Part 2: %lld\n", count);
	return 0;
}
