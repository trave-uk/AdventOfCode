// day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char map[100][100] = {'\0'};

int CountSeats(int x, int y, char seat, int part)
{
	int total = 0;
	if (part == 1)
	{
		for (int dx = x - 1; dx <= x + 1; ++dx)
		{
			for (int dy = y - 1; dy <= y + 1; ++dy)
			{
				if (dx < 0 || dy < 0 || (dx == x && dy == y))
				{
					continue;
				}
				if (map[dy][dx] == seat)
				{
					++total;
				}
			}
		}
	}
	else  // part 2
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			for (int dy = -1; dy <= 1; ++dy)
			{
				if (dx == 0 && dy == 0)
					continue;
				int xx = x, yy = y;
				while (true)
				{
					xx += dx;
					yy += dy;
					if (xx < 0 || yy < 0)
					{
						break;
					}
					else if (map[yy][xx] == '#')
					{
						++total;
						break;
					}
					else if (map[yy][xx] != '.')
					{
						break;
					}
				}
			}
		}
	}
	return total;
}

bool ProcessStep(int part)
{
	bool noChanges = true;
	char next[100][100];
	for (int y = 0; y < 100; ++y)
	{
		for (int x = 0; x < 100; ++x)
		{
			next[y][x] = map[y][x];
			if (next[y][x] == 'L') // empty seat
			{
				if (!CountSeats(x, y, '#', part))
				{
					next[y][x] = '#';
					noChanges = false;
				}
			}
			else if (next[y][x] == '#')
			{
				if (CountSeats(x, y, '#', part) >= (3+part))	// part 1: 4 seats, part 2: 5 seats
				{
					next[y][x] = 'L';
					noChanges = false;
				}
			}
		}
	}
	memcpy(map, next, sizeof(map));
	return noChanges;
}

int ProcessFile(const char *filename, int part)
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	int line = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (*thisLine)
			{
				strcpy(map[line], thisLine);
				++line;
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	while (!ProcessStep(part))
	{
		// wait for Process to return true ("no changes")
	}

	int filled = 0;
	for (int x = 0; x < 100; ++x)
	{
		for (int y = 0; y < 100; ++y)
		{
			if (map[y][x] == '#')
			{
				++filled;
			}
		}
	}

	return filled;
}

int main()
{
	//const char* filename = "example.txt";
	const char* filename = "input.txt";
	printf("Part 1: %d\n", ProcessFile(filename, 1));
	printf("Part 2: %d\n", ProcessFile(filename, 2));
	return 0;
}
