// day06.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 06/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

class Map : public std::vector<std::string>
{
public:
	char& at(coord pos)
	{
		static char out = 'X';
		if (contains(pos))
			return (*this)[pos.second][pos.first];
		else
			return out;
	}

	const char& at(coord pos) const
	{
		static char out = 'X';
		if (contains(pos))
			return (*this)[pos.second][pos.first];
		else
			return out;
	}

	bool contains(coord pos) const
	{
		size_t height = this->size();
		size_t width = (*this)[0].length();
		return pos.first >= 0 && pos.first < width && pos.second >= 0 && pos.second < height;
	}
};

class Guard
{
public:
	coord pos;
	coord dir;

	bool operator==(const Guard& other)
	{
		return pos == other.pos && dir == other.dir;
	}
	bool operator<(const Guard& other) const
	{
		return pos < other.pos || (pos == other.pos && dir < other.dir);
	}
};

int64 scanPart1(const Map& m, const Guard& g)
{
	int64 count = 0;
	std::set<Guard> been;
	Map map = m;
	Guard guard = g;
	while (map.contains(guard.pos))
	{
		size_t before = been.size();
		been.insert(guard);
		if (been.size() == before)
			return -1; // in a loop!
		if (map.at(guard.pos) != 'X')
		{
			map.at(guard.pos) = 'X';
			++count;
		}
		coord newPos = guard.pos;
		newPos += guard.dir;
		if (map.at(newPos) == '#')
		{
			coord newDir(-guard.dir.second, guard.dir.first);
			guard.dir = newDir;
		}
		else
		{
			guard.pos = newPos;
		}
	}
	return count;
}

int64 scanPart2(const Map& map, const Guard& guard)
{
	int64 count = 0;
	size_t height = map.size();
	size_t width = map[0].length();
	coord pos;
	for (pos.first = 0; pos.first < width; ++pos.first)
	{
		for (pos.second = 0; pos.second < height; ++pos.second)
		{
			if (map.at(pos) == '.')
			{
				Map m = map;
				Guard g = guard;
				m.at(pos) = '#';
				if (scanPart1(m, g) == -1)
				{
					++count;
				}
			}
		}
	}
	return count;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	Guard guard;
	guard.dir = coord(0, -1);
	guard.pos.second = 0;
	bool foundGuard = false;
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
				if (!foundGuard)
				{
					auto found = std::find(line.begin(), line.end(), '^');
					if (found != line.end())
					{
						foundGuard = true;
						guard.pos.first = found - line.begin();
					}
					else
					{
						++guard.pos.second;
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = scanPart1(map, guard);
	part2 = scanPart2(map, guard);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 41, 6);
	Process("input.txt");

	return 0;
}
