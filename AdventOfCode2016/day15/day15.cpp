// day15.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

class Disc
{
public:
	Disc(std::string input)
	{
		// e.g.:
		// Disc #1 has 5 positions; at time=0, it is at position 4.
		if (sscanf(input.c_str(), "Disc #%d has %d positions; at time=0, it is at position %d.", &index, &size, &position) != 3)
		{
			assert(false);
		}
	}

	Disc(int _index, int _size, int _position)
		: index(_index)
		, size(_size)
		, position(_position)
	{}

	void tick()
	{
		++position;
		position %= size;
	}

	int index;
	int size;
	int position;

	bool matchesStart(int start) const
	{
		return ((start + index + position) % size) == 0;
	}
};

void process(std::vector<Disc> &discs)
{
	for (Disc &d : discs)
	{
		d.tick();
	}
}

bool checkStart(int start, const std::vector<Disc> &discs)
{
	for (const Disc &d : discs)
	{
		if (!d.matchesStart(start))
		{
			return false;
		}
	}
}

int main()
{
	std::vector<Disc> discs;
	int result = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				Disc d(thisLine);
				discs.push_back(d);
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int part1;
	for (part1 = 0; !checkStart(part1, discs); ++part1)
	{
	}
	printf( "Part 1: %d\n", part1 );

	Disc d(discs.size() + 1, 11, 0);
	discs.push_back(d);
	int part2;
	for (part2 = 0; !checkStart(part2, discs); ++part2)
	{
	}
	printf("Part 2: %d\n", part2);

	return 0;
}
