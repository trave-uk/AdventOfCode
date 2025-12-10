// day07.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 07/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::vector<std::string>;
using Beams = std::set<int>;
using BeamsPart2 = std::map<int, int64>;

int64 FireTachyonBeam(const Map& map)
{
	int64 part1 = 0;
	int start = map[0].find('S');
	Beams beams;
	beams.insert(start);
	for (const std::string& line : map)
	{
		Beams newBeams = beams;
		for (int beam : beams)
		{
			if (line[beam] == '^')
			{
				newBeams.erase(beam);
				newBeams.insert(beam - 1);
				newBeams.insert(beam + 1);
				++part1;
			}
		}
		beams = newBeams;
	}
	return part1;
}

int64 FireTachyonBeam2(const Map& map)
{
	int64 part2 = 1;
	int start = map[0].find('S');
	BeamsPart2 beams;
	beams[start] = 1;
	for (const std::string& line : map)
	{
		BeamsPart2 newBeams = beams;
		for (auto beam : beams)
		{
			if (line[beam.first] == '^')
			{
				newBeams[beam.first - 1] += beam.second;
				newBeams[beam.first + 1] += beam.second;
				newBeams[beam.first] = 0;
				part2 += beam.second;
			}
		}
		beams = newBeams;
	}
	return part2;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
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
				map.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = FireTachyonBeam(map);
	part2 = FireTachyonBeam2(map);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 21, 40);
	Process("input.txt");

	return 0;
}
