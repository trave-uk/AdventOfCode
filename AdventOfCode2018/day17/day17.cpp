// day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static const int SIZE = 2048;

std::array<std::array<char, SIZE>, SIZE> map;
int min_y = SIZE, max_y = 0;
int min_x = SIZE, max_x = 0;
int water = 0;
int undrained = 0;

void print_map()
{
	const int maxwidth = 233;
	int x1 = min_x-1, x2 = max_x+1;
	if ( max_x - min_x > maxwidth )
	{
		int less = ((max_x - min_x) - maxwidth) / 2;
		x1 += less;
		x2 -= less;
	}
	for ( int y = 0; y <= max_y; ++y )
	{
		for ( int x = x1; x <= x2; ++x )
		{
			putchar( map[y][x] );
		}
		putchar( '\n' );
	}
}

bool isblocked( char c )
{
	return c == '#' || c == '~';
}

// ......+.........
// ......|.........
// .#....|.......#.
// .#~~||||||||||#.
// .#~~~~~~~~~~~~#.
// .#~~~#~~~~#~~~#.
// .#~~~#~~~~#~~~#.
// .#~~~######~~~#.
// .#~~~~~~~~~~~~#.
// .##############.
// 63 tiles of water

void fallfill( int start_x, int start_y )
{
	int x = start_x;
	int y = start_y;
	static bool print = false;
	if ( print )
	{
		print_map();
		getchar();
	}
	// move downwards while sand
	while ( map[y][x] == '.' && y <= max_y )
	{
		if ( map[y][x] != '|' )
		{
			map[y][x] = '|';
			if ( y >= min_y )
				++water;
		}
		++y;
	}
	if ( isblocked(map[y][x]) )
	{
		// Hit clay. fill sideways
		while ( y > start_y )
		{
			--y;
			int start_x = x;
			// clay (or water) below, sand to the right
			while ( isblocked(map[y + 1][x]) && !isblocked(map[y][x + 1]) )
			{
				++x;
				if ( map[y][x] != '|' )
				{
					++water;
					map[y][x] = '|';
				}
				if ( !isblocked( map[y + 1][x] ) )
				{
					fallfill( x, y + 1 );
				}
			}
			bool blocked = isblocked(map[y + 1][x]) && isblocked(map[y][x + 1]);
			x = start_x;
			// clay (or water) below, sand to the left
			while ( isblocked(map[y + 1][x]) && !isblocked(map[y][x - 1]) )
			{
				--x;
				if ( map[y][x] != '|' )
				{
					++water;
					map[y][x] = '|';
				}
				if ( !isblocked( map[y + 1][x] ) )
				{
					fallfill( x, y + 1 );
				}
			}
			if ( isblocked(map[y + 1][x]) && isblocked(map[y][x - 1]) )
			{
				if ( blocked )
				{
					// blocked left and right
					// fill with static water
					while ( map[y][x] == '|' )
					{
						map[y][x] = '~';
						++undrained;
						x++;
					}
					x = start_x;
					continue;
				}
			}
			break;
		}
	}
}

bool process( const char *filename )
{
	for ( auto& line : map )
	{
		line.fill( '.' );
	}
	map[0][500] = '+';
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
				char axis1, axis2;
				int x11, x21, x22;
				sscanf( thisLine, "%c=%d, %c=%d..%d", &axis1, &x11, &axis2, &x21, &x22 );

				int x1, x2, y1, y2;
				if ( axis1 == 'x' )
				{
					assert( axis2 == 'y' );
					x1 = x2 = x11;
					y1 = x21;
					y2 = x22;
				}
				else
				{
					assert( axis1 == 'y' );
					assert( axis2 == 'x' );
					y1 = y2 = x11;
					x1 = x21;
					x2 = x22;
				}
				for ( int y = y1; y <= y2; ++y )
				{
					if ( y < min_y )
						min_y = y;
					if ( y > max_y )
						max_y = y;
					for ( int x = x1; x <= x2; ++x )
					{
						if ( x < min_x )
							min_x = x;
						if ( x > max_x )
							max_x = x;
						map[y][x] = '#';
					}
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// Now count water
	fallfill( 500, 1 );

	print_map();
	printf( "%d tiles of water, %d of which are undrained\n", water, undrained );
	return true;
}

int main()
{
//	process( "example.txt" );
	process( "input.txt" );
	getchar();
	return 0;
}

