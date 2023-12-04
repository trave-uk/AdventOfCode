// day04.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 04/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<int> extraCards;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				int card = -1;
				// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
				char* tok = strtok(thisLine, ":");
				int result = sscanf(tok, "Card %d", &card);
				assert(result == 1);
				char* winning = strtok(nullptr, "|");
				char* numbers = strtok(nullptr, "|");
				std::set<int> winningNumbers;
				std::set<int> thisHand;
				tok = strtok(winning, " ");
				while (tok)
				{
					if (*tok)
						winningNumbers.insert(atoi(tok));
					tok = strtok(nullptr, " ");
				}
				tok = strtok(numbers, " ");
				while (tok)
				{
					if (*tok)
						thisHand.insert(atoi(tok));
					tok = strtok(nullptr, " ");
				}

				std::vector<int> intersect;
				std::set_intersection(winningNumbers.begin(), winningNumbers.end(), thisHand.begin(), thisHand.end(),
					std::back_inserter(intersect));
				int64 score = intersect.size();
				if (score > 0)
					part1 += pow(2, score - 1);

				// part 2
				if (score > 0)
				{
					int firstCard = card + 1;
					int lastCard = card + score;
					while (extraCards.size() < lastCard + 1)
						extraCards.push_back(0);
					for (int c = firstCard; c <= lastCard; ++c)
						extraCards[c] += (1 + extraCards[card]);
				}
				while (extraCards.size() < card + 1)
					extraCards.push_back(0);
				part2 += (1 + extraCards[card]);
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
	Process("example.txt", 13, 30);
	Process("input.txt");

	return 0;
}
