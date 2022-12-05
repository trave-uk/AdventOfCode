// day05.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 05/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Stack = std::stack<char>;
struct Move
{
	int number;
	int from;
	int to;
};

void Process(const char* filename, int part = -1, std::string expectedResult = "")
{
	std::vector<Stack> stacks;
	std::vector<Move> moves;

	std::string result = "";
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
				if (*thisLine == ' ' || *thisLine == '[')
				{
					// stacks
					if (isdigit(thisLine[1]))
					{
						continue;
					}
					// e.g. "[Z] [M] [P]"
					for (int i = 1, s = 0; i < strlen(thisLine); i += 4, ++s)
					{
						char crate = thisLine[i];
						while (s+1 > stacks.size())
						{
							stacks.push_back(Stack());
						}
						if (isalpha(crate))
						{
							stacks[s].push(crate);
						}
					}
				}
				else if (*thisLine == 'm')
				{
					// moves
					// e.g. "move 3 from 1 to 3"
					Move move;
					int m = sscanf(thisLine, "move %d from %d to %d", &move.number, &move.from, &move.to);
					assert(m == 3);

					moves.push_back(move);
				}
			}
		}
	}
	// all the stacks are upside down (read in top-to-bottom) so invert them.
	for (int i = 0; i < stacks.size(); ++i)
	{
		Stack copy;
		while (!stacks[i].empty())
		{
			copy.push(stacks[i].top());
			stacks[i].pop();
		}
		stacks[i] = copy;
	}
	fclose(fp);
	delete[] buffer;

	for (Move m : moves)
	{
		Stack temp;
		for (int i = 0; i < m.number; ++i)
		{
			assert(!stacks[m.from - 1].empty());
			if (part == 1)
			{
				stacks[m.to - 1].push(stacks[m.from - 1].top());
			}
			else
			{
				assert(part == 2);
				temp.push(stacks[m.from - 1].top());
			}
			stacks[m.from - 1].pop();
		}
		if (part == 2)
		{
			for (int i = 0; i < m.number; ++i)
			{
				assert(!temp.empty());
				stacks[m.to - 1].push(temp.top());
				temp.pop();
			}
		}
	}

	for (Stack s : stacks)
	{
		result.append(1, s.top());
	}

	assert(expectedResult == "" || expectedResult == result);
	printf("%s: Part %d: %s\n", filename, part, result.c_str());
}

int main()
{
	Process("example.txt", 1, "CMZ");
	Process("example.txt", 2, "MCD");
	Process("input.txt", 1);
	Process("input.txt", 2);

	return 0;
}
