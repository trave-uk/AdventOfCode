// day15.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 15/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Tile : char
{
	SPACE = '.',
	WALL = '#',
	BOX = 'O',
	ROBOT = '@',
	LEFTBOX = '[',
	RIGHTBOX = ']'
};

enum Move : char
{
	LEFT = '<',
	RIGHT = '>',
	UP = '^',
	DOWN = 'v'
};

using Map = std::map<coord, Tile>;
using Moves = std::vector<Move>;

int64 Score(const Map& map)
{
	int64 score = 0;
	for (const auto& t : map)
	{
		if (t.second == BOX || t.second == LEFTBOX)
			score += t.first.first + (100l * t.first.second);
	}
	return score;
}

const coord& Direction(const Move& move)
{
	static coord left(-1, 0);
	static coord right(1, 0);
	static coord up(0, -1);
	static coord down(0, 1);

	switch (move)
	{
	case LEFT:
		return left;
	case RIGHT:
		return right;
	case UP:
		return up;
	case DOWN:
		return down;
	default:
		assert(false);
		return left;
	}
}

bool MoveBox(Map& map, const coord& box, const Move& move, bool bJustTest = false)
{
	const coord& dir = Direction(move);
	if (map.at(box) == BOX)
	{
		coord next = box;
		next += dir;
		if (map.count(next))
		{
			if (!MoveBox(map, next, move, bJustTest))
				return false;
		}
		map[next] = BOX;
		map.erase(box);
		return true;
	}
	if (map.at(box) == RIGHTBOX)
	{
		coord leftBox = box;
		--leftBox.first;
		assert(map.at(leftBox) == LEFTBOX);
		return MoveBox(map, leftBox, move, bJustTest);
	}
	if (map.at(box) == WALL)
		return false;
	assert(map.at(box) == LEFTBOX);
	coord leftBox = box;
	coord rightBox = box;
	++rightBox.first;
	assert(map.at(rightBox) == RIGHTBOX);

	switch (move)
	{
	case LEFT:
	{
		coord next = leftBox;
		--next.first;
		if (map.count(next))
		{
			if (!MoveBox(map, next, move))
				return false;
		}
		map[next] = LEFTBOX;
		map[leftBox] = RIGHTBOX;
		map.erase(rightBox);
		return TRUE;
	}
	case RIGHT:
	{
		coord next = rightBox;
		++next.first;
		if (map.count(next))
		{
			if (!MoveBox(map, next, move))
				return false;
		}
		map[next] = RIGHTBOX;
		map[rightBox] = LEFTBOX;
		map.erase(leftBox);
		return true;
	}
	case UP:
	case DOWN:
	{
		coord leftNext = leftBox;
		leftNext += dir;
		coord rightNext = rightBox;
		rightNext += dir;
		if (map.count(leftNext))
		{
			if (!MoveBox(map, leftNext, move, true))
				return false;
		}
		if (map.count(rightNext) && map.at(rightNext) != RIGHTBOX)
		{
			if (!MoveBox(map, rightNext, move, true))
				return false;
		}
		if (!bJustTest)
		{
			if (map.count(leftNext))
			{
				bool moved = MoveBox(map, leftNext, move);
				assert(moved);
			}
			if (map.count(rightNext))
			{
				bool moved = MoveBox(map, rightNext, move);
				assert(moved);
			}
			map[leftNext] = LEFTBOX;
			map[rightNext] = RIGHTBOX;
			map.erase(leftBox);
			map.erase(rightBox);
		}
		return true;
	}
	default:
	{
		assert(false);
		return false;
	}
	}
}

void MoveRobot(Map& map, coord& robot, const Move& move)
{
	const coord& dir = Direction(move);
	coord newPos = robot;
	newPos += dir;
	if (map.count(newPos))
	{
		if (map[newPos] == WALL)
			return;

		if (!MoveBox(map, newPos, move))
			return;
	}
	robot = newPos;
}

void PrintMap(const Map& map, const coord& robot)
{
	int64 width = 0;
	coord pos;
	while (map.count(pos))
	{
		std::string line;
		while (map.count(pos) || pos.first < width)
		{
			Tile t = SPACE;
			if (pos == robot)
			{
				t = ROBOT;
				assert(map.count(pos) == 0);
			}
			else if (map.count(pos))
			{
				t = map.at(pos);
			}
			line.append(1, char(t));

			++pos.first;
			if (pos.first > width)
				width = pos.first;
		}
		printf("%s\n", line.c_str());
		pos.first = 0;
		++pos.second;
	}
}

int64 FollowMoves(const Map& map_, const coord& robot_, const Moves& moves, bool bPrint = false)
{
	short x, y;
	getxy(x, y);
	Map map = map_;
	coord robot = robot_;
	int m = 1;
	for (Move move : moves)
	{
		MoveRobot(map, robot, move);
		if (bPrint)
		{
			gotoxy(x, y);
			printf("Move %d (%c):\n", m, char(move));
			PrintMap(map, robot);
		}
		++m;
	}

	return Score(map);
}

void ExpandMap(Map& map, coord& robot)
{
	Map newMap;
	robot.first *= 2;
	for (auto& t : map)
	{
		coord newPos = t.first;
		newPos.first *= 2;
		Tile left, right;
		if (t.second == WALL)
		{
			left = WALL;
			right = WALL;
		}
		else
		{
			assert(t.second == BOX);
			left = LEFTBOX;
			right = RIGHTBOX;
		}
		newMap[newPos] = left;
		++newPos.first;
		newMap[newPos] = right;
	}
	map = newMap;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1, bool bPrint = false)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");

	Map map;
	Moves moves;
	coord robot;

	coord pos;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				if (line[0] == '#')
				{
					// ##@.O..#
					const int64 lineLength = line.length();
					for (pos.first = 0; pos.first < lineLength; ++pos.first)
					{
						Tile t = Tile(line[pos.first]);
						if (t == WALL || t == BOX)
						{
							map.insert(std::make_pair(pos, t));
						}
						else if (t == ROBOT)
						{
							robot = pos;
						}
						else
						{
							assert(t == SPACE);
						}
					}
					++pos.second;
				}
				else
				{
					// <^^>>>vv<v>>v<<
					for (char c : line)
					{
						Move m = Move(c);
						moves.push_back(m);
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	if (expectedPart1 != -2)
	{
		int64 part1 = FollowMoves(map, robot, moves, bPrint);

		assert(expectedPart1 == -1 || expectedPart1 == part1);
		printf("%s: Part 1: %lld\n", filename, part1);
	}

	if (expectedPart2 != -2)
	{
		ExpandMap(map, robot);
		int64 part2 = FollowMoves(map, robot, moves, bPrint);

		assert(expectedPart2 == -1 || expectedPart2 == part2);
		printf("%s: Part 2: %lld\n", filename, part2);
	}
}

int main()
{
	Process("example1.txt", 2028, -2, true);
	Process("example3.txt", -2, -1, true);
	Process("example2.txt", 10092, 9021, true);
	Process("input.txt");

	return 0;
}
