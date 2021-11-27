// day19.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 19/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename)
{
	int result = 0;
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

			}
		}
	}
	fclose(fp);
	delete[] buffer;

	printf("%s: Part 1: %d\n", filename, result);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
