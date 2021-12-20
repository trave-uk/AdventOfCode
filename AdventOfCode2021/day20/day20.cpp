// day20.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 20/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::string algorithm;

std::set<coord> map;
coord topLeft;
coord bottomRight;
bool infiniteLit = false;

void Print()
{
	coord pos;
	for (pos.second = topLeft.second; pos.second <= bottomRight.second; ++pos.second)
	{
		for (pos.first = topLeft.first; pos.first <= bottomRight.first; ++pos.first)
		{
			_putch((map.count(pos) > 0) ? '#' : '.');
		}
		_putch('\n');
	}
	_putch('\n');
}

void Step()
{
	int count = 0;
	std::set<coord> newMap;
	coord pos;
	for (pos.second = topLeft.second - 1; pos.second <= bottomRight.second + 1; ++pos.second)
	{
		for (pos.first = topLeft.first - 1; pos.first <= bottomRight.first + 1; ++pos.first)
		{
			coord pos2;
			int accum = 0;
			std::string accum2;
			for (pos2.second = pos.second - 1; pos2.second <= pos.second + 1; ++pos2.second)
			{
				for (pos2.first = pos.first - 1; pos2.first <= pos.first + 1; ++pos2.first)
				{
					bool light = (map.count(pos2) > 0);
					if (!pos2.isContained(topLeft, bottomRight))
						light = infiniteLit;
					accum *= 2;
					accum += light;
					accum2.append(1, light ? '#' : '.');
				}
			}
			if (algorithm[accum] == '#')
			{
				++count;
				newMap.insert(pos);
			}
		}
	}
	map = newMap;

	coord expand(1, 1);
	topLeft -= expand;
	bottomRight += expand;
	if (algorithm[0] == '#' && algorithm[511] == '.')
	{
		infiniteLit = !infiniteLit;
	}
}

void Process(const char* filename)
{
	algorithm.clear();
	map.clear();
	topLeft = coord(0, 0);
	bottomRight = topLeft;
	infiniteLit = false;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord pos;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (algorithm.length() == 0)
				{
					algorithm = thisLine;
				}
				else
				{
					std::string row(thisLine);
					pos.first = 0;
					for (char c : row)
					{
						if (pos.first > bottomRight.first)
							bottomRight.first = pos.first;
						if (pos.second > bottomRight.second)
							bottomRight.second = pos.second;

						if (c == '#')
						{
							map.insert(pos);
						}
						++pos.first;
					}
					++pos.second;
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int i = 0; i < 50; ++i)
	{
		Step();
		if (i == 1)
			printf("%s: Part 1: %lld\n", filename, map.size()); // not 5964, not 5426??
	}
	printf("%s: Part 2: %lld\n", filename, map.size()); // not 5964, not 5426??
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
