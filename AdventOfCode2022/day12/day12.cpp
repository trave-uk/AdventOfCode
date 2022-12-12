// day12.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 12/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node : public INode
{
	Node(char h = 0) :
		height(h)
	{
	}

	virtual bool isBlocked(coord& from) const override;
	virtual int getCost(coord& from) const override;

	char height = 0; // 'a' to 'z'
};

std::map<coord, Node> space;

bool Node::isBlocked(coord& from) const 
{
	auto it = space.find(from);
	if (height - it->second.height > 1)
		return true;
	else
		return false;
}

int Node::getCost(coord& from) const
{
	auto it = space.find(from);
	if (height - it->second.height > 1)
		return 100000;
	else
		return 1;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<std::string> grid;

	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos, start, end;
	space.clear();
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				grid.push_back(thisLine);
				for (char c : grid.back())
				{
					char height = c;
					if (c == 'S')
					{
						height = 'a';
						start = pos;
					}
					else if (c == 'E')
					{
						height = 'z';
						end = pos;
					}
					Node n(height);
					space.insert(std::make_pair(pos, n));
					++pos.first;
				}
				pos.first = 0;
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	std::vector<coord> path;
	part1 = aStarSearch(start, end, space, &path);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	std::set<int64> distances;

	for (std::pair<coord, Node> pos : space)
	{
		if (pos.second.height == 'a')
		{
			int64 d = aStarSearch(pos.first, end, space);
			if (d != -1)
			{
				distances.insert(d);
			}
		}
	}

	part2 = *distances.begin();

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 31, 29);
	Process("input.txt");

	return 0;
}
