// day16.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 13/03/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Sue
{
	Sue() = default;
	Sue(char* input)
	{
		// Sue 1: goldfish: 9, cars: 0, samoyeds: 9

		char* tok = strtok(input, ":");
		int found = sscanf(tok, "Sue %d", &index);
		assert(found == 1);
		bool bPossiblePart1 = true;
		bool bPossiblePart2 = true;
		while (tok = strtok(nullptr, ","))
		{
			char nam[20];
			int amount;
			found = sscanf(tok, " %[^:]: %d", nam, &amount);
			assert(found == 2);
			std::string name = nam;
			if (name == "children")
			{
				children = amount;
				if (children != 3)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else if (name == "cats")
			{
				cats = amount;
				if (cats != 7)
					bPossiblePart1 = false;
				if (cats <= 7)
					bPossiblePart2 = false;
			}
			else if (name == "samoyeds")
			{
				samoyeds = amount;
				if (samoyeds != 2)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else if (name == "pomeranians")
			{
				pomeranians = amount;
				if (pomeranians != 3)
					bPossiblePart1 = false;
				if (pomeranians >= 3)
					bPossiblePart2 = false;
			}
			else if (name == "akitas")
			{
				akitas = amount;
				if (akitas != 0)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else if (name == "vizslas")
			{
				vizslas = amount;
				if (vizslas != 0)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else if (name == "goldfish")
			{
				goldfish = amount;
				if (goldfish != 5)
					bPossiblePart1 = false;
				if (goldfish >= 5)
					bPossiblePart2 = false;
			}
			else if (name == "trees")
			{
				trees = amount;
				if (trees != 3)
					bPossiblePart1 = false;
				if (trees <= 3)
					bPossiblePart2 = false;
			}
			else if (name == "cars")
			{
				cars = amount;
				if (cars != 2)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else if (name == "perfumes")
			{
				perfumes = amount;
				if (perfumes != 1)
					bPossiblePart1 = bPossiblePart2 = false;
			}
			else
				assert(false);
		}
		if (bPossiblePart1)
			printf("Part 1: Sue %d matches\n", index);
		if (bPossiblePart2)
			printf("Part 2: Sue %d matches\n", index);
	}
	int index;

	int children = -1;
	int cats = -1;
	int samoyeds = -1;
	int pomeranians = -1;
	int akitas = -1;
	int vizslas = -1;
	int goldfish = -1;
	int trees = -1;
	int cars = -1;
	int perfumes = -1;
};

void Process(const char* filename)
{
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
				// Sue 1: goldfish: 9, cars: 0, samoyeds: 9
				/*
				children: 3
				cats: 7
				samoyeds: 2
				pomeranians: 3
				akitas: 0
				vizslas: 0
				goldfish: 5
				trees: 3
				cars: 2
				perfumes: 1
				*/
				Sue s(thisLine);
			}
		}
	}
	fclose(fp);
	delete[] buffer;
}

int main()
{
	Process("input.txt");

	return 0;
}
