// day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void Process(std::vector<int> input)
{
	std::vector<int> output = input;
	while (output.size() < 2020)
	{
		int last = output.back();
		int count = 0;
		for (auto iter = output.rbegin(); iter != output.rend(); ++iter)
		{
			if (count > 0 && *iter == last)
			{
				break;
			}
			++count;
		}
		if (count == output.size())
			count = 0;
		output.push_back(count);
	}
	std::string stringInput;
	for (int i : input)
	{
		stringInput += std::to_string(i);
		stringInput += ',';
	}
	printf("  - Given the starting numbers %s the 2020th number spoken is %d.\n", stringInput.data(), output.back());
}

void Process2(std::vector<int64> input, int64 target = 2020L)
{
	std::map<int64, int64> seen;
	int64 ix = 0;
	for (int64 i : input)
	{
		if (ix < (int64)input.size() - 1)
			seen[i] = ix;
		++ix;
	}
	int64 number = input.back();
	while (ix < target)
	{
		int64 newnum;
		if (!seen.count(number))
		{
			newnum = 0;
		}
		else
		{
			newnum = ix - 1 - seen[number];
		}
		seen[number] = ix-1;
		++ix;
		number = newnum;
	}
	std::string stringInput;
	for (int64 i : input)
	{
		stringInput += std::to_string(i);
		stringInput += ',';
	}
	printf("  - Given %s the %lldth number spoken is %lld.\n", stringInput.data(), target, number);
}

int main()
{
	Process2({ 0,3,6 });
	Process2({ 1,3,2 });
	Process2({ 2,1,3 });
	Process2({ 1,2,3 });
	Process2({ 2,3,1 });
	Process2({ 3,2,1 });
	Process2({ 3,1,2 });
	Process2({ 13,16,0,12,15,1 });
	printf("\n");
	int64 target = 30000000L;
	Process2({ 0,3,6 }, target);
	Process2({ 1,3,2 }, target);
	Process2({ 2,1,3 }, target);
	Process2({ 1,2,3 }, target);
	Process2({ 2,3,1 }, target);
	Process2({ 3,2,1 }, target);
	Process2({ 3,1,2 }, target);
	Process2({ 13,16,0,12,15,1 }, target);
}