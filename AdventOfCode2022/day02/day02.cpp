// day02.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 02/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int winner[3] = { 1, 2, 0 };

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
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
				// Each line contains "[ABC] [XYZ]". A/X = Rock, B/Y = Paper, C/Z= Scissors
				int player1 = thisLine[0] - 'A';
				int player2 = thisLine[2] - 'X';
				assert(player1 >= 0 && player1 <= 2 && player2 >= 0 && player2 <= 2);
				part1 += player2 + 1;
				if (player1 == player2)
					part1 += 3;
				else if (player2 == winner[player1])
					part1 += 6;
				else
				{
					assert(player1 == winner[player2]);
				}
				// Part 2: ABC as before, X = lose, Y = draw, Z = win
				int diff = thisLine[2] - 'Y';
				player2 = (player1 + diff + 3) % 3;

				assert(player1 >= 0 && player1 <= 2 && player2 >= 0 && player2 <= 2);
				part2 += player2 + 1;
				if (player1 == player2)
					part2 += 3;
				else if (player2 == winner[player1])
					part2 += 6;
				else
				{
					assert(player1 == winner[player2]);
				}
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
	Process("example.txt", 15, 12);
	Process("input.txt");

	return 0;
}
