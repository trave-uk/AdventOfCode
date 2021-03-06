// day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


const int size = 50;

using map_t = std::array<std::array<char, size>, size>;
map_t map;
int width = 0;
int height = 0;

volatile int step = 10000;

int countNeighbouringTiles( int x, int y, char type )
{
	int count = 0;
	for ( int xx = x - 1; xx <= x + 1; ++xx )
	{
		for ( int yy = y - 1; yy <= y + 1; ++yy )
		{
			if ( xx == x && yy == y )
				continue;
			if ( xx < 0 || xx >= width || yy < 0 || yy >= height )
				continue;
			if ( map[yy][xx] == type )
				++count;
		}
	}
	return count;
}

void print_map()
{
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			putchar( map[y][x] );
		}
		putchar( '\n' );
	}
}

void process()
{
	int trees = 0, mills = 0, open = 0;
	for ( int minute = 0; minute < 1000000000; ++minute )
	{
		if ( minute >= 1000 )
		{
			printf( "Minute %d:\n", minute );
			print_map();
			printf( "Total trees = %d, total mills = %d, %d * %d = %d\n", trees, mills, trees, mills, trees*mills );
			getchar();
		}

		trees = 0;
		mills = 0;
		open = 0;
		map_t newmap;
		for ( int y = 0; y < height; ++y )
		{
			for ( int x = 0; x < width; ++x )
			{
				char newtile = map[y][x];
				switch ( map[y][x] )
				{
					case '.':
					{
						if ( countNeighbouringTiles( x, y, '|' ) >= 3 )
						{
							newtile = '|';
						}
						break;
					}
					case '|':
					{
						if ( countNeighbouringTiles( x, y, '#' ) >= 3 )
						{
							newtile = '#';
						}
						break;
					}
					case '#':
					{
						if ( countNeighbouringTiles( x, y, '#' ) == 0 || countNeighbouringTiles( x, y, '|' ) == 0 )
						{
							newtile = '.';
						}
						break;
					}
				}
				if ( newtile == '|' )
					++trees;
				if ( newtile == '#' )
					++mills;
				if ( newtile == '.' )
					++open;
				newmap[y][x] = newtile;
			}
		}
		assert( trees + mills + open == width * height );
		map = newmap;
	}
	print_map();
	printf( "Total trees = %d, total mills = %d, %d * %d = %d\n", trees, mills, trees, mills, trees*mills );
}

bool load( const char *filename )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	int y = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				if ( strlen( thisLine ) > width )
					width = strlen( thisLine );
				memcpy( &map[y][0], thisLine, width );
				++y;
				if ( y > height )
					height = y;
			}
		}
	}
	return true;
}

int main()
{
	//if ( load( "example.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		getchar();
	}
	return 0;
}

// Calculation error on my part: 218750 was the answer at minute 10000, and the pattern was repeating every 28 minutes. However, I was thinking "1000", and calculated that (1000000000 - 1000) % 28 = 0, so thought 218750 was the answer.
// Actually, (1000000000 - 10000) % 28 = 16, so the answer (215404) was at 10016.
// I really need to automate that calculation!

// 218750 too high
// 215625 too high
// 212553 too low
// 212711 wrong
// 214900
// 215404 correct!

