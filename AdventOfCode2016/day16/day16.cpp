// day16.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 17/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void test(const char* name, int length, std::string initial)
{
	std::string disk = initial;
	disk.reserve(length);
	
	std::string copy;
	copy.reserve(length);
	while (disk.length() < length)
	{
		copy = disk;
		disk.append("0");
		for (std::string::reverse_iterator it = copy.rbegin(); it != copy.rend(); ++it)
		{
			if (disk.length() == length)
				break;

			char c = *it;
			if (c == '1')
			{
				disk.append("0");
			}
			else
			{
				disk.append("1");
			}
		}
	}

	while ((disk.length() % 2) == 0)
	{
		std::string next;
		for (int i = 0; i < disk.length(); i += 2)
		{
			if (disk[i] == disk[i+1])
			{
				next.append("1");
			}
			else
			{
				next.append("0");
			}
		}
		disk = next;
	}
	printf("%s: checksum: %s\n", name, disk.c_str());
}

int main()
{
	test("Example 1", 12, "110010110100");
	test("Example 2", 20, "10000");
	test("Part 1", 272, "00111101111101000");
	test("Part 2", 35651584, "00111101111101000");
	return 0;
}
