// day14.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 26/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Reindeer
{
	Reindeer(std::string name, int speed, int duration, int rest)
		: name(name)
		, speed(speed)
		, duration(duration)
		, rest(rest)
	{}

	std::string name;
	int speed;
	int duration;
	int rest;

	int moving = 0;
	int resting = 0;
	int64 distance = 0;
	int64 score = 0;

	void Tick()
	{
		if (!distance) moving = duration;

		if (moving > 0)
		{
			--moving;
			distance += speed;
			if (moving == 0)
			{
				resting = rest;
			}
		}
		else if (resting > 0)
		{
			--resting;
			if (resting == 0)
			{
				moving = duration;
			}
		}
		else
		{
			assert(false);
		}
	}
};

void Process(const char* filename, int seconds, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::string name1;
	int64 part1 = 0;
	int64 part2 = 0;
	std::vector<Reindeer> reindeers;
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
				// Rudolph can fly 22 km/s for 8 seconds, but then must rest for 165 seconds.
				char name[20];
				int speed;
				int duration;
				int rest;
				int matches = sscanf(thisLine, "%s can fly %d km/s for %d seconds, but then must rest for %d seconds.", name, &speed, &duration, &rest);
				assert(matches == 4);
				reindeers.push_back(Reindeer(name, speed, duration, rest));
				int time = seconds;
				int64 distance = 0;
				while (time > 0)
				{
					int moving = min(time, duration);
					distance += moving * speed;
					time -= moving;
					time -= rest;
				}
				if (distance > part1)
				{
					name1 = name;
					part1 = distance;
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %s: %lldkm\n", filename, name1.c_str(), part1);

	for (int time = 0; time <= seconds; ++time)
	{
		Reindeer* pR = nullptr;
		for (Reindeer& r : reindeers)
		{
			r.Tick();
			if (!pR || r.distance > pR->distance)
			{
				pR = &r;
			}
		}
		for (Reindeer& r : reindeers)
		{
			if (r.distance == pR->distance)
			{
				++r.score;
			}
		}
	}

	Reindeer* pR = nullptr;
	for (Reindeer& r : reindeers)
	{
		if (!pR || r.score > pR->score)
		{
			pR = &r;
		}
	}

	part2 = pR->score;
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %s: %lld\n", filename, pR->name.c_str(), part2);
}

int main()
{
	Process("example.txt", 1000, 1120, 689);
	Process("input.txt", 2503);

	return 0;
}
