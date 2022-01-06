// day09.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 6/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::set<std::string> cities;

std::map<std::pair<std::string, std::string>, int64> distanceMap;

using path = std::vector<std::string>;

int64 Distance(path p)
{
	int64 distance = 0;
	std::string last;
	for (std::string city : p)
	{
		if (last.length() > 0)
		{
			auto key = std::make_pair(last, city);
			distance += distanceMap[key];
		}
		last = city;
	}
	return distance;
}

std::set<int64> distances;
void Worker(path p)
{
	if (p.size() == cities.size())
	{
		distances.insert(Distance(p));
	}
	for (std::string city : cities)
	{
		if (find(p.begin(), p.end(), city) == p.end())
		{
			path q = p;
			q.push_back(city);
			Worker(q);
		}
	}
}

int64 BuildPermutations()
{
	distances.clear();
	Worker(path());
	return *distances.begin();
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	cities.clear();
	distanceMap.clear();
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
				char city1[32];
				char city2[32];
				int64 distance;
				int matches = sscanf(thisLine, "%s to %s = %lld", city1, city2, &distance);
				assert(matches == 3);
				std::string c1 = city1;
				std::string c2 = city2;
				cities.insert(city1);
				cities.insert(city2);
				distanceMap.insert_or_assign(std::make_pair(c1, c2), distance);
				distanceMap.insert_or_assign(std::make_pair(c2, c1), distance);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = BuildPermutations();
	part2 = *distances.rbegin();

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 605, 982);
	Process("input.txt");

	return 0;
}
