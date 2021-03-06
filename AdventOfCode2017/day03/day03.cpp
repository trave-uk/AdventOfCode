// day3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define WIDTH 599
#define HEIGHT 599

#define CENTRE 299

#define TARGET 347991

std::array<std::array<int, WIDTH>, HEIGHT> grid = {};
bool bFoundPart2 = false;
void setIndex( int x, int y, int& index )
{
	if ( index == TARGET )
	{
		int distance = abs( x - CENTRE ) + abs( y - CENTRE );
		printf( "Part 1: %d found at (%d, %d)\nDistance from centre = %d.", index, x, y, distance );
	}
	int value = 0;
	for ( int i = x - 1; i <= x + 1; ++i )
	{
		for ( int j = y - 1; j <= y + 1; ++j )
		{
			value += grid[i][j];
		}
	}
	value = __max( 1, value );
	grid[x][y] = value;
	if ( !bFoundPart2 && value >= TARGET )
	{
		printf( "Part 2: first value greater than %d is %d\n", TARGET, value );
		bFoundPart2 = true;
	}

	index++;
}

int main()
{
	int index = 1;
	int x = CENTRE;
	int y = CENTRE;
	setIndex( x, y, index );
	for ( int side = 1; side < HEIGHT-2; side += 2 )
	{
		for ( int i = 0; i < side; ++i )
		{
			++x;
			setIndex( x, y, index );
		}
		for ( int i = 0; i < side; ++i )
		{
			--y;
			setIndex( x, y, index );
		}
		for ( int i = 0; i < side+1; ++i )
		{
			--x;
			setIndex( x, y, index );
		}
		for ( int i = 0; i < side+1; ++i )
		{
			++y;
			setIndex( x, y, index );
		}
	}
	return 0;
}

