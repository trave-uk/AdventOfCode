// day11.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 11/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Elephant
{
	int x;
	int y;
	int energy;
	void Inc()
	{
		++energy;
		if (energy == 10)
		{
			++flashes;
			int x1 = max(x - 1, 0);
			int y1 = max(y - 1, 0);
			int x2 = min(x + 1, 9);
			int y2 = min(y + 1, 9);
			for (int dy = y1; dy <= y2; ++dy)
			{
				for (int dx = x1; dx <= x2; ++dx)
				{
					elephants[dy][dx].Inc();
				}
			}
		}
	}
	void Reset()
	{
		if (energy > 9)
		{
			energy = 0;
		}
	}

	static void Print()
	{
		for (auto row : elephants)
		{
			for (Elephant e : row)
			{
				printf("%d", e.energy);
			}
			_putch('\n');
		}
		_putch('\n');
	}

	static int64 flashes;
	static std::array<std::array<Elephant, 10>, 10> elephants;
};

int64 Elephant::flashes = 0;
std::array<std::array<Elephant, 10>, 10> Elephant::elephants;


bool Step()
{
	int64 flashesBefore = Elephant::flashes;
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			Elephant::elephants[y][x].Inc();
		}
	}
	for (int y = 0; y < 10; ++y)
	{
		for (int x = 0; x < 10; ++x)
		{
			Elephant::elephants[y][x].Reset();
		}
	}
	int64 flashesThisStep = Elephant::flashes - flashesBefore;
	return (flashesThisStep == 100);
}

void Process(const char* filename)
{
	Elephant::flashes = 0;
	int result = 0;
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
				std::string line(thisLine);
				int x = 0;
				for (char c : line)
				{
					Elephant e;
					e.x = x;
					e.y = y;
					e.energy = c - '0';
					Elephant::elephants[y][x] = e;
					++x;
				}
				++y;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int64 i = 1; ; ++i)
	{
		bool bFinished = Step();
		if (i == 100)
		{
			printf("%s: Part 1: %lld\n", filename, Elephant::flashes);
		}
		if (bFinished)
		{
			printf("%s: Part 2: %lld\n", filename, i);
			break;
		}
	}
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
