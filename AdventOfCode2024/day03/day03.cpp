// day03.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 03/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<std::string> code;
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
				code.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	const std::regex pattern("mul\\((\\d+),(\\d+)\\).*");
	bool do_ = true;
	for (std::string line : code)
	{
		for (size_t start = 0; start < line.length(); ++start)
		{
			const std::string sline = line.substr(start);
			std::smatch match;
			if (strncmp(sline.c_str(), "do()", 4) == 0)
				do_ = true;
			else if (strncmp(sline.c_str(), "don't()", 7) == 0)
				do_ = false;
			if (std::regex_match(sline, match, pattern))
			{
				assert(match.size() == 3);
				part1 += atoi(match[1].str().c_str()) * atoi(match[2].str().c_str());
				if (do_)
					part2 += atoi(match[1].str().c_str()) * atoi(match[2].str().c_str());
			}
		}
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 161);
	Process("example2.txt", -1, 48);

	Process("input.txt");

	return 0;
}
