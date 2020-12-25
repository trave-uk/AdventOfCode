// day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Coord
{
	int x = 0;
	int y = 0;

	Coord() = default;
	Coord( int x_, int y_ )
	{
		x = x_;
		y = y_;
	}
	bool operator<( const Coord& other ) const
	{
		return ( x < other.x || ( x == other.x && y < other.y ) );
	}
};

enum STATE { CLEAN, INFECTED, WEAKENED, FLAGGED };

using Map = std::map<Coord, STATE>;

enum DIR { UP, RIGHT, DOWN, LEFT };

DIR operator++(DIR& dir)
{
	dir = (DIR)((int)dir+1);
	if ( dir > LEFT )
	{
		dir = UP;
	}
	return dir;
}

DIR operator--( DIR& dir )
{
	dir = (DIR)( (int)dir - 1 );
	if ( dir < UP )
	{
		dir = LEFT;
	}
	return dir;
}

Map map;
Coord pos;
DIR dir = UP;
int infections = 0;

void ProcessBurst(int part)
{
	STATE& state = map[pos];
	if ( part == 1 )
	{
		if ( state == INFECTED )
		{
			++dir;
			state = CLEAN;
		}
		else
		{
			assert( state == CLEAN );
			--dir;
			state = INFECTED;
			++infections;
		}
	}
	else
	{
		// part 2
		switch ( state )
		{
			case CLEAN:
				state = WEAKENED;
				--dir;
				break;
			case WEAKENED:
				state = INFECTED;
				++infections;
				break;
			case INFECTED:
				state = FLAGGED;
				++dir;
				break;
			case FLAGGED:
				state = CLEAN;
				++dir;
				++dir;
				break;
		}
	}
	pos.x += ( dir == RIGHT ) - ( dir == LEFT );
	pos.y += ( dir == DOWN ) - ( dir == UP );
}

void ProcessFile( const char* filename, int part )
{
	map.clear();
	pos = Coord( 0, 0 );
	dir = UP;
	infections = 0;

	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	int size = 0;
	int dxy = -1;
	int y = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				if ( !size )
				{
					size = strlen( thisLine );
					dxy = -size / 2;
				}
				assert( strlen( thisLine ) == size );
				for ( int x = 0; x < size; ++x )
				{
					if ( thisLine[x] == '#' )
					{
						Coord c( x + dxy, y + dxy );
						map[c] = INFECTED;
					}
				}
				++y;
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	assert( y == size );

	const int steps = ( part == 1 ) ? 10000 : 10000000;
	for ( int step = 0; step < steps; ++step )
	{
		ProcessBurst( part );
	}

	int result = infections;
	printf( "Part %d: %d\n", part, result );
}

int main()
{
	ProcessFile( "example.txt", 1 );
	ProcessFile( "example.txt", 2 );
	ProcessFile( "input.txt", 1 );
	ProcessFile( "input.txt", 2 );
	return 0;
}
