// day15.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 15/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"


int64 Hash(std::string s)
{
	int64 hash = 0;
	for (char c : s)
	{
		int64 ascii = c;
		hash += ascii;
		hash *= 17;
		hash &= 255;
	}
	return hash;
}

struct Lens
{
	Lens(std::string l, int64 fl)
		: label(l)
		, focalLength(fl)
	{}

	std::string label;
	int64 focalLength;
};

using Box = std::vector<Lens>;
void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::array<Box, 256> boxes;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				char* tok = strtok(thisLine, ",");
				while (tok)
				{
					std::string line(tok);
					part1 += Hash(line);

					// "rn=1"
					char* context;
					char* tok2 = strtok_s(tok, "=-", &context);
					std::string label(tok2);
					int64 box = Hash(label);
					char op = line[label.length()];
					Box& b = boxes[box];
					if (op == '-')
					{
						// remove lens with label from box
						for (Box::iterator it = b.begin(); it != b.end(); ++it)
						{
							Lens& lens = *it;
							if (lens.label == label)
							{
								b.erase(it);
								break;
							}
						}
					}
					else
					{
						// insert or replace lens with label to box
						assert(op == '=');
						int64 fl = atoll(tok2 + label.length() + 1);
						bool replaced = false;
						for (Lens& lens : b)
						{
							if (lens.label == label)
							{
								lens.focalLength = fl;
								replaced = true;
								break;
							}
						}
						if (!replaced)
						{
							b.push_back(Lens(label, fl));
						}
					}
					tok = strtok(nullptr, ",");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 b = 1;
	for (Box& box : boxes)
	{
		int64 l = 1;
		for (Lens& lens : box)
		{
			part2 += b * l * lens.focalLength;
			++l;
		}
		++b;
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 1320, 145);
	Process("input.txt");

	return 0;
}
