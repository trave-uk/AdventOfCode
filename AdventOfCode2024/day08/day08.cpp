// day08.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 08/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node
{
	Node(char n = '.') : name(n) {}
	char name;
	std::vector<coord> positions;
};

using NodeMap = std::map<char, Node>;

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<std::string> array;
	NodeMap nodes;
	std::set<coord> antinodes;
	std::set<coord> antinodes2;

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
				array.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	coord pos;
	pos.second = 0;
	for (auto line : array)
	{
		pos.first = 0;
		for (char c : line)
		{
			if (c != '.')
			{
				if (nodes.find(c) == nodes.end())
				{
					nodes[c] = Node(c);
				}
				nodes[c].positions.push_back(pos);
			}
			++pos.first;
		}
		++pos.second;
	}
	int64 height = array.size();
	int64 width = array[0].length();
	coord min_(0, 0);
	coord max_(width-1, height-1);
	for (auto n : nodes)
	{
		const auto node = n.second;
		for (size_t i = 0; i < node.positions.size(); ++i)
		{
			for (size_t j = i + 1; j < node.positions.size(); ++j)
			{
				coord dir = node.positions[j];
				dir -= node.positions[i];
				coord antinode = node.positions[i];
				antinode -= dir;
				if (antinode.isContained(min_, max_))
				{
					antinodes.insert(antinode);
				}
				antinode = node.positions[j];
				antinode += dir;
				if (antinode.isContained(min_, max_))
				{
					antinodes.insert(antinode);
				}
				// for part 2, +-width should be big enough as a multiplier
				for (int64 k = -width; k < width; ++k)
				{
					coord offset = dir;
					offset *= k;
					coord an = node.positions[i];
					an += offset;
					if (an.isContained(min_, max_))
					{
						antinodes2.insert(an);
					}
				}
			}
		}
	}

	part1 = antinodes.size();
	part2 = antinodes2.size();

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 14, 34);
	Process("input.txt");

	return 0;
}
