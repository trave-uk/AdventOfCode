// day22.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 22/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Direction
{
	NONE = -1,
	RIGHT,
	DOWN,
	LEFT,
	UP
};

enum Turn
{
	TURN_NONE = 0,
	TURN_RIGHT = 1,
	TURN_LEFT = -1
};

struct Action
{
	int64 move = 0;
	Turn turn = TURN_NONE;
};

struct Edge
{
	Edge() = default;
	Edge(const coord &p, Direction out, const coord &q, Direction in)
		: pos(p)
		, dir(out)
		, newPos(q)
		, newDir(in)
	{
	}
	coord pos;
	Direction dir = NONE;
	coord newPos;
	Direction newDir = NONE;
};

using EdgeKey = std::pair<coord, Direction>;
using EdgeMap = std::map<EdgeKey, Edge>;

void GenerateEdge(EdgeMap& edges, int faceSize, const coord& outStart, const coord& outStep, Direction outDir, const coord& inStart, const coord& inStep, Direction inDir, const std::vector<std::string>& map)
{
	coord out = outStart;
	coord in = inStart;
	Direction backDir = (Direction)(((int)inDir + 2) % 4);
	Direction backInDir = (Direction)(((int)outDir + 2) % 4);
	coord offStep((outDir == RIGHT) - (outDir == LEFT), (outDir == DOWN) - (outDir == UP));
	coord backStep((backDir == RIGHT) - (backDir == LEFT), (backDir == DOWN) - (backDir == UP));
	for (int f = 0; f < faceSize; ++f)
	{
		coord test = out;
		assert(map[test.second][test.first] != ' ');
		test += offStep;
		assert(map[test.second][test.first] == ' ');

		Edge edge = Edge(out, outDir, in, inDir);
		EdgeKey key = std::make_pair(out, outDir);
		edges.insert(std::make_pair(key, edge));

		test = in;
		assert(map[test.second][test.first] != ' ');
		test += backStep;
		assert(map[test.second][test.first] == ' ');

		Edge edge2 = Edge(in, backDir, out, backInDir);
		EdgeKey key2 = std::make_pair(in, backDir);
		edges.insert(std::make_pair(key2, edge2));

		out += outStep;
		in += inStep;
	}
}

coord GetCorner(int faceSize, Direction topBottom, Direction leftRight, int x, int y)
{
	coord c((x - 1) * faceSize, (y - 1) * faceSize);
	if (topBottom == UP)
		c.second += 1;
	else if (topBottom == DOWN)
		c.second += faceSize;
	else
		assert(false);
	if (leftRight == LEFT)
		c.first += 1;
	else if (leftRight == RIGHT)
		c.first += faceSize;
	else
		assert(false);
	return c;
}

// The example and the actual input do not have the same shape, but I'm just going to account for those two shapes and orientations rather than cope with any unwrapped cube
void GenerateCubeEdges(EdgeMap& edges, const std::vector<std::string>& map)
{
	const coord stepRight(1, 0);
	const coord stepLeft(-1, 0);
	const coord stepUp(0, -1);
	const coord stepDown(0, 1);

	if (map.size() < 25)
	{ // example.txt
		// faces 4x4
		// 
		//   O
		// OOO
		//   OO
		int faceSize = 4;
		coord topLeft31 = GetCorner(faceSize, UP, LEFT, 3, 1); // (9, 1)
		coord topRight12 = GetCorner(faceSize, UP, RIGHT, 1, 2); // (4, 5)
		GenerateEdge(edges, faceSize, topLeft31, stepRight, UP, topRight12, stepLeft, DOWN, map);
		coord topLeft22 = GetCorner(faceSize, UP, LEFT, 2, 2); // (5, 5)
		GenerateEdge(edges, faceSize, topLeft31, stepDown, LEFT, topLeft22, stepRight, DOWN, map);
		coord topRight31 = GetCorner(faceSize, UP, RIGHT, 3, 1); // (12, 1)
		coord bottomRight43 = GetCorner(faceSize, DOWN, RIGHT, 4, 3); // (16, 12)
		GenerateEdge(edges, faceSize, topRight31, stepDown, RIGHT, bottomRight43, stepUp, LEFT, map);
		coord topRight32 = GetCorner(faceSize, UP, RIGHT, 3, 2); // (12, 5)
		coord topRight43 = GetCorner(faceSize, UP, RIGHT, 4, 3); // (16, 9)
		GenerateEdge(edges, faceSize, topRight32, stepDown, RIGHT, topRight43, stepLeft, DOWN, map);
		coord topLeft12 = GetCorner(faceSize, UP, LEFT, 1, 2); // (1, 5);
		GenerateEdge(edges, faceSize, topLeft12, stepDown, LEFT, bottomRight43, stepLeft, UP, map);
		coord bottomLeft12 = GetCorner(faceSize, DOWN, LEFT, 1, 2); // (1, 8);
		coord bottomRight33 = GetCorner(faceSize, DOWN, RIGHT, 3, 3); // (12, 12);
		GenerateEdge(edges, faceSize, bottomLeft12, stepRight, DOWN, bottomRight33, stepLeft, UP, map);
		coord bottomLeft22 = GetCorner(faceSize, DOWN, LEFT, 2, 2); // (5, 8);
		coord bottomLeft33 = GetCorner(faceSize, DOWN, LEFT, 3, 3); // (9, 12);
		GenerateEdge(edges, faceSize, bottomLeft22, stepRight, DOWN, bottomLeft33, stepUp, RIGHT, map);
	}
	else
	{ // input.txt
		// faces 50x50
		// 
		//  OO
		//  O
		// OO
		// O
		int faceSize = 50;
		coord bottomLeft22 = GetCorner(faceSize, DOWN, LEFT, 2, 2);
		coord topRight13 = GetCorner(faceSize, UP, RIGHT, 1, 3);
		GenerateEdge(edges, faceSize, bottomLeft22, stepUp, LEFT, topRight13, stepLeft, DOWN, map);
		coord bottomLeft21 = GetCorner(faceSize, DOWN, LEFT, 2, 1);
		coord topLeft13 = GetCorner(faceSize, UP, LEFT, 1, 3);
		GenerateEdge(edges, faceSize, bottomLeft21, stepUp, LEFT, topLeft13, stepDown, RIGHT, map);
		coord topLeft21 = GetCorner(faceSize, UP, LEFT, 2, 1);
		coord topLeft14 = GetCorner(faceSize, UP, LEFT, 1, 4);
		GenerateEdge(edges, faceSize, topLeft21, stepRight, UP, topLeft14, stepDown, RIGHT, map);
		coord bottomLeft23 = GetCorner(faceSize, DOWN, LEFT, 2, 3);
		coord topRight14 = GetCorner(faceSize, UP, RIGHT, 1, 4);
		GenerateEdge(edges, faceSize, bottomLeft23, stepRight, DOWN, topRight14, stepDown, LEFT, map);
		coord bottomLeft31 = GetCorner(faceSize, DOWN, LEFT, 3, 1);
		coord topRight22 = GetCorner(faceSize, UP, RIGHT, 2, 2);
		GenerateEdge(edges, faceSize, bottomLeft31, stepRight, DOWN, topRight22, stepDown, LEFT, map);
		coord topRight31 = GetCorner(faceSize, UP, RIGHT, 3, 1);
		coord bottomRight23 = GetCorner(faceSize, DOWN, RIGHT, 2, 3);
		GenerateEdge(edges, faceSize, topRight31, stepDown, RIGHT, bottomRight23, stepUp, LEFT, map);
		coord bottomRight14 = GetCorner(faceSize, DOWN, RIGHT, 1, 4);
		GenerateEdge(edges, faceSize, topRight31, stepLeft, UP, bottomRight14, stepLeft, UP, map);
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<std::string> map;
	std::vector<Action> path;

	std::string emptyline = "                                                                                                                                                                                                             ";
	map.push_back(emptyline);
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (isalnum(*thisLine))
				{
					char* read = thisLine;
					while (*read)
					{
						if (isdigit(*read))
						{
							Action a;
							a.move = atoi(read);
							read += strspn(read, "0123456789");
							path.push_back(a);
						}
						else
						{ 
							assert(*read == 'L' || *read == 'R');
							Action a;
							a.turn = (*read == 'R') ? TURN_RIGHT : TURN_LEFT;
							++read;
							path.push_back(a);
						}
					}
				}
				else
				{
					std::string line = std::string(" ") + thisLine + emptyline;
					map.push_back(line);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	map.push_back(emptyline);

	int64 y = 1;
	int64 x = strspn(map[y].c_str(), " ");
	{
		enum Direction dir = RIGHT;
		coord pos(x, y);

		for (const Action& a : path)
		{
			if (a.turn)
			{
				dir = Direction((int(dir) + int(a.turn) + 4) % 4);
			}
			else if (a.move)
			{
				for (int i = 0; i < a.move; ++i)
				{
					coord step;
					step.first = (dir == RIGHT) - (dir == LEFT);
					step.second = (dir == DOWN) - (dir == UP);
					coord newPos = pos;
					newPos += step;
					char c = map[newPos.second][newPos.first];
					if (c == ' ')
					{
						do
						{
							newPos -= step;
							c = map[newPos.second][newPos.first];
						} while (c != ' ');
						newPos += step;
						c = map[newPos.second][newPos.first];
					}
					assert(c == '.' || c == '#');
					if (c == '#')
						break;
					pos = newPos;
				}
			}
			else
				assert(false);
		}
		part1 = 1000 * pos.second + 4 * pos.first + (int64)dir;
	}
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	{
		// For part 2, generate some metadata to reflect the cube-connectivity of the edges of the map
		EdgeMap edges;
		GenerateCubeEdges(edges, map);

		enum Direction dir = RIGHT;
		coord pos(x, y);

		for (const Action& a : path)
		{
			if (a.turn)
			{
				assert(!a.move);
				dir = Direction((int(dir) + int(a.turn) + 4) % 4);
			}
			else if (a.move)
			{
				for (int i = 0; i < a.move; ++i)
				{
					coord step;
					step.first = (dir == RIGHT) - (dir == LEFT);
					step.second = (dir == DOWN) - (dir == UP);
					coord newPos = pos;
					newPos += step;
					Direction newDir = dir;
					char c = map[newPos.second][newPos.first];
					if (c == ' ')
					{
						EdgeKey key = std::make_pair(pos, dir);
						assert(edges.count(key) > 0);
						Edge& e = edges[key];
						newPos = e.newPos;
						newDir = e.newDir;
						c = map[newPos.second][newPos.first];
					}
					assert(c == '.' || c == '#');
					if (c == '#')
						break;
					pos = newPos;
					dir = newDir;
				}
			}
			else
				assert(false);
		}
		part2 = 1000 * pos.second + 4 * pos.first + (int64)dir;
	}

	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 6032, 5031);
	Process("input.txt");

	return 0;
}

// not 126384