// day24.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 4/12/2021 https://github.com/trave-uk/AdventOfCode
//

// Essentially a "traveling salesman" problem, except that we don't need to return to the start (for part 1 at least).
//
// Checking input.txt, there are 8 numbered nodes, 0 to 7, and we start at 0.
// Use A* to calculate the shortest distance between nodes. There being 8 nodes, that's 7+6+5+4+3+2+1 = 28 paths. Store in an 8x8 2D array, duplicating distances in two directions
// Then it's probably feasible to brute-force all combinations. 7! = 7*6*5*4*3*2 = "only" 5040 possible routes.
//
// NB: to add "return to start" there are the same number of combinations to test

#include "stdafx.h"

struct Node
{
	int id = -1;
	coord pos;
	bool blocked = false;
	bool isBlocked() { return blocked; }
};

std::map<coord, Node> maze;
std::array<coord, 8> POIs;
int nPOI = 0;

bool load(const char *filename)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	if (!fp)
		return false;
	coord pos;
	nPOI = 0;
	maze.clear();
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line = thisLine;
				for (pos.first = 0; pos.first < line.length(); ++pos.first)
				{
					char c = line[pos.first];
					Node node;
					node.pos = pos;
					node.blocked = (c == '#');
					if (isdigit(c))
					{
						node.id = c - '0';
						POIs[node.id] = pos;
						++nPOI;
					}
					maze.insert(std::make_pair(pos, node));
				}
				pos.second++;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	return true;
}

void run(const char* filename)
{
	printf("\n%s:\n", filename);
	load(filename);

	// find distances between all points of interest
	std::array<std::array<int64, 8>, 8> distances;
	for (int i = 0; i < nPOI; ++i)
	{
		for (int j = 0; j < nPOI; ++j)
		{
			// Don't bother filtering, just find all distances even if duplicating effort
			distances[i][j] = aStarSearch(POIs[i], POIs[j], maze);
			assert(distances[i][j] >= 0);
		}
	}

	// now, starting at POI 0, try all combinations of these distances.
	using Path = std::vector<int>;
	std::set<Path> paths;
	Path path;
	path.push_back(0);
	paths.insert(path);

	int nPaths = 0;
	int64 minDistance = INT64_MAX;
	int64 part2 = INT64_MAX;
	while (!paths.empty())
	{
		Path p = *paths.begin();
		paths.erase(p);

		for (int i = 1; i < nPOI; ++i)
		{
			if (std::find(p.begin(), p.end(), i) != p.end())
				continue;
			Path q = p;
			q.push_back(i);
			if (q.size() == nPOI)
			{
				// Test this path
				int64 distance = 0;
				for (int k = 1; k < nPOI; ++k)
				{
					distance += distances[q[k - 1]][q[k]];
				}
				if (distance < minDistance)
				{
					minDistance = distance;
				}
				distance += distances[q[0]][q[nPOI - 1]];
				if (distance < part2)
				{
					part2 = distance;
				}
				++nPaths;
			}
			else
			{
				paths.insert(q);
			}
		}
	}

	printf("Part 1: %d paths found; shortest path %lld steps\n", nPaths, minDistance);
	printf("Part 2: %lld steps\n", part2);
}

int main()
{
	run("example.txt");
	run("input.txt");
	return 0;
}
