// day17.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 17/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// Part 2 thoughts
// ~~~~~~~~~~~~~~~
// 
// There must be a pattern as there are a finite number of rock shapes and a repeating pattern of wind.
// The input data has 10091 gusts before the wind repeats. The example data only has 40.
// For the example data, the data must repeat much earlier than for the input data.
// So... Could calculate where this repeats, but perhaps better to run a few rocks (1000) to get above the floor, and then
// to store the shape index and wind index on spawning a new rock, and to see where that combination repeats.
// I will sleep on this so that I don't pass out cycling home this morning,
// Right. Step one is to get the same result in part 1 using this method for the example data.


// Rocks in this order:

// ####

// .#.
// ###
// .#.

// ..#
// ..#
// ###

// #
// #
// #
// #

// ##
// ##

// "The tall, vertical chamber is exactly seven units wide. Each rock appears so that its left edge is two units away from the
// left wall and its bottom edge is three units above the highest rock in the room (or the floor, if there isn't one)."

struct RockShape : public std::array<std::array<char, 4>, 4>
{
	RockShape(std::string pattern)
	{
		int x = 0, y = 0;
		width = 0;
		for (char c : pattern)
		{
			if (c == '\n')
			{
				y++;
				assert(width == 0 || width == x);
				width = x;
				x = 0;
			}
			else
			{
				(*this)[y][x] = c;
				x++;
			}
		}
		assert(width == 0 || width == x);
		width = x;
		height = y + 1;
	}
	int width;
	int height;
};

struct Rock
{
	Rock() = default;
	Rock(const RockShape& s, int height )
		: shape(s)
		, x(3) // left wall is at x=0
		, y(height + 3)
	{}
	const RockShape& shape;
	int x;
	int y;
};

// Chamber is stored upside-down (first entry is the floor)
// The vector of strings only contains the rocks after they've settled
struct Chamber : std::vector<std::string>
{
	const std::string walls = "|       |";
	const std::string floor = "+-------+";

	const std::array<RockShape, 5> rockShapes =
	{
		RockShape("####"),
		RockShape(".#.\n###\n.#."),
		RockShape("###\n..#\n..#"),
		RockShape("#\n#\n#\n#"),
		RockShape("##\n##")
	};

	Chamber(std::string _wind, int64 _rocks)
		: wind(_wind)
		, rocks(_rocks)
		, nextRock(0)
	{
		push_back(floor);
	}

	// y=0 is the floor. (x,y) is the position of the bottom-left tile in the rock
	bool Collide(const Rock& rock, int dx, int dy)
	{
		for (int y = 0; y < rock.shape.height; ++y)
		{
			int Y = rock.y + y + dy;
			const std::string& row = (Y >= size()) ? walls : (*this)[Y];
				
			for (int x = 0; x < rock.shape.width; ++x)
			{
				int X = rock.x + x + dx;
				if (rock.shape[y][x] == '#' && row[X] != ' ')
				{
					return true;
				}
			}
		}
		return false;
	}

	void Embed(const Rock& rock)
	{
		// make enough space for this rock
		while ((rock.y + rock.shape.height) > size())
		{
			push_back(walls);
		}
		for (int y = 0; y < rock.shape.height; ++y)
		{
			for (int x = 0; x < rock.shape.width; ++x)
			{
				if (rock.shape[y][x] == '#')
				{
					assert((*this)[rock.y + y][rock.x + x] == ' ');
					(*this)[rock.y + y][rock.x + x] = '#';
				}
			}
		}
	}

	int NextWind()
	{
		char d = wind[windIndex++];
		assert(d == '<' || d == '>');
		if (windIndex >= wind.size())
		{
			windIndex = 0;
		}
		return (d == '<') ? -1 : +1;
	}

	const RockShape& NextRockShape()
	{
		int index = nextRock++;
		if (nextRock >= rockShapes.size())
		{
			nextRock = 0;
		}
		return rockShapes[index];
	}

	int64 Process()
	{
		int snapshotWindIndex = -1, snapshotRockIndex = -1;
		int64 lastHeight = -1;
		int64 lastRock = -1;
		int64 lastDiff = -1;
		int64 rocksDiff = -1;
		int sameCount = 0;
		int64 heightToAdd = 0;
		int64 windLength = wind.length();
		for (int64 r = 0; r < rocks; ++r)
		{
			int64 height = size() - 1;
			if (r == windLength)
			{
				// arbitrary time to assume rocks have settled and start looking for a pattern
				snapshotWindIndex = windIndex;
				snapshotRockIndex = nextRock;
				lastHeight = height;
				lastRock = r;
			}
			else if (r > windLength && snapshotWindIndex == windIndex && snapshotRockIndex == nextRock)
			{
				int64 diff = height - lastHeight;
				int64 rdiff = r - lastRock;
				lastHeight = height;
				lastRock = r;
				if (diff == lastDiff && rdiff == rocksDiff)
				{
					++sameCount;
					if (sameCount == 10)
					{
						// same diff 10 times in a row. This must be the pattern!
						assert(heightToAdd == 0);
						int64 remainingRocks = rocks - r;
						heightToAdd = remainingRocks / rocksDiff;
						heightToAdd *= diff;
						remainingRocks %= rocksDiff;
						rocks = r + remainingRocks;
					}
				}
				else
				{
					sameCount = 0;
				}
				lastDiff = diff;
				rocksDiff = rdiff;
			}
			Rock rock(NextRockShape(), size());
			for(;;)
			{
				// move with wind
				int dx = NextWind();
				if (!Collide(rock, dx, 0))
				{
					rock.x += dx;
				}
				// move down
				if (Collide(rock, 0, -1))
				{
					Embed(rock);
					break;
				}
				else
				{
					rock.y--;
				}
			}
		}
		return size() - 1 + heightToAdd;
	}

	std::string wind;
	int windIndex = 0;
	int64 rocks;
	int nextRock;
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
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
				Chamber chamber(thisLine, 2022);
				part1 = chamber.Process();

				Chamber chamber2(thisLine, 1000000000000);
				part2 = chamber2.Process();
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	if (expectedPart1 != -1)
	{
		printf("%s: Part 1 expected: %lld\n", filename, expectedPart1);
	}
	printf("%s: Part 1: %lld\n", filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	if (expectedPart2 != -1)
	{
		printf("%s: Part 2 expected: %lld\n", filename, expectedPart2);
	}
	printf("%s: Part 2: %lld\n", filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 3068, 1514285714288L);
	Process("input.txt");

	return 0;
}
