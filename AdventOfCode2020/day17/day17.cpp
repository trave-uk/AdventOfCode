// day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int shift = 16;
const int64 add = 1 << (shift - 1);
const int64 mask = (1 << shift) - 1;

int64 Index(int x, int y, int z, int w)
{
	assert(x > -add && x < add && y > -add && y < add && z > -add && z < add && w > -add && w < add);
	return (x + add) + ((y + add) << shift) + ((z + add) << (shift * 2)) + ((w + add) << (shift * 3));
}

void Coords(int64 index, int&x, int&y, int&z, int &w)
{
	x = (index & mask) - add;
	index = index >> shift;
	y = (index & mask) - add;
	index = index >> shift;
	z = (index & mask) - add;
	index = index >> shift;
	w = (index & mask) - add;
}

using Map = std::set<int64>;
Map map;

int CountCubes(int x, int y, int z, int w, int part)
{
	int total = 0;

	for (int dx = x - 1; dx <= x + 1; ++dx)
	{
		for (int dy = y - 1; dy <= y + 1; ++dy)
		{
			for (int dz = z - 1; dz <= z + 1; ++dz)
			{
				for (int dw = w - (part - 1); dw <= w + (part - 1); ++dw)
				{
					if (dx == x && dy == y && dz == z && dw == w)
					{
						continue;
					}
					if (map.count(Index(dx, dy, dz, dw)))
					{
						++total;
					}
				}
			}
		}
	}
	return total;
}

bool ProcessStep(int part)
{
	bool changes = false;
	Map neighbours;
	for (int64 active : map)
	{
		int x, y, z, w;
		Coords(active, x, y, z, w);
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				for (int dz = -1; dz <= 1; ++dz)
				{
					for (int dw = 1 - part; dw <= part - 1; ++dw)
					{
						neighbours.insert(Index(x + dx, y + dy, z + dz, w + dw));
					}
				}
			}
		}
	}

	Map next;
	for (int64 test : neighbours)
	{
		int x, y, z, w;
		Coords(test, x, y, z, w);
		int count = CountCubes(x, y, z, w, part);
		bool active = map.count(test) > 0;

		if (active)
		{
			if (count == 2 || count == 3)
			{
				next.insert(test);
			}
			else
			{
				changes = true;
			}
		}
		else if (!active && count == 3)
		{
			next.insert(test);
			changes = true;
		}
	}
	map = next;
	return changes;
}

size_t ProcessFile(const char *filename, int part)
{
	map.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int y = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line = thisLine;
				int x = 0;
				for (char c : line)
				{
					if (c == '#')
					{
						map.insert(Index(x, y, 0, 0));
					}
					++x;
				}
				++y;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int s = 0; s < 6; ++s)
	{
		ProcessStep(part);
	}

	return map.size();
}

int main()
{
	//const char* filename = "example.txt";
	const char* filename = "input.txt";
	printf("Part 1: %zd\n", ProcessFile(filename, 1));
	printf("Part 2: %zd\n", ProcessFile(filename, 2));
	return 0;
}
