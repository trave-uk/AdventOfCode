// day22.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 27/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node
{
	int x;
	int y;

	int sizeT;
	int usedT;
	int availT;
	int percent;

	int GetIndex() { return y * 32 + x; }
};

void Load(const char* filename, std::map<int, Node>& nodes)
{
	nodes.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				if (*thisLine == '/')
				{
					Node node;
					int fields = sscanf(thisLine, "/dev/grid/node-x%d-y%d %dT %dT %dT %d%%", &node.x, &node.y, &node.sizeT, &node.usedT, &node.availT, &node.percent);
					assert(fields == 6);
					nodes.insert(std::make_pair(node.GetIndex(), node));
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int part1 = 0;
	for (auto nodeA : nodes)
	{
		for (auto nodeB : nodes)
		{
			if (nodeA.first == nodeB.first)
				continue;
			if (nodeA.second.usedT == 0)
				continue;
			if (nodeA.second.usedT > nodeB.second.availT)
				continue;
			++part1;
		}
	}
	printf( "%s Part 1: %d\n", filename, part1 );
}

int main()
{
	std::map<int, Node> nodes;
	Load("example.txt", nodes);

	Load("input.txt", nodes);
	return 0;
}