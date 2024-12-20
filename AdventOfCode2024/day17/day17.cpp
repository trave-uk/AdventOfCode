// day17.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 17/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Output = std::vector<int64>;

enum Instruction
{
	adv = 0,
	bxl = 1,
	bst = 2,
	jnz = 3,
	bxc = 4,
	out = 5,
	bdv = 6,
	cdv = 7
};

struct Computer
{
	int64 a, b, c;
	std::vector<int64> program;

	int64 Combo(int64 value)
	{
		if (value < 4)
			return value;
		else if (value == 4)
			return a;
		else if (value == 5)
			return b;
		else if (value == 6)
			return c;
		assert(false);
		return -1;
	}
	void Run(Output& output)
	{
		int64 pc = 0;
		const int64 programSize = program.size();
		while (pc < programSize)
		{
			Instruction op = static_cast<Instruction>(program[pc++]);
			int64 operand = program[pc++];
			switch (op)
			{
			case adv:
			{
				a = a >> Combo(operand);
				break;
			}
			case bxl:
			{
				b = b ^ operand;
				break;
			}
			case bst:
			{
				b = Combo(operand) & 7;
				break;
			}
			case jnz:
			{
				if (a)
					pc = operand;
				break;
			}
			case out:
			{
				output.push_back(Combo(operand) & 7);
				break;
			}
			case bxc:
			{
				b ^= c;
				break;
			}
			case bdv:
			{
				b = a >> Combo(operand);
				break;
			}
			case cdv:
			{
				c = a >> Combo(operand);
				break;
			}
			default:
				assert(false);
			}
		}
	}

	bool TestPart2(std::vector<int64> test)
	{
		std::vector<int64> output;
		Run(output);
		return test == output;
	}
};

int64 CalculatePart2(const Output& program)
{
	// Taking the output one at a time in reverse, shift each candidate and set the bottom 3 bits to all possibilities (0-7) and see which ones produce the correct result
	Output reversedProgram = program;
	std::reverse(reversedProgram.begin(), reversedProgram.end());
	std::set<int64> candidates;
	candidates.insert(0);
	Output tail;
	for (int64 next : reversedProgram)
	{
		assert(next >= 0 && next <= 7);
		// tail contains the output from here to the end
		std::reverse(tail.begin(), tail.end());
		tail.push_back(next);
		std::reverse(tail.begin(), tail.end());

		std::set<int64> nextCandidates;
		// For each candidate, shift left by 3 and try all possibilities for the bottom three bits
		for (const int64 a : candidates)
		{
			int64 shiftedA = a << 3;
			for (int i = 0; i <= 7; ++i)
			{
				int64 nextA = shiftedA | i;
				Computer c;
				c.a = nextA;
				c.program = program;
				if (c.TestPart2(tail))
				{
					nextCandidates.insert(nextA);
				}
			}
		}
		assert (nextCandidates.size() > 0);
		candidates = nextCandidates;
	}
	printf("Found %lld candidate values:\n", candidates.size());
	for (int64 candidate : candidates)
	{
		printf(" %lld\n", candidate);
	}
	// The lowest value will be the first returned for the set by begin()
	return *candidates.begin();
}

void Process(const char* filename, std::string expectedPart1 = "", int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	std::string part1 = "";
	int64 part2 = 0;
	Computer computer;
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
				if (line[0] == 'R')
				{
					// Register A: 729
					int64 value = atoll(thisLine + 11);
					switch (line[9])
					{
					case 'A':
						computer.a = value; break;
					case 'B':
						computer.b = value; break;
					case 'C':
						computer.c = value; break;
					default:
						assert(false);
					}
				}
				else
				{
					// Program: 0,1,5,4,3,0
					assert(line[0] == 'P');
					char* tok = strtok(thisLine + 9, ",");
					while (tok)
					{
						computer.program.push_back(atoi(tok));
						tok = strtok(nullptr, ",");
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	Output output;
	Computer c = computer;
	c.Run(output);
	for (int64 v : output)
	{
		if (part1.length() == 0)
			part1 += string_format("%d", v);
		else
			part1 += string_format(",%d", v);
	}
	assert(expectedPart1 == "" || expectedPart1 == part1);
	printf("%s: Part 1: %s\n", filename, part1.c_str());

	if (expectedPart2 == -2)
		return;

	part2 = CalculatePart2(computer.program);
	c.a = part2;
	assert(c.TestPart2(c.program));

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example1.txt", "4,6,3,5,6,3,5,2,1,0", -2);
	Process("example2.txt", "", 117440);
	Process("input.txt");

	return 0;
}
