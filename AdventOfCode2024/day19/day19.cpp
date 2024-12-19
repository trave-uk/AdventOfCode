// day19.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 19/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Colour : char
{
	WHITE = 'w',
	BLUE = 'u',
	BLACK = 'b',
	RED = 'r',
	GREEN = 'g'
};

struct Towel
{
	Towel(std::string input)
	{
		for (const char c : input)
		{
			colours.push_back(static_cast<Colour>(c));
		}
	}
	std::vector<Colour> colours;
};
using Towels = std::vector<Towel>;

struct Design
{
	Design(std::string input)
	{
		for (const char c : input)
		{
			colours.push_back(static_cast<Colour>(c));
			knownValid.push_back(-1);
		}
	}

	bool Contains(const Towel& towel, int64 offset) const
	{
		if (towel.colours.size() > colours.size() - offset)
			return false;
		for (int64 i = 0; i < towel.colours.size(); ++i)
		{
			if (towel.colours[i] != colours[offset+i])
				return false;
		}
		return true;
	}

	const char* c_str() const
	{
		static char buffer[1024];
		int i = 0;
		for (Colour c : colours)
		{
			buffer[i++] = static_cast<char>(c);
		}
		buffer[i] = '\0';
		return buffer;
	}
	std::vector<Colour> colours;

	// knownValid caches whether we already know the validity of the design from any given offset
	// -1 = unknown, 0 = valid, +ve = number of valid combinations (at this offset)
	mutable std::vector<int64> knownValid;
};
using Designs = std::vector<Design>;

int64 PossibleDesign(const Design& design, const Towels& towels, const int64 offset = 0)
{
	int64& count = design.knownValid[offset];
	if (count != -1)
	{
		return count;
	}
	count = 0;
	for (const Towel& towel : towels)
	{
		if (design.Contains(towel, offset))
		{
			int64 newOffset = offset + towel.colours.size();
			if (design.colours.size() == newOffset)
			{
				++count;
			}
			else 
			{
				int64 newCount = PossibleDesign(design, towels, newOffset);
				count += newCount;
			}
		}
	}
	return count;
}

std::pair<int64, int64> CountPossibleDesigns(const Designs& designs, const Towels& towels, bool bAllPossible = false)
{
	int64 part1 = 0;
	int64 part2 = 0;

	for (const Design& design : designs)
	{
		int64 possible = PossibleDesign(design, towels);
		if (possible)
		{
			++part1;
			part2 += possible;
		}
	}

	return std::make_pair(part1, part2);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Towels towels;
	Designs designs;

	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
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
				// r, wr, b, g, bwu, rb, gb, br
				if (towels.size() == 0)
				{
					char* tok = strtok(thisLine, ", ");
					while (tok)
					{
						towels.push_back(Towel(tok));
						tok = strtok(nullptr, ", ");
					}
				}
				else
				{
					designs.push_back(Design(line));
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	auto pair = CountPossibleDesigns(designs, towels);
	int64 part1 = pair.first;
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	int64 part2 = pair.second;
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 6, 16);
	Process("input.txt");

	return 0;
}
