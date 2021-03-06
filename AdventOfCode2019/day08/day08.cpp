// day8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int width = 25;
const int height = 6;
const int depth = 100;
struct layer
{
	int pixels[height][width];
};

int main()
{
	layer layers[depth];
	char* buffer = new char[16384];
	FILE *fp = fopen( "input.txt", "rt" );
	int l = 0;
	int y = 0;
	int x = 0;
	int zeros = 0, ones = 0, twos = 0;
	int minZeros = INT_MAX;
	int result = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 16384, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			for ( int i=0; i<strlen(thisLine); ++i )
			{
				assert( l < depth );
				int number = thisLine[i] - '0';
				switch ( number )
				{
					case 0:
						++zeros;
						break;
					case 1:
						++ones;
						break;
					case 2:
						++twos;
						break;
				}
				layers[l].pixels[y][x] = number;
				++x;
				if ( x >= width )
				{
					x = 0;
					++y;
					if ( y >= height )
					{
						printf( "%d: %d, %d, %d\n", l, zeros, ones, twos );

						y = 0;
						++l;

						if ( zeros < minZeros )
						{
							minZeros = zeros;
							result = ones * twos;
						}
						zeros = 0;
						ones = 0;
						twos = 0;
					}
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	printf( "Result 1: %d\n", result );

	for ( y = 0; y < height; ++y )
	{
		for ( x = 0; x < width; ++x )
		{
			for ( l = 0; l < depth; ++l )
			{
				int pixel = layers[l].pixels[y][x];
				switch ( pixel )
				{
					case 0:
						printf( " " );
						l = depth;
						break;
					case 1:
						printf( "#" );
						l = depth;
						break;
					case 2:
						break;
				}
			}
		}
		printf( "\n" );
	}
	return 0;
}

// part 2 1st try: 115032 (too high)