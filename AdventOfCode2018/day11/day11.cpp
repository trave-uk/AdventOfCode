// day11.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// The power level in a given fuel cell can be found through the following process :
// 
// Find the fuel cell's rack ID, which is its X coordinate plus 10.
// Begin with a power level of the rack ID times the Y coordinate.
// Increase the power level by the value of the grid serial number( your puzzle input ).
// Set the power level to itself multiplied by the rack ID.
// Keep only the hundreds digit of the power level( so 12345 becomes 3; numbers with no hundreds digit become 0 ).
// Subtract 5 from the power level.

// Your goal is to find the 3x3 square which has the largest total power.The square must be entirely within the 300x300 grid.Identify this square using the X, Y coordinate of its top - left fuel cell.

static const int gridSerialNumber = 9810;

const int size = 300;

int get_index( int x, int y )
{
	return ((y - 1) * size) + (x - 1);
}

int main()
{
	int *grid = new int[ size * size ];
	for ( int y = 1; y <= size; ++y )
	{
		for ( int x = 1; x <= size; ++x )
		{
			int rackID = x + 10;
			int power = rackID * y;
			power += gridSerialNumber;
			power *= rackID;
			int digit = (power % 1000) / 100;
			power = digit - 5;
			int index = get_index( x, y );
			grid[index] = power;
		}
	}

	int maxmax = 0;
	int maxsize = 0;
	int maxmaxx = 0;
	int maxmaxy = 0;
	// Part 1 just uses testsize 3.
	for ( int testsize = 1; testsize <= 300; ++testsize )
	{
		int max = 0;
		int maxx = 0;
		int maxy = 0;
		for ( int y = 1; y <= size - testsize + 1; ++y )
		{
			for ( int x = 1; x <= size - testsize + 1; ++x )
			{
				int total = 0;
				for ( int y1 = y; y1 < y + testsize; ++y1 )
				{
					for ( int x1 = x; x1 < x + testsize; ++x1 )
					{
						int index = get_index( x1, y1 );
						total += grid[index];
					}
				}
				if ( total > max )
				{
					max = total;
					maxx = x;
					maxy = y;
				}
			}
		}
		if ( max > maxmax )
		{
			maxmax = max;
			maxsize = testsize;
			maxmaxx = maxx;
			maxmaxy = maxy;
		}

	}
	printf( "Maximum power %d at grid %d,%d,%d\n", maxmax, maxmaxx, maxmaxy, maxsize );
	getchar();
    return 0;
}

