// day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum class DIR
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};

int x = 0;
int y = 0;
DIR dir = DIR::EAST;
int wx = 10;
int wy = -1;

void Move(DIR d, int dist, int part)
{
	int& xx = (part == 1) ? x : wx;
	int& yy = (part == 1) ? y : wy;
	switch (d)
	{
	case DIR::EAST:
		xx += dist;
		break;
	case DIR::WEST:
		xx -= dist;
		break;
	case DIR::NORTH:
		yy -= dist;
		break;
	case DIR::SOUTH:
		yy += dist;
		break;
	}
}

void Turn(bool Left, int Angle, int part)
{
	for (int i = 0; i < Angle / 90; ++i)
	{
		if (part == 1)
		{
			switch (dir)
			{
			case DIR::EAST:
				dir = Left ? DIR::NORTH : DIR::SOUTH;
				break;
			case DIR::WEST:
				dir = Left ? DIR::SOUTH : DIR::NORTH;
				break;
			case DIR::NORTH:
				dir = Left ? DIR::WEST : DIR::EAST;
				break;
			case DIR::SOUTH:
				dir = Left ? DIR::EAST : DIR::WEST;
				break;
			}
		}
		else // part 2
		{
			int tx = wx;
			int ty = wy;
			if (Left)
			{
				wy = -tx;
				wx = ty;
			}
			else
			{
				wy = tx;
				wx = -ty;
			}
		}
	}
}

int main()
{
	const char* filename =
		//"example.txt";
		"input.txt";
	char* buffer = new char[65536];
	for (int part = 1; part <= 2; ++part)
	{
		x = 0;
		y = 0;
		dir = DIR::EAST;	//p1
		wx = 10;	// p2
		wy = -1;		// p2
		FILE *fp = fopen(filename, "rt");
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (*thisLine)
				{
					int dist = atoi(thisLine + 1);
					switch (thisLine[0])
					{
					case 'N':
						Move(DIR::NORTH, dist, part);
						break;
					case 'S':
						Move(DIR::SOUTH, dist, part);
						break;
					case 'W':
						Move(DIR::WEST, dist, part);
						break;
					case 'E':
						Move(DIR::EAST, dist, part);
						break;
					case 'L':
						Turn(true, dist, part);
						break;
					case 'R':
						Turn(false, dist, part);
						break;
					case 'F':
						if (part == 1)
						{
							Move(dir, dist, part);
						}
						else
						{
							x += wx * dist;
							y += wy * dist;
						}
						break;
					}
				}
			}
		}
		fclose(fp);
		printf("Part %d: %d\n", part, abs(x) + abs(y));
	}
	delete[] buffer;

	return 0;
}
