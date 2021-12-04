// day04.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 4/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Row = std::array<int, 5>;
using Board = std::array<Row, 5>;

void Process(const char* filename)
{
	std::vector<int> numbers;
	std::vector<Board> boards;;
	std::vector<bool> boardFinished;

	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");

	Board readBoard;
	int row = 0;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (numbers.empty())
				{
					char *tok = strtok(thisLine, ",");
					while (tok)
					{
						numbers.push_back(atoi(tok));
						tok = strtok(nullptr, ",");
					}
				}
				else
				{
					int col = 0;
					char *tok = strtok(thisLine, " ");
					while (tok)
					{
						if (*tok)
						{
							readBoard[row][col] = atoi(tok);
							++col;
						}
						tok = strtok(nullptr, " ");
					}
					assert(col == 5);
					++row;
					if (row == 5)
					{
						boards.push_back(readBoard);
						row = 0;
					}
				}
			}
		}
	}
	assert(row == 0);
	fclose(fp);
	delete[] buffer;
	boardFinished.assign(boards.size(), false);

	int winningScore = 0;
	int lastScore = 0;
	bool bWinnerFound = false;
	for (int number : numbers)
	{
		std::vector<bool>::iterator finishedIt = boardFinished.begin();
		for (Board& board : boards)
		{
			if (*finishedIt)
			{
				++finishedIt;
				continue;
			}
			bool bingo = false;
			for (Row& row : board)
			{
				bool bingoRow = true;
				for (int& boardNum : row)
				{
					if (boardNum == number)
					{
						boardNum = 999;
					}
					if (boardNum != 999)
					{
						bingoRow = false;
					}
				}
				bingo |= bingoRow;
			}
			int total = 0;
			for (int testCol = 0; testCol < 5; ++testCol)
			{
				bool bingoCol = true;
				for (Row row : board)
				{
					if (row[testCol] != 999)
					{
						total += row[testCol];
						bingoCol = false;
					}
				}
				bingo |= bingoCol;
			}
			if (bingo)
			{
				*finishedIt = true;
				// calculate score
				int score = total * number;
				lastScore = score;
				if (!bWinnerFound)
				{
					if (score > winningScore)
					{
						winningScore = score;
						bWinnerFound = true;
					}
				}
			}
			++finishedIt;
		}
	}
	printf("%s: Part 1: %d\n", filename, winningScore);
	printf("%s: Part 2: %d\n", filename, lastScore);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
