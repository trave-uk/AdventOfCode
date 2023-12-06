// day06.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 06/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void ReadInts(char* thisLine, std::vector<int64>& output)
{
	char* tok = strtok(thisLine, " ");
	while (tok)
	{
		output.push_back(atoll(tok));
		tok = strtok(nullptr, " ");
	}
}

int64 Simulate(int64 time, int64 t)
{
	int64 speed = t;
	int64 duration = time - t;
	int64 distance = speed * duration;
	return distance;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<int64> times;
	std::vector<int64> distances;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// Time:      7  15   30
				// Distance:  9  40  200
				if (strncmp(thisLine, "Time:", 5) == 0)
				{
					ReadInts(thisLine + 9, times);
				}
				else if (strncmp(thisLine, "Distance:", 9) == 0)
				{
					ReadInts(thisLine + 9, distances);
				}
				else
				{
					assert(false);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	assert(times.size() == distances.size());
	int races = times.size();

	part1 = 1;
	for (int i = 0; i < races; ++i)
	{
		int64 time = times[i];
		int64 record = distances[i];
		int64 part1_ways = 0;
		for (int64 t = 1; t < time; ++t)
		{
			int64 distance = Simulate(time, t);
			if (distance > record)
				part1_ways++;
		}
		part1 *= part1_ways;
	}

	int64 part2_time;
	int64 part2_record;
	char buffer2[100];
	if (races == 3)
	{
		sprintf(buffer2, "%lld%lld%lld", times[0], times[1], times[2]);
		part2_time = atoll(buffer2);
		sprintf(buffer2, "%lld%lld%lld", distances[0], distances[1], distances[2]);
		part2_record = atoll(buffer2);
	}
	else
	{
		sprintf(buffer2, "%lld%lld%lld%lld", times[0], times[1], times[2], times[3]);
		part2_time = atoll(buffer2);
		sprintf(buffer2, "%lld%lld%lld%lld", distances[0], distances[1], distances[2], distances[3]);
		part2_record = atoll(buffer2);
	}
	printf("Part 2: time %lld, record %lld\n", part2_time, part2_record);

	// The distance is a bell-curve that goes up and then down. So find where the curve goes past the record and where it goes back down.
	int64 start = 0;
	int64 end = part2_time/2;
	while (start+1 != end)
	{
		int64 test = (start + end) / 2;
		int64 distance = Simulate(part2_time, test);
		if (distance > part2_record)
		{
			end = test;
		}
		else
		{
			start = test;
		}
	}
	int64 firstWin = end;
	int64 lastWin = part2_time - end;
	part2 = lastWin - firstWin + 1;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 288, 71503);
	Process("input.txt");

	return 0;
}
