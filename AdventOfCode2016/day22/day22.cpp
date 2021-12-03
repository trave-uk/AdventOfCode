// day22.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 27/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node
{
	coord pos;

	int size;
	int used;

	static int maxUsableSize;

	int getAvail() const { return getSize() - getUsed(); }
	int getSize() const { return size; }
	int getUsed() const { return used & 0x7fff; }
	void markTarget() { used |= 0x8000; }
	bool containsTarget() const { return (used & 0x8000) == 0x8000; }
	bool isBlocked() const { return used >= maxUsableSize; } // NB the current target position counts as blocked
};

int Node::maxUsableSize = 0;

void Load(const char* filename, std::map<coord, Node>& nodes, coord& topRight, coord &emptyNode)
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
					int avail, percent;
					Node node;
					int fields = sscanf(thisLine, "/dev/grid/node-x%lld-y%lld %dT %dT %dT %d%%", &node.pos.first, &node.pos.second, &node.size, &node.used, &avail, &percent);
					assert(fields == 6);
					assert((node.size - node.used) == avail);
					nodes.insert(std::make_pair(node.pos, node));
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int part1 = 0;
	int closeNeighbours = 0;
	
	topRight = coord(0, 0);

	int lowestUsableSize = INT_MAX;
	int lowestUsableUsed = INT_MAX;
	int emptyNodes = 0;

	for (auto nodeA : nodes)
	{
		if (nodeA.second.pos.second == 0 && nodeA.second.pos.first > topRight.first)
		{
			topRight = nodeA.second.pos;
		}
		if (nodeA.second.getSize() < lowestUsableSize)
		{
			lowestUsableSize = nodeA.second.getSize();
		}
		if (nodeA.second.used > 0 && nodeA.second.used < lowestUsableUsed)
		{
			lowestUsableUsed = nodeA.second.used;
		}
		if (nodeA.second.used == 0)
		{
			emptyNode = nodeA.first;
			++emptyNodes;
			continue;
		}
		for (auto nodeB : nodes)
		{
			if (nodeA.first == nodeB.first)
				continue;
			if (nodeA.second.used > nodeB.second.getAvail())
				continue;
			++part1;
			// Check whether these two nodes are actually neighbours (in which case we have a cheap empty node for part 2)
			if (((nodeA.second.pos.first == nodeB.second.pos.first) && (abs(nodeA.second.pos.second - nodeB.second.pos.second) <= 1)) ||
				((abs(nodeA.second.pos.first - nodeB.second.pos.first) <= 1) && (nodeA.second.pos.second == nodeB.second.pos.second)))
			{
				++closeNeighbours;
			}
		}
	}
	assert(emptyNodes == 1);
	// having got absolute lowest size, see how much bigger nodes with larger used actually are
	int impassableNodes = 0;
	int lowestUnusableSize = INT_MAX;
	int lowestUnusableUsed = INT_MAX;
	int largestUnusableAvail = 0;
	int passableNodes = 0;
	int largestUsableUsed = 0;
	for (auto node : nodes)
	{
		if (node.second.used > lowestUsableSize)
		{
			++impassableNodes;
			if (node.second.getSize() < lowestUnusableSize)
			{
				lowestUnusableSize = node.second.getSize();
			}
			if (node.second.used < lowestUnusableUsed)
			{
				lowestUnusableUsed = node.second.used;
			}
			if (node.second.getAvail() > largestUnusableAvail)
			{
				largestUnusableAvail = node.second.getAvail();
			}
		}
		else
		{
			++passableNodes;
			if (node.second.used > largestUsableUsed)
			{
				largestUsableUsed = node.second.used;
			}
		}
	}
	Node::maxUsableSize = lowestUnusableUsed;

	printf("%s\nPart 1: %d\n", filename, part1);
	printf("Of these, %d are neighbours\n", closeNeighbours);
	printf("%d passable nodes: lowest size %d, largest used %d, lowest used %d\n", passableNodes, lowestUsableSize, largestUsableUsed, lowestUsableUsed);
	printf("%d impassable nodes: lowest size %d, lowest used %d, largest avail %d\n", impassableNodes, lowestUnusableSize, lowestUnusableUsed, largestUnusableAvail);
	assert(largestUnusableAvail < lowestUsableUsed);
	assert(lowestUnusableUsed > lowestUsableSize);
	// Assuming all viable pairs are between a passable node and the empty node, then the number of such pairs (part1) is the number of passable nodes-1 (-1 being the empty node)
	assert(passableNodes == part1 + 1);
	nodes[topRight].markTarget();
}

void Process(std::map<coord, Node> nodes, coord topRight, coord emptyNode)
{
	// Find the least number of "moves" to get the data (used) in the top right to position (0,0) (the top left)
	// In the example, there is one empty node, one impassible node (too big and full) and every passible node contains enough space (if empty) to contain any other node's data. The (mobile) empty node is used to move data around.
	// Given there are only 937 viable pairs, with 960 nodes, it's rare for any node to have enough space without emptying itself first to contain another node's data.
	// In the input data, there is also one empty node, there are 22 impassible nodes (too full), so the 937 viable pairs are all between passable nodes and the empty node. So we can reduce this to simple pathfinding.
	// Of the 937 viable pairs, only 4 are neighbours, which fits with the previous assumption.

	// So. In the initial state, there are 4 possible moves. After each move, there are up to 3 available moves that don't go back.
	// We only care about the location of the target payload (using the empty node to move it around).
	// We could do an exhaustive search. This seems unlikely to be useful.

	// Suggestion:
	// Determine shortest route from topRight to 0 that doesn't pass through any impassible nodes.
	// Then work out how to make each successive position free by moving the empty node there, also avoiding impassible nodes and the current target location.
	// Use the A* algorithm. I don't _seem_ to have used this previously in 4.8 years of AoC, but I should catalogue what each AoC puzzle I've solved actually uses...

	// TODO:
	//  * implement (download) A* algorithm
	//  * use A* to solve this puzzle, as descibed abive
	coord goal(0, 0);
	std::vector<coord> path;
	int64 distance = aStarSearch(topRight, goal, nodes, &path);
	printf("distance to move %lld\n", distance);

	coord prevPos = topRight;
	for (coord pos : path)
	{
		// Skip the first position, which is where we start
		if (pos == prevPos)
			continue;

		// Along each step, first make pos empty by finding a path from the empty node to pos. For this we just need the distance.
		distance += aStarSearch(emptyNode, pos, nodes);

		// Then, move the payload from prevPos to pos;
		nodes[emptyNode].used = nodes[pos].used;
		nodes[pos].used = nodes[prevPos].used;
		nodes[prevPos].used = 0;
		emptyNode = prevPos;

		// No need to add this move to distance, as distance already contains the path to take
		prevPos = pos;
	}

	printf("Part 2: %lld\n", distance);
}

std::map<coord, Node> nodes;

int main()
{
	coord topRight, emptyNode;
	Load("example.txt", nodes, topRight, emptyNode);
	Process(nodes, topRight, emptyNode);

	printf("\n");

	Load("input.txt", nodes, topRight, emptyNode);
	Process(nodes, topRight, emptyNode);
	return 0;
}