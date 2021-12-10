// day10.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 10/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

/*
): 3 points.
]: 57 points.
}: 1197 points.
>: 25137 points.
*/

std::string open("([{<");
std::string close(")]}>");

void Process(const char* filename)
{
	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	std::vector<char> stack;
	std::vector<int64> scores2;
	int64 score = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				stack.clear();
				bool failed = false;
				for (char c : line)
				{
					size_t opentype = open.find(c);
					if (opentype != std::string::npos)
					{
						stack.push_back(close[opentype]);
					}
					else
					{
						size_t closetype = close.find(c);
						assert(closetype != std::string::npos);
						char d = stack.back();
						stack.pop_back();
						if (c != d)
						{
							switch (c)
							{
							case ')':
								score += 3;
								break;
							case ']':
								score += 57;
								break;
							case '}':
								score += 1197;
								break;
							case '>':
								score += 25137;
								break;
							}
							failed = true;
							break;
						}
					}
				}
				if (!failed)
				{
					/*
					): 1 point.
					]: 2 points.
					}: 3 points.
					>: 4 points.
					*/
					int64 score2 = 0;
					std::reverse(stack.begin(), stack.end());
					for (char c : stack)
					{
						score2 *= 5;
						switch (c)
						{
						case ')':
							score2 += 1;
							break;
						case ']':
							score2 += 2;
							break;
						case '}':
							score2 += 3;
							break;
						case '>':
							score2 += 4;
							break;
						}
					}
					scores2.push_back(score2);
				}

			}
		}
	}
	fclose(fp);
	delete[] buffer;
	printf("%s: Part 1: %lld\n", filename, score);

	std::sort(scores2.begin(), scores2.end());
	size_t pos = scores2.size() / 2;
	printf("%s: Part 2: %lld\n", filename, scores2[pos]);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
