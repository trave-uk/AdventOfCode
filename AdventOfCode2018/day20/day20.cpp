// day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::string input;

bool load( const char *filename )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				input = thisLine;
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

const int SIZE = 256;
const int START = 128;

struct pos
{
	int x = START;
	int y = START;
};

struct room
{
	bool north = false;
	bool south = false;
	bool west = false;
	bool east = false;
	int distance = SIZE*SIZE;
};

std::array<std::array<room, SIZE>, SIZE> map;
pos min;
pos max;

void print_map()
{
	for ( int y = min.y; y <= max.y; ++y )
	{
		for ( int x = min.x; x <= max.x; ++x )
		{
			room& here = map[y][x];
			putchar( '#' );
			putchar( here.north ? '-' : '#' );
		}
		printf( "#\n" );
		for ( int x = min.x; x <= max.x; ++x )
		{
			room& here = map[y][x];
			putchar( here.west ? '|' : '#' );
			putchar( '.' );
		}
		printf( "#\n" );
	}
	for ( int x = min.x; x <= max.x; ++x )
	{
		putchar( '#' );
		putchar( '#' );
	}
	printf( "#\n" );
}

void visit( int x, int y, int distance )
{
	room& here = map[y][x];
	if ( here.distance <= distance )
		return;
	here.distance = distance;
	if ( here.north )
	{
		visit( x, y - 1, distance + 1 );
	}
	if ( here.south )
	{
		visit( x, y + 1, distance + 1 );
	}
	if ( here.west )
	{
		visit( x - 1, y, distance + 1 );
	}
	if ( here.east )
	{
		visit( x + 1, y, distance + 1 );
	}
}

void process()
{
	// Process the regex. First, remove initial ^ and trailing $
	assert( input.front() == '^' && input.back() == '$' );
	input.erase( input.begin() );
	input.pop_back();

	std::stack<pos> parents;

	// The remaining regex consists of runs of directions in (possibly nested) brackets.
	int depth = 0;
	pos current;
	for ( char c : input )
	{
		room& here = map[current.y][current.x];
		room& north = map[current.y - 1][current.x];
		room& south = map[current.y + 1][current.x];
		room& west = map[current.y][current.x - 1];
		room& east = map[current.y][current.x + 1];
		switch ( c )
		{
			case 'N':
			{
				here.north = true;
				north.south = true;
				--current.y;
				break;
			}
			case 'E':
			{
				here.east = true;
				east.west = true;
				++current.x;
				break;
			}
			case 'W':
			{
				here.west = true;
				west.east = true;
				--current.x;
				break;
			}
			case 'S':
			{
				here.south = true;
				south.north = true;
				++current.y;
				break;
			}
			case '(':
			{
				parents.push( current );
				depth++;
				break;
			}
			case '|':
			{
				current = parents.top();
				break;
			}
			case ')':
			{
				current = parents.top();
				parents.pop();
				depth--;
				break;
			}
		}
		if ( current.x > max.x )
			max.x = current.x;
		if ( current.x < min.x )
			min.x = current.x;
		if ( current.y > max.y )
			max.y = current.y;
		if ( current.y < min.y )
			min.y = current.y;
	}
	print_map();

	// starting at (START,START), visit all open directions 
	visit( START, START, 0 );
	int max_distance = 0;
	int num_rooms = 0;
	for ( int y = min.y; y <= max.y; ++y )
	{
		for ( int x = min.x; x <= max.x; ++x )
		{
			int this_distance = map[y][x].distance;
			if ( this_distance == SIZE * SIZE )
				continue;
			if ( this_distance > max_distance )
				max_distance = this_distance;
			if ( this_distance >= 1000 )
				++num_rooms;
		}
	}
	printf( "Maximum distance = %d\n", max_distance );
	printf( "%d rooms are past at least 1000 doors\n", num_rooms );
}

int main()
{
//	if ( load( "example.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		printf( "Press return to continue...\n" );
		getchar();
	}
	return 0;
}

