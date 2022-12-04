// day03.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 03/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int score(char present)
{
	assert(isalpha(present));
	if (present <= 'Z')
	{
		return present - 'A' + 27;
	}
	else
	{
		return present - 'a' + 1;
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	std::array<bool, 52> elves[3];
	int elf = 0;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// PART 1: find the letter that is in both halves.
				bool found[52] = { false };
				std::array<bool, 52> first;
				std::array<bool, 52> second;
				first.assign(false);
				second.assign(false);

				// PART 2: find the letter that is common to a group of three
				elves[elf].assign(false);

				std::string line(thisLine);
				int index = 0;
				bool done1 = false;
				for (char c : line)
				{
					int s = score(c);
					if (!done1)
					{
						if (index < line.size() / 2)
						{
							first[s - 1] = true;
						}
						else
						{
							if (first[s - 1])
							{
								printf("%c", c);
								part1 += s;
								done1 = true;
							}
							second[s - 1] = true;
						}
					}
					elves[elf][s - 1] = true;

					++index;
				}

			}
		}
		
		++elf;
		if (elf == 3)
		{
			for (int i = 0; i < 52; ++i)
			{
				if (elves[0][i] && elves[1][i] && elves[2][i])
				{
					part2 += i + 1;
					break;
				}
			}
			elf = 0;
		}
	}
	fclose(fp);
	delete[] buffer;
	printf("\n");

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 157, 70);
	Process("input.txt");

	return 0;
}
