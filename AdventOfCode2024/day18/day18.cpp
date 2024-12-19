// day18.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 18/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Bytes = std::vector<coord>;

class Node : public INode
{
public:
	Node(const char c = '.') : cell(c) {}
	virtual bool isBlocked() const override { return cell == '#'; }
	virtual int getCost() const override { return 1; }
	char cell;
};

using Map = std::map<coord, Node>;

int64 TestStep(const Bytes& bytes, const int64 size, const int64 step)
{
	coord start(0, 0);
	coord goal(size, size);
	Map map;
	coord pos;
	for (pos.second = 0; pos.second <= size; ++pos.second)
	{
		for (pos.first = 0; pos.first <= size; ++pos.first)
		{
			map[pos] = Node('.');
		}
	}
	int64 i = 0;
	for (const coord& byte : bytes)
	{
		map[byte] = Node('#');
		if (++i == step)
			break;
	}
	return aStarSearch(start, goal, map);
}

int64 Calculate(const Bytes& bytes, int64 size, int64 part1steps, std::string& part2)
{
	int64 part1 = TestStep(bytes, size, part1steps);

	// Binary search
	int64 low = part1steps;
	int64 high = bytes.size() - 1;

	while (low < high - 1)
	{
		int64 test = (low + high) / 2;
		int64 result = TestStep(bytes, size, test);
		if (result == -1)
			high = test;
		else
			low = test;
	}
	const coord& byte = bytes[low];
	part2 = string_format("%lld,%lld", byte.first, byte.second);
	return part1;
}

void Process(const char* filename, int64 size, int64 part1steps, int64 expectedPart1 = -1, std::string expectedPart2 = "")
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	std::string part2 = "";
	Bytes bytes;
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
				coord byte;
				// 5,4
				int found = sscanf_s(thisLine, "%lld,%lld", &byte.first, &byte.second);
				assert(found == 2);
				bytes.push_back(byte);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = Calculate(bytes, size, part1steps, part2);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == "" || expectedPart2 == part2);
	printf("%s: Part 2: %s\n", filename, part2.c_str());

}

int main()
{
	Process("example.txt", 6, 12, 22, "6,1");
	Process("input.txt", 70, 1024);

	return 0;
}
