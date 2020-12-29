// day08.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 29/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

#define at(x,y) tiles[ ( ( y ) * width ) + ( x ) ]

void ProcessFile( const char* filename, int width, int height )
{
	printf( "Processing %s...\n", filename );
	char* tiles = new char[width*height];
	memset( tiles, '.', width*height );

	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				char* tok = strtok( thisLine, " " );
				if ( strcmp( tok, "rect" ) == 0 )
				{
					tok = strtok( nullptr, " " );
					int w, h;
					int count = sscanf( tok, "%dx%d", &w, &h );
					assert( count == 2 );
					for ( int y = 0; y < h; ++y )
					{
						for ( int x = 0; x < w; ++x )
						{
							at( x, y ) = '#';
						}
					}
				}
				else if ( strcmp( tok, "rotate" ) == 0 )
				{
					char dir[10];
					char axis;
					int num;
					int amount;
					tok = strtok( nullptr, "\n" );
					int count = sscanf( tok, "%s %c=%d by %d", dir, &axis, &num, &amount );
					assert( count == 4 );
					for ( int i = 0; i < amount; ++i )
					{
						if ( axis == 'x' )
						{
							int x = num;
							int t = at( x, height - 1 );
							for ( int y = height - 1; y > 0; --y )
							{
								at( x, y ) = at( x, y - 1 );
							}
							at( x, 0 ) = t;
						}
						else
						{
							int y = num;
							int t = at( width - 1, y );
							for ( int x = width - 1; x >0; --x )
							{
								at( x, y ) = at( x - 1, y );
							}
							at( 0, y ) = t;
						}
					}
				}
				else
				{
					assert( false );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int part1 = 0;
	for ( int i = 0; i < width*height; ++i )
	{
		if ( tiles[i] == '#' )
			++part1;
	}
	printf( "Part 1: %d\n", part1 );
	printf( "Part 2:\n" );
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			char c = at( x, y );
			printf( "%c", c );
		}
		printf( "\n" );
	}
	delete[] tiles;
}

int main()
{
	ProcessFile( "example.txt", 7, 3 );
	ProcessFile( "input.txt", 50, 6 );
	return 0;
}