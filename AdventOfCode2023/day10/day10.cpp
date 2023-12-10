// day10.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 10/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Direction
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

Direction opposite(Direction d)
{
	switch (d)
	{
	case NONE:
		return NONE;
	case LEFT:
		return RIGHT;
	case RIGHT:
		return LEFT;
	case UP:
		return DOWN;
	case DOWN:
		return UP;
	}
}

void sides(Direction d, Direction& l, Direction& r)
{
	switch (d)
	{
	case LEFT:
		l = DOWN;
		r = UP;
		break;
	case RIGHT:
		l = UP;
		r = DOWN;
		break;
	case UP:
		l = LEFT;
		r = RIGHT;
		break;
	case DOWN:
		l = RIGHT;
		r = LEFT;
		break;
	}
}

struct Connectivity
{
	Connectivity(bool l = false, bool r = false, bool u = false, bool d = false, Direction d1 = NONE, Direction d2 = NONE)
		: left(l)
		, right(r)
		, up(u)
		, down(d)
		, dir1(d1)
		, dir2(d2)
	{
	}
	bool operator==(Connectivity& other)
	{
		return left == other.left && right == other.right && up == other.up && down == other.down && dir1 == other.dir1 && dir2 == other.dir2;
	}
	bool left;
	bool right;
	bool up;
	bool down;
	Direction dir1, dir2;
};

std::map<char, Connectivity> connectivity = {
	{ '.', Connectivity(false, false, false, false, NONE, NONE) },
	{ 'F', Connectivity(false, true, false, true, RIGHT, DOWN) },
	{ 'L', Connectivity(false, true, true, false, RIGHT, UP) },
	{ 'J', Connectivity(true, false, true, false, LEFT, UP) },
	{ '7', Connectivity(true, false, false, true, LEFT, DOWN) },
	{ '|', Connectivity(false, false, true, true, UP, DOWN) },
	{ '-', Connectivity(true, true, false, false, LEFT, RIGHT) },
};

using Map = std::map<coord, char>;

char checkMap(coord pos, Map& map)
{
	if (map.count(pos) > 0)
		return map[pos];
	else
		return '.';
}

void ReplaceStart(Map& map, coord start)
{
	// Given start, find connectivity
	bool up = false, down = false, left = false, right = false;
	std::array<coord, 4> tests = {
		coord(start.first - 1, start.second),
		coord(start.first + 1, start.second),
		coord(start.first, start.second - 1),
		coord(start.first, start.second + 1),
	};
	Connectivity startCon;
	Connectivity test = connectivity[checkMap(coord(start.first - 1, start.second), map)]; // left
	if (test.right)
	{
		startCon.left = true;
		startCon.dir1 = LEFT;
	}
	test = connectivity[checkMap(coord(start.first + 1, start.second), map)]; // right
	if (test.left)
	{
		startCon.right = true;
		if (startCon.dir1 == NONE)
			startCon.dir1 = RIGHT;
		else
			startCon.dir2 = RIGHT;
	}
	test = connectivity[checkMap(coord(start.first, start.second - 1), map)]; // up
	if (test.down)
	{
		startCon.up = true;
		if (startCon.dir1 == NONE)
			startCon.dir1 = UP;
		else
			startCon.dir2 = UP;
	}
	test = connectivity[checkMap(coord(start.first, start.second + 1), map)]; // down
	if (test.up)
	{
		startCon.down = true;
		if (startCon.dir1 == NONE)
			startCon.dir1 = DOWN;
		else
			startCon.dir2 = DOWN;
	}
	for (auto x : connectivity)
	{
		if (x.second == startCon)
		{
			map[start] = x.first;
		}
	}
}

void floodFill(Map& map, Map &map2, coord pos, char value, int64& count)
{
	std::queue<coord> openSet;
	openSet.push(pos);
	while (!openSet.empty())
	{
		coord p = openSet.front();
		openSet.pop();
		char v = checkMap(p, map2);
		if (v == '.' && map.count(p))
		{
			map2[p] = value;
			++count;
			for (int i = 0; i < 4; ++i)
			{
				coord q = p;
				q.first += (i == 0) - (i == 1);
				q.second += (i == 2) - (i == 3);
				openSet.push(q);
			}
		}
	}

}

coord getDelta(Direction move)
{
	coord delta = coord((move == RIGHT) - (move == LEFT), (move == DOWN) - (move == UP));
	return delta;
}

void printMap(Map& map, int64 max)
{
	for (int64 y = 0; y < max; ++y)
	{
		for (int64 x = 0; x < max; ++x)
		{
			coord pos(x, y);
			char c = '*';
			if (map.count(pos) != 0)
			{
				c = map[pos];
			}
			_putch(c);
		}
		_putch('\n');
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	int64 y = 0;
	Map map;
	coord start;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				int x = 0;
				for (char c : line)
				{
					coord pos(x, y);
					if (c == 'S')
						start = pos;
					map[pos] = c;
					++x;
				}
				++y;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	ReplaceStart(map, start);
	Connectivity startCon = connectivity[map[start]];

	Map part2map; // This will be filled in with in (I), out (O), path (P)
	part2map[start] = 'P';
	Direction firstFrom = opposite(startCon.dir1);
	Direction secondFrom = opposite(startCon.dir2);
	coord first = start;
	coord second = start;
	do
	{
		Connectivity firstCon = connectivity[map[first]];
		Direction firstMove = firstCon.dir1 == opposite(firstFrom) ? firstCon.dir2 : firstCon.dir1;
		coord firstDelta = getDelta(firstMove);
		first += firstDelta;
		firstFrom = firstMove;
		part2map[first] = 'P';

		Connectivity secondCon = connectivity[map[second]];
		Direction secondMove = secondCon.dir1 == opposite(secondFrom) ? secondCon.dir2 : secondCon.dir1;
		coord secondDelta = getDelta(secondMove);
		second += secondDelta;
		secondFrom = secondMove;
		part2map[second] = 'P';

		++part1;
	} while (first != second);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	// Part 2. flood-fill the inside and outside of part2map against 'P' (which was filled in during part 1)
	// Can't be sure which is In and which is Out until floodfill hits the border, so use 'L' (left) and 'R' (right) initially
	Direction from = opposite(startCon.dir1);
	coord pos = start;
	int64 leftCount = 0;
	int64 rightCount = 0;
	do
	{
		Connectivity con = connectivity[map[pos]];
		Direction move = con.dir1 == opposite(from) ? con.dir2 : con.dir1;
		coord delta = getDelta(move);
		pos += delta;
		Direction leftSide, rightSide;
		sides(move, leftSide, rightSide);

		coord leftPos = pos;
		leftPos += getDelta(leftSide);
		floodFill(map, part2map, leftPos, 'L', leftCount);
		leftPos -= getDelta(move);
		floodFill(map, part2map, leftPos, 'L', leftCount);

		coord rightPos = pos;
		rightPos += getDelta(rightSide);
		floodFill(map, part2map, rightPos, 'R', rightCount);
		rightPos -= getDelta(move);
		floodFill(map, part2map, rightPos, 'R', rightCount);

		from = move;
	} while (pos != start);
//	printMap(part2map, y);
	assert(map.size() == part2map.size());
	if (part2map[coord(0, 0)] == 'L')
		part2 = rightCount;
	else
		part2 = leftCount;
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 4, 1);
	Process("example2.txt", 8, 1);
	Process("example3.txt", -1, 4);
	Process("example4.txt", -1, 8);
	Process("example5.txt", -1, 10);
	Process("input.txt");

	return 0;
}
//488 < part2 < 497