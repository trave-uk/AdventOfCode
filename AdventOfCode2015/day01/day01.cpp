// day01.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int part2 = -1;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				std::string line(thisLine);
				int index = 0;
				for (char c : line)
				{
					switch (c)
					{
					case '(':
						++pos;
						break;
					case ')':
						--pos;
						if (pos == -1 && part2 == -1)
						{
							part2 = index + 1;
						}
						break;
					default:
						assert(false);
						break;
					}
					++index;
				}
				printf("%s: Part 1: %d\n", filename, pos);
				printf("%s: Part 2: %d\n", filename, part2);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
