// day08.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 08/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"
#include "../primes.h"

struct Instruction
{
	// node = (left, right)
	Instruction(const char* thisLine = "N = (L, R)")
	{
		char n[10], l[10], r[10];
		int x = sscanf(thisLine, "%s = (%[A-Z0-9], %[A-Z0-9])", n, l, r);
		assert(x == 3);
		node = std::string(n);
		left = std::string(l);
		right = std::string(r);
	}
	std::string node;
	std::string left;
	std::string right;
};

using InstructionMap = std::map<std::string, Instruction>;

int64 FollowInstructions(std::string node, std::string target, std::string moves, InstructionMap& instructions)
{
	int64 count = 0;
	while (true)
	{
		for (char move : moves)
		{
			++count;
			const Instruction& i = instructions[node];
			if (move == 'L')
				node = i.left;
			else
			{
				assert(move == 'R');
				node = i.right;
			}
			if (node == target)
				return count;
		}
	}
}

int64 FindPattern(const std::vector<std::vector<std::pair<std::string, int64>>>& periods)
{
	// Get plenty of samples, and then check for patterns. If each set of periods recurs on a set schedule then we can figure it out
	std::vector<int64> pattern;
	for (const auto& x : periods)
	{
		int64 previous = 0;
		int64 previousDiff = 0;
		bool found = false;
		for (const auto& y : x)
		{
			int64 diff = y.second - previous;
			if (diff == previousDiff)
			{
				if (!found)
				{
					found = true;
					printf("%lld + n * %lld\n", previous, diff);
					assert(previous == diff);
					pattern.push_back(diff);
				}
			}
			else if (found)
			{
				printf("Broken");
				break;
			}
			previousDiff = diff;
			previous = y.second;
		}
	}
	// Now, get all prime factors of these numbers
	std::vector<int64> primeFactors;
	for (int64 prime : primes)
	{
		if (prime == 1)
			continue;
		int found = 0;
		int64 todo = pattern.size();
		do
		{
			found = 0;
			todo = pattern.size();
			for (int64& p : pattern)
			{
				if (p % prime == 0)
				{
					++found;
					p /= prime;
				}
				if (p == 1)
					--todo;
			}
			if (found)
			{
				primeFactors.push_back(prime);
			}
			if (!todo)
			{
				break;
			}
		} while (found > 0);
		if (!todo)
		{
			break;
		}
	}
	// Finally, multiply out the prime factors
	int64 result = 1;
	for (int64 p : primeFactors)
	{
		result *= p;
	}
	return result;
}

int64 FollowInstructions2(std::vector<std::string> nodes, std::string moves, InstructionMap& instructions)
{
	int64 count = 0;
	int samples = 0;
	std::vector<std::vector<std::pair<std::string,int64>>> periods;
	periods.assign(nodes.size(), std::vector<std::pair<std::string, int64>>());
	while (true)
	{
		for (char move : moves)
		{
			++count;
			bool finished = true;
			int index = 0;
			for (std::string& node : nodes)
			{
				const Instruction& i = instructions[node];
				if (move == 'L')
					node = i.left;
				else
				{
					assert(move == 'R');
					node = i.right;
				}
				if (node.back() == 'Z')
				{
					periods[index].push_back(std::make_pair(node, count));
					++samples;
					if (samples == 200)
					{
						return FindPattern(periods);
					}
				}
				else if (node.back() != 'Z')
					finished = false;
				++index;
			}
			if (finished)
			{
				return count;
			}
		}
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	InstructionMap instructions;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::string moves;
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
				if (moves.length() == 0)
					moves = line;
				else
				{
					Instruction i(thisLine);
					instructions[i.node] = i;
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	if (expectedPart1 != -2)
	{
		part1 = FollowInstructions("AAA", "ZZZ", moves, instructions);
		assert(expectedPart1 == -1 || expectedPart1 == part1);
		printf("%s: Part 1: %lld\n", filename, part1);
	}

	std::vector<std::string> nodes;
	for (const auto &myPair: instructions)
	{
		const std::string& node = myPair.first;
		if (node.back() == 'A')
			nodes.push_back(node);
	}
	part2 = FollowInstructions2(nodes, moves, instructions);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 2);
	Process("example2.txt", 6);
	Process("example3.txt", -2, 6);
	Process("input.txt");

	return 0;
}
