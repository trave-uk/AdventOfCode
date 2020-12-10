// day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char map[256][256];

enum class DIRECTION
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int line = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			strcpy(map[line], thisLine);
			++line;
		}
	}
	fclose( fp );
	delete[] buffer;

	std::vector<char> result;

	// find start, on row 0.
	int x = 0;
	int y = 0;
	for (int i = 0; x == 0; ++i)
	{
		if (map[0][i] != ' ')
			x = i;
	}
	DIRECTION dir = DIRECTION::DOWN;
	int distance = 0;

	do
	{
		switch (dir)
		{
		case DIRECTION::LEFT:
			--x;
			break;
		case DIRECTION::RIGHT:
			++x;
			break;
		case DIRECTION::UP:
			--y;
			break;
		case DIRECTION::DOWN:
			++y;
			break;
		}
		++distance;
		char tile = map[y][x];
		if (tile == '-' || tile == '|')
		{
			// continue on
			continue;
		}
		if (isalpha(tile))
		{
			result.push_back(tile);
			continue;
		}
		if (tile == '+')
		{
			// turn
			if (dir == DIRECTION::LEFT || dir == DIRECTION::RIGHT)
			{
				if (map[y - 1][x] != ' ')
				{
					dir = DIRECTION::UP;
				}
				else
				{
					assert(map[y + 1][x] != ' ');
					dir = DIRECTION::DOWN;
				}
			}
			else
			{
				if (map[y][x - 1] != ' ')
				{
					dir = DIRECTION::LEFT;
				}
				else
				{
					assert(map[y][x + 1] != ' ');
					dir = DIRECTION::RIGHT;
				}
			}
		}
		else
		{
			// End
			break;
		}
	} while (true);
	result.push_back('\0');
	printf("Part 1: %s\n", &result.front());
	printf("Part 2: %d\n", distance);
	return 0;
}
