// day02.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 02/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// only 12 red cubes, 13 green cubes, and 14 blue cubes

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				char* context;
				char* tok = strtok_s(thisLine, ":", &context);
				int64 gameid = 0;
				int result = sscanf(tok, "Game %lld", &gameid);
				assert(result == 1);
				tok = strtok_s(nullptr, ";", &context);
				bool possible = true;
				int minred = 0;
				int minblue = 0;
				int mingreen = 0;
				while (tok)
				{
					int red = 0, green = 0, blue = 0;
					char* context2;
					char* tok2 = strtok_s(tok, ",", &context2);
					while (tok2)
					{
						int amount;
						char col[10];
						result = sscanf(tok2, "%d %s", &amount, col);
						assert(result == 2);

						std::string colour(col);
						if (colour == "red")
							red = amount;
						else if (colour == "green")
							green = amount;
						else if (colour == "blue")
							blue = amount;
						tok2 = strtok_s(nullptr, ",", &context2);
					}

					if (red > 12 || green > 13 || blue > 14)
					{
						possible = false;
					}

					if (red > minred)
						minred = red;
					if (blue > minblue)
						minblue = blue;
					if (green > mingreen)
						mingreen = green;
					tok = strtok_s(nullptr, ";", &context);
				}
				if (possible)
					part1 += gameid;
				part2 += minred * minblue * mingreen;

			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 8, 2286);
	Process("input.txt");

	return 0;
}
