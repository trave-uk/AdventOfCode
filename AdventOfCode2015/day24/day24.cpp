// day24.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 08/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Presents = std::set<int64>;
int64 QE(const Presents& p)
{
	int64 result = 1;
	for (int64 i : p)
	{
		result *= i;
	}
	return result;
}

int64 Weight(const Presents& p)
{
	int64 result = 0;
	for (int64 i : p)
	{
		result += i;
	}
	return result;
}
Presents operator-(const Presents& l, const Presents& r)
{
	Presents result = l;
	for (int64 p : r)
	{
		result.erase(p);
	}
	return result;
}

Presents operator+(const Presents& l, int64 r)
{
	Presents result = l;
	result.insert(r);
	return result;
}

std::set<Presents> Groups(const Presents& p, int64 group)
{
	std::set<Presents> found;

	std::stack<Presents> stack;
	stack.push(Presents());
	while (!stack.empty())
	{
		Presents q = stack.top();
		stack.pop();
		Presents r = p - q;
		int64 high = (q.size() > 0) ? *q.rbegin() : -1;
		for (int64 s : r)
		{
			if (s < high)
				continue;
			Presents t = q + s;
			int64 w = Weight(t);
			if (w < group)
			{
				stack.push(t);
			}
			else if (w == group)
			{
				found.insert(t);
			}
		}
	}
	return found;
}

int64 Split(const Presents& p, int64 group)
{
	int64 smallest = INT64_MAX;
	int64 result = INT64_MAX;
	std::set<Presents> g = Groups(p, group);
	for (Presents q : g)
	{
		int64 size = q.size();
		if (size < smallest)
		{
			smallest = size;
			result = QE(q);
		}
		else if (size == smallest)
		{
			int64 qe = QE(q);
			if (qe < result)
			{
				result = qe;
			}
		}
	}
	return result;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	Presents presents;
	int64 totalweight = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				assert(isdigit(*thisLine));
				int64 weight = atoi(thisLine);
				presents.insert(weight);
				totalweight += weight;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 groupweight = totalweight / 3;
	assert(totalweight == groupweight * 3);

	part1 = Split(presents, groupweight);
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	groupweight = totalweight / 4;
	assert(totalweight == groupweight * 4);

	part2 = Split(presents, groupweight);
	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 99, 44);
	Process("input.txt");

	return 0;
}
