// day22.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 22/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Space = std::set<coord3>;

struct Brick
{
	Brick(char* input)
	{
		char* s = strtok(input, "~");
		char* e = strtok(nullptr, "~");
		start = coord3(s);
		end = coord3(e);
		if (start < end)
		{
			std::swap(start, end);
		}
	}

	~Brick()
	{
	}

	coord3 start;
	coord3 end;

	bool operator<(const Brick& other) const
	{
		if (end < other.end)
			return true;
		else if (end > other.end)
			return false;
		else if (start < other.start)
			return true;
		assert(start >= other.start);
		return false;
	}

	void operator-=(const coord3& by)
	{
		start -= by;
		end -= by;
	}

	void operator+=(const coord3& by)
	{
		start += by;
		end += by;
	}

	void AddToSpace(Space& space) const
	{
		coord3 delta = start;
		delta -= end;
		int64 length = delta.Length();
		if (!length)
			delta = coord3(1, 0, 0);
		else
			delta /= length;
		for (coord3 pos(end); pos <= start; pos += delta)
		{
			assert(!space.count(pos));
			space.insert(pos);
		}
	}

	bool HasSpace(Space& space) const
	{
		coord3 delta = start;
		delta -= end;
		int64 length = delta.Length();
		if (!length)
			delta = coord3(1, 0, 0);
		else
			delta /= length;
		for (coord3 pos(end); pos <= start; pos += delta)
		{
			if (space.count(pos))
				return false;
		}
		return true;
	}

	void RemoveFromSpace(Space& space) const
	{
		coord3 delta = start;
		delta -= end;
		int64 length = delta.Length();
		if (!length)
			delta = coord3(1, 0, 0);
		else
			delta /= length;
		for (coord3 pos(end); pos <= start; pos += delta)
		{
			assert(space.count(pos));
			space.erase(pos);
		}
	}
};

using Bricks = std::set<Brick>;

bool ApplyGravity(Bricks& bricks, Space& space, int64* bricksMoved = nullptr)
{
	bool moved = false;
	Bricks newBricks;
	int64 brickIndex = 0;
	for (const Brick& b : bricks)
	{
		b.RemoveFromSpace(space);
		Brick test(b);
		coord3 down(0, 0, -1);
		bool brickMoved = false;
		for (;;)
		{
			Brick test2(test);
			test2 += down;
			if (!test2.HasSpace(space) || test2.end.get<2>() < 0)
			{
				break;
			}
			else
			{
				brickMoved = true;
				test = test2;
			}
		}
		if (brickMoved)
		{
			if (bricksMoved)
				++(*bricksMoved);
			moved = brickMoved;
		}
		test.AddToSpace(space);
		assert(!newBricks.count(test));
		newBricks.insert(test);
		++brickIndex;
	}
	bricks = newBricks;
	return moved;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Space space;
	Bricks bricks;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// 1,0,1~1,2,1
				Brick brick(thisLine);
				bricks.insert(brick);
				brick.AddToSpace(space);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// First move all bricks down as far as they can go.
	// bricks should be sorted bottom-up, as operator< sorts on z first
	ApplyGravity(bricks, space);

	// Now try removing each brick in turn and see whether any move when gravity is applied.
	for (const Brick& b : bricks)
	{
		Bricks testBricks = bricks;
		Space testSpace = space;
		testBricks.erase(b);
		b.RemoveFromSpace(testSpace);
		int64 bricksMoved = 0;
		if (!ApplyGravity(testBricks, testSpace, &bricksMoved))
			++part1;
		else
			part2 += bricksMoved;
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 5, 7);
	Process("input.txt");

	return 0;
}
