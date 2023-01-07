// day08.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 2/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void EscapeString(std::string& input)
{
	std::string result;
	std::string escaped;
	for (char c : input)
	{
		if (escaped.length() > 0)
		{
			escaped.append(1, c);
			if (escaped[1] == 'x')
			{
				if (escaped.length() == 4)
				{
					result.append(1, char(strtol(&escaped.front() + 2, nullptr, 16)));
					escaped.clear();
				}
			}
			else
			{
				assert(c == '"' || c == '\\');
				result.append(1, c);
				escaped.clear();
			}
		}
		else if (c == '\\')
		{
			escaped = "\\";
		}
		else if (c != '"')
		{
			result.append(1, c);
		}
	}
	input = result;
}

void EncodeString(std::string& input)
{
	std::string result;
	result.append("\"");
	for (char c : input)
	{
		if (c == '\\' || c == '"')
			result.append("\\");
		result.append(1, c);
	}
	result.append("\"");
	input = result;
}

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
				std::string line(thisLine);
				size_t s = line.length();
				EscapeString(line);
				s -= line.length();
				part1 += s;

				std::string line2(thisLine);
				s = 0 - (line2.length());
				EncodeString(line2);
				s += line2.length();
				part2 += s;
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
	Process("example.txt", 12, 19);
	Process("input.txt");

	return 0;
}
