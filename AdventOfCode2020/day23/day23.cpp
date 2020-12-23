// day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Circle = std::map<int, int>;
Circle circle;

void Process(const char* input, int turns, int part)
{
	// Initialise the circle
	circle.clear();
	int minvalue = INT_MAX;
	int maxvalue = 0;
	std::vector<int> inputs;
	for (const char *c = input; *c; ++c)
	{
		int n = *c - '0';
		inputs.push_back(n);
		if (n > maxvalue)
			maxvalue = n;
		if (n < minvalue)
			minvalue = n;
	}
	int lastValue = 0;
	for (int i = 0; i < inputs.size(); ++i)
	{
		circle[inputs[i]] = inputs[(i + 1) % inputs.size()];
		lastValue = inputs[i];
	}

	if (part == 2)
	{
		// increase size to a million entries
		while (maxvalue < 1000000)
		{
			++maxvalue;
			circle[lastValue] = maxvalue;
			lastValue = maxvalue;
		}
		circle[lastValue] = inputs[0];
	}
	
	int value = inputs[0];
	for (int t = 0; t < turns; ++t)
	{
		std::set<int> tset;
		// remove 3 pieces clockwise from current position
		int next = circle[value];
		int x = next;
		int last;
		for (int i = 0; i < 3; ++i)
		{
			tset.insert(x);
			last = x;
			x = circle[x];
		}
		circle[value] = x;
		// choose a lower value
		int newvalue = value;
		while (newvalue == value || tset.count(newvalue))
		{
			--newvalue;
			if (newvalue < minvalue)
				newvalue = maxvalue;
		}
		// insert 3 pieces after this value
		circle[last] = circle[newvalue];
		circle[newvalue] = next;
		// next position
		value = circle[value];
	}

	if (part == 1)
	{
		// split circle at '1'
		std::string result;
		int v = circle[1];
		while (v != 1)
		{
			result.push_back(v + '0');
			v = circle[v];
		}
		printf("Part 1: input %s, %d turns, output %s\n", input, turns, result.data());
	}
	else
	{
		int mult = 0;
		int v = circle[1];
		int64 result = v;
		result *= circle[v];
		printf("Part 2: input %s, %d turns, output %d*%d= %lld\n", input, turns, v, circle[v], result);
	}
}

int main()
{
	Process("389125467", 10, 1);
	Process("389125467", 100, 1);
	Process("167248359", 100, 1); // puzzle input

	Process("389125467", 10000000, 2);
	Process("167248359", 10000000, 2); // puzzle input
}
