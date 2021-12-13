// day13.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 13/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	std::set<coord> dots;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 part1 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (isdigit(*thisLine))
				{
					coord pos;
					int matches = sscanf(thisLine, "%lld,%lld", &pos.first, &pos.second);
					assert(matches == 2);
					dots.insert(pos);
				}
				else
				{
					char axis;
					int64 at;
					int matches = sscanf(thisLine, "fold along %c=%lld", &axis, &at);
					assert(matches == 2);
					std::vector<std::pair<coord, coord>> moves;
					for (coord pos : dots)
					{
						coord newPos = pos;
						switch (axis)
						{
						case 'x':
						{
							if (pos.first > at)
							{
								newPos.first = at - (pos.first - at);
							}
							break;
						}
						case'y':
						{
							if (pos.second > at)
							{
								newPos.second = at - (pos.second - at);
							}
							break;
						}
						}
						if (pos != newPos)
						{
							moves.push_back(std::make_pair(pos, newPos));
						}
					}
					for (auto move : moves)
					{
						dots.erase(move.first);
						dots.insert(move.second);
					}
					if (!part1)
					{
						part1 = dots.size();
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	printf("%s: Part 1: %lld\n", filename, part1);

	printf("%s: Part 2:\n", filename);
	coord bottomRight;
	for (coord pos : dots)
	{
		if (pos.first > bottomRight.first)
			bottomRight.first = pos.first;
		if (pos.second > bottomRight.second)
			bottomRight.second = pos.second;
	}

	coord pos;
	for (pos.second = 0; pos.second <= bottomRight.second; ++pos.second)
	{
		for (pos.first = 0; pos.first <= bottomRight.first; ++pos.first)
		{
			if (dots.count(pos))
				_putch('#');
			else
				_putch(' ');
		}
		_putch('\n');
	}
	_putch('\n');
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
