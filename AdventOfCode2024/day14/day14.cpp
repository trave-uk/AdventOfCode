// day14.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 14/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Robot
{
	Robot(const coord& pos = coord(), const coord& vel = coord()) : pos(pos), vel(vel) {}
	coord pos;
	coord vel;

	void Move(const coord& bounds, const int64 steps = 1)
	{
		coord move(steps, steps);
		move *= vel;
		pos += move;
		pos %= bounds;
	}
};
using Robots = std::vector<Robot>;

void PrintMap(const Robots& robots, const coord& bounds)
{
	std::set<coord> robotPositions;
	for (auto r : robots)
	{
		robotPositions.insert(r.pos);
	}

	gotoxy(0, 2);
	printf("Robots:\n");
	coord pos;
	for (pos.second = 0; pos.second < bounds.second; ++pos.second)
	{
		std::string line;
		for (pos.first = 0; pos.first < bounds.first; ++pos.first)
		{
			if (robotPositions.count(pos))
				line += "#";
			else
				line += ".";
		}
		printf("%s\n", line.c_str());
	}
}

bool DetectEasterEgg(const Robots& robots, const coord& bounds, const int64 steps)
{
	std::vector<int64> x(bounds.first), y(bounds.second);
	int64 maxX = 0, maxY = 0;
	for (auto r : robots)
	{
		++x[r.pos.first];
		if (x[r.pos.first] > maxX)
			maxX = x[r.pos.first];
		++y[r.pos.second];
		if (y[r.pos.second] > maxY)
			maxY = y[r.pos.second];
	}

	return (maxX > 20) && (maxY > 20);
}

int64 FindEasterEgg(const Robots& robots_, const int64 width, const int64 height)
{
	coord bounds(width, height);
	Robots robots = robots_;
	int64 steps = 0;
	static int64 step = 1;
	while (true)
	{
		if (DetectEasterEgg(robots, bounds, steps))
		{
			PrintMap(robots, bounds);
			break;
		}
		for (auto& r : robots)
		{
			r.Move(bounds, step);
		}
		steps += step;
	}
	return steps;
}

int64 ProcessSteps(const Robots& robots_, const int64 width, const int64 height, const int64 steps)
{
	std::array<int, 4> quadrants = {};
	coord bounds(width, height);
	Robots robots = robots_;
	for (Robot& r : robots)
	{
		r.Move(bounds, steps);

		if (r.pos.first == width / 2 || r.pos.second == height / 2)
			continue;
		if (r.pos.first < width / 2)
		{
			if (r.pos.second < height / 2)
				++quadrants[0];
			else
				++quadrants[1];
		}
		else
		{
			if (r.pos.second < height / 2)
				++quadrants[2];
			else
				++quadrants[3];
		}
	}
	return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

void Process(const char* filename, int64 width, int64 height, int64 expectedPart1 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	Robots robots;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				Robot r;
				std::string line(thisLine);
				// p=0,4 v=3,-3
				int count = sscanf_s(thisLine, "p=%lld,%lld v=%lld,%lld", &r.pos.first, &r.pos.second, &r.vel.first, &r.vel.second);
				assert(count == 4);
				if (r.vel.first < 0)
					r.vel.first += width;
				if (r.vel.second < 0)
					r.vel.second += height;
				robots.push_back(r);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = ProcessSteps(robots, width, height, 100);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (expectedPart1 == -1)
	{
		int64 part2 = FindEasterEgg(robots, width, height);
		printf("%s: Part 2: %lld\n", filename, part2);
	}

}

int main()
{
	Process("example.txt", 11, 7, 12);
	Process("input.txt", 101, 103);

	return 0;
}
