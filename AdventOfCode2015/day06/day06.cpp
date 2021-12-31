// day06.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 31/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum class Action
{
	Toggle,
	TurnOn,
	TurnOff
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part2 = 0;
	std::set<coord> map;
	std::map<coord, int> map2;
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
				// turn on 0,0 through 999,999
				// toggle 0,0 through 999,0
				// turn off 499,499 through 500,500
				coord topLeft;
				coord bottomRight;
				char action[10];
				thisLine[4] = '_'; // turn_on, turn_off or togg_e
				int matches = sscanf(thisLine, "%s %lld,%lld through %lld,%lld", action, &topLeft.first, &topLeft.second, &bottomRight.first, &bottomRight.second);
				assert(matches == 5);
				Action a = (action[1] == 'o') ? Action::Toggle : ((action[6] == 'n') ? Action::TurnOn : Action::TurnOff);
				coord pos;
				for (pos.first = topLeft.first; pos.first <= bottomRight.first; ++pos.first)
				{
					for (pos.second = topLeft.second; pos.second <= bottomRight.second; ++pos.second)
					{
						switch (a)
						{
						case Action::TurnOn:
							map.insert(pos);

							++map2[pos];
							++part2;
							break;
						case Action::TurnOff:
							map.erase(pos);

							if (map2[pos] > 0)
							{
								--map2[pos];
								--part2;
							}
							break;
						case Action::Toggle:
							if (map.count(pos) > 0)
							{
								map.erase(pos);
							}
							else
							{
								map.insert(pos);
							}

							map2[pos] += 2;
							part2 += 2;
							break;
						}
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = map.size();
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example1.txt", 998996);
	Process("example2.txt", -1, 2000001);
	Process("input.txt");

	return 0;
}
