// day01.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Coord
{
	Coord() = default;
	Coord( int x_, int y_ ) : x( x_ ), y( y_ ) {}
	bool operator<( const Coord& other ) const { return x < other.x || ( x == other.x && y < other.y ); }
	int len() { return abs( x ) + abs( y ); }
	int x;
	int y;
};

int main()
{
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	//FILE *fp = fopen( "example2.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	Coord pos(0,0);
	Coord dir(0,1);
	Coord dest(0,0);
	std::set<Coord> history;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				char* tok = strtok( thisLine, ", " );
				while ( tok )
				{
					int dist = atoi( tok+1 );
					switch ( *tok )
					{
						case 'R':
						{
							int d = dir.y;
							dir.y = -dir.x;
							dir.x = d;
							break;
						}
						case 'L':
						{
							int d = dir.y;
							dir.y = dir.x;
							dir.x = -d;
							break;
						}
						default:
						{
							assert( false );
						}
					}
					for ( int step = 0; step < dist; ++step )
					{
						pos.x += dir.x;
						pos.y += dir.y;

						if ( dest.len() == 0 )
						{
							if ( history.count( pos ) )
							{
								dest = pos;
							}
							else
							{
								history.insert( pos );
							}
						}
					}
					tok = strtok( nullptr, ", " );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %d\n", pos.len() );
	printf( "Part 2: %d\n", dest.len() );
	return 0;
}
