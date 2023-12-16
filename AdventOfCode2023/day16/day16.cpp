// day16.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 16/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

const coord ZERO(0, 0);
const coord RIGHT(1, 0);
const coord LEFT(-1, 0);
const coord UP(0, -1);
const coord DOWN(0, 1);

struct Light
{
	Light(const coord& p = ZERO, const coord& d = RIGHT)
		: pos(p)
		, dir(d)
	{}

	coord pos;
	coord dir;

	bool operator<(const Light& other) const
	{
		if (pos < other.pos)
			return true;
		else if (pos == other.pos && dir < other.dir)
			return true;
		return false;
	}
};

using Map = std::map<coord, char>;
using LightMap = std::map<coord, std::set<Light>>;

int64 MakeBeams(Map map, LightMap energy, std::vector<Light> lights)
{
	int64 totalEnergy = 0;
	for (;;)
	{
		if (lights.size() == 0)
			break;
		std::vector<Light> newLights;
		for (Light l : lights)
		{
			auto& inMap = energy[l.pos];
			if (inMap.count(l))
			{
				// this Light has started repeating, so remove it;
				continue;
			}
			else
			{
				if (inMap.size() == 0)
					++totalEnergy;
				inMap.insert(l);
			}
			char tile = map[l.pos];
			switch (tile)
			{
			case '/':
				if (l.dir == RIGHT)
					l.dir = UP;
				else if (l.dir == LEFT)
					l.dir = DOWN;
				else if (l.dir == UP)
					l.dir = RIGHT;
				else if (l.dir == DOWN)
					l.dir = LEFT;
				newLights.push_back(l);
				break;
			case '\\':
				if (l.dir == RIGHT)
					l.dir = DOWN;
				else if (l.dir == LEFT)
					l.dir = UP;
				else if (l.dir == UP)
					l.dir = LEFT;
				else if (l.dir == DOWN)
					l.dir = RIGHT;
				newLights.push_back(l);
				break;
			case '|':
				if (l.dir == RIGHT || l.dir == LEFT)
				{
					l.dir = UP;
					newLights.push_back(l);
					l.dir = DOWN;
				}
				newLights.push_back(l);
				break;
			case '-':
				if (l.dir == UP || l.dir == DOWN)
				{
					l.dir = LEFT;
					newLights.push_back(l);
					l.dir = RIGHT;
				}
				newLights.push_back(l);
				break;
			case '.':
				newLights.push_back(l);
				break;
			}
		}
		lights = std::vector<Light>();
		for (Light nl : newLights)
		{
			nl.pos += nl.dir;
			if (map.count(nl.pos) != 0)
				lights.push_back(nl);
		}
	}
	return totalEnergy;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	LightMap energy;
	coord pos(0, 0);
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				pos.first = 0;
				for (char c : line)
				{
					map[pos] = c;
					energy[pos] = std::set<Light>();
					++pos.first;
				}
				++pos.second;
			}
		}
	}
	coord size = pos;
	fclose(fp);
	delete[] buffer;
	std::vector<Light> lights;
	lights.push_back(Light(coord(0, 0), RIGHT));
	part1 = MakeBeams(map, energy, lights);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	for (int64 x = 0; x < size.first; ++x)
	{
		std::vector<Light> lightsDown;
		lightsDown.push_back(Light(coord(x, 0), DOWN));
		int64 test = MakeBeams(map, energy, lightsDown);
		if (test > part2)
			part2 = test;

		std::vector<Light> lightsUp;
		lightsUp.push_back(Light(coord(x, size.second - 1), UP));
		test = MakeBeams(map, energy, lightsUp);
		if (test > part2)
			part2 = test;
	}

	for (int64 y = 0; y < size.second; ++y)
	{
		std::vector<Light> lightsRight;
		lightsRight.push_back(Light(coord(0, y), RIGHT));
		int64 test = MakeBeams(map, energy, lightsRight);
		if (test > part2)
			part2 = test;

		std::vector<Light> lightsLeft;
		lightsLeft.push_back(Light(coord(size.first - 1, y), LEFT));
		test = MakeBeams(map, energy, lightsLeft);
		if (test > part2)
			part2 = test;
	}

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 46, 51);
	Process("input.txt");

	return 0;
}
