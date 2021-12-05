// day05.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 5/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct line
{
	coord start;
	coord end;
};

void Process(const char* filename, int part)
{
	std::vector<line> lines;
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
				// 0,9 -> 5,9
				line l;
				int matches = sscanf(thisLine, "%lld,%lld -> %lld,%lld", &l.start.first, &l.start.second, &l.end.first, &l.end.second);
				assert(matches == 4);
				lines.push_back(l);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// For part 1, only conside vertical and horizontal lines
	int result = 0;
	std::map<coord, int> grid;
	for (const line& l : lines)
	{
		if (part == 1 && l.start.first != l.end.first && l.start.second != l.end.second)
		{
			// skip this line, it's diagonal
			continue;
		}
		coord v = l.end;
		v -= l.start;
		int64 length = v.length();
		v /= length;
		coord pos = l.start;
		for (int64 i = 0; i <= length; ++i)
		{
			grid[pos]++;
			if (grid[pos] == 2)
			{
				++result;
			}
			pos += v;
		}
	}
	printf("%s: Part %d: %d\n", filename, part, result);
}

int main()
{
	Process("example.txt", 1);
	Process("input.txt", 1);

	Process("example.txt", 2);
	Process("input.txt", 2);

	return 0;
}
