// day15.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 15/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node : public INode
{
	Node(int _cost = 0)
		: cost(_cost)
	{}
	int cost;
	virtual int getCost() const override { return cost; }
};

void Process(const char* filename)
{
	std::map<coord, Node> map;

	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos;
	coord bottomRight;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int len = strlen(thisLine);
				for (pos.first = 0; pos.first < len; ++pos.first)
				{
					Node n(thisLine[pos.first] - '0');
					map.insert(std::make_pair(pos, n));
					if (pos.first > bottomRight.first)
						bottomRight.first = pos.first;
				}
				if (pos.second > bottomRight.second)
					bottomRight.second = pos.second;
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	coord start(0, 0);
	coord end = bottomRight;

	int64 cost = aStarSearch(start, end, map);
	printf("%s: Part 1: %lld\n", filename, cost);

	// part 2: expand the map
	for (pos.second = 0; pos.second <= bottomRight.second; ++pos.second)
	{
		for (pos.first = 0; pos.first <= bottomRight.first; ++pos.first)
		{
			int base = map[pos].getCost();
			for (int y = 0; y < 5; ++y)
			{
				for (int x = 0; x < 5; ++x)
				{
					Node n((base + x + y - 1) % 9 + 1);
					coord nPos = pos;
					nPos.first += x * (bottomRight.first + 1);
					nPos.second += y * (bottomRight.second + 1);
					map.insert(make_pair(nPos, n));
				}
			}
		}
	}

	int64 newSize = (bottomRight.first + 1) * 5 - 1;
	coord end2(newSize, newSize);
	int64 cost2 = aStarSearch(start, end2, map);
	printf("%s: Part 2: %lld\n", filename, cost2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
