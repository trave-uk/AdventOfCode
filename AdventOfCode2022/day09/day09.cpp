// day09.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 09/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Coord = std::pair<int64, int64>;

using Cloud = std::set<Coord>;

void Catch(Coord& T, const Coord& H)
{
	int64 move1 = H.first - T.first;
	int64 move2 = H.second - T.second;
	if (abs(move1) > 1 || abs(move2) > 1)
	{
		T.first += sgn(move1);
		T.second += sgn(move2);
	}
}

void Process(const char* filename, int part, int64 expectedResult = -1)
{
	int RopeLength = (part == 1) ? 2 : 10;
	std::vector<Coord> Rope;
	Rope.assign(RopeLength, Coord());
	Coord& H = Rope[0];
	Coord& T = Rope[RopeLength - 1];
	Cloud Been;
	Been.insert(Rope[0]);
	int64 result = 0;
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
				char dir;
				int dist;
				int m = sscanf(thisLine, "%c %d", &dir, &dist);
				 
				for (int i = 0; i < dist; ++i)
				{
					switch (dir)
					{
					case 'R':
						++H.first;
						break;
					case 'L':
						--H.first;
						break;
					case 'U':
						--H.second;
						break;
					case 'D':
						++H.second;
						break;
					}
					for (int j = 0; j < RopeLength - 1; ++j)
					{
						Catch(Rope[j+1], Rope[j]);
					}
					Been.insert(T);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	result = Been.size();
	assert(expectedResult == -1 || expectedResult == result);
	printf("%s: Part %d: %lld\n", filename, part, result);
}

int main()
{
	Process("example1.txt", 1, 13);
	Process("input.txt", 1);

	Process("example2.txt", 2, 36);
	Process("input.txt", 2);

	return 0;
}
