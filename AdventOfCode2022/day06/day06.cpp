// day06.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 06/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int part, int64 expectedResult = -1)
{
	const int size = (part == 1) ? 4 : 14;
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
				for (int i = 0; i <= strlen(thisLine) - size; ++i)
				{
					bool notFound = false;
					for (int j = 1; j < size; ++j)
					{
						for (int k = 0; k < j; ++k)
						{
							if (thisLine[i + j] == thisLine[i + k])
								notFound = true;
						}
					}
					if (!notFound)
					{
						result = i + size;
						break;
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedResult == -1 || expectedResult == result);
	printf("%s: Part %d: %lld\n", filename, part, result);
}

int main()
{
	Process("example1.txt", 1, 7);
	Process("example2.txt", 1, 5);
	Process("example3.txt", 1, 6);
	Process("example4.txt", 1, 10);
	Process("example5.txt", 1, 11);
	Process("input.txt", 1);
	Process("input.txt", 2);

	return 0;
}
