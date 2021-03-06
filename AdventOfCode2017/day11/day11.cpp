// day11.cpp : Defines the entry point for the console application.
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
	return true;
}

enum dir
{
	n,
	ne,
	se,
	s,
	sw,
	nw,
	dir_count
};

const char* dirs[] = 
{
	"n",
	"ne",
	"se",
	"s",
	"sw",
	"nw"
};

static_assert( _ARRAYSIZE_( dirs ) == dir_count, "" );

bool ready( const std::array<int, dir_count>& steps )
{
	int directions = 0;
	bool adjacent = false;
	for ( int i = 0; i < dir_count; ++i )
	{
		if ( steps[i] > 0 )
		{
			++directions;
			if ( steps[( i + 1 ) % dir_count] > 0 )
			{
				adjacent = true;
			}
		}
	}
	return ( directions <= 1 ) || ( directions == 2 && adjacent );
}

void resolve_opposites( std::array<int, dir_count>& steps )
{
	// Resolve opposites
	std::array<int, dir_count> oldsteps = steps;
	for ( int d = 0; d < dir_count; ++d )
	{
		int opp = ( d + 3 ) % dir_count;
		steps[d] -= oldsteps[opp];
	}
	for ( int d = 0; d < dir_count; ++d )
	{
		if ( steps[d] < 0 )
			steps[d] = 0;
	}
}

void resolve( std::array<int, dir_count>& steps )
{
	resolve_opposites( steps );
	// For every pair of entries exactly two directions apart, resolve one of them to zero, incrementing the mid direction appropriately
	for ( int d = 0; d < dir_count; ++d )
	{
		int &dirsteps = steps[d];
		int &stepsplus2 = steps[( d + 2 ) % dir_count];
		if ( dirsteps > 0 && stepsplus2 > 0 )
		{
			int mid = min( dirsteps, stepsplus2 );
			dirsteps -= mid;
			stepsplus2 -= mid;
			steps[( d + 1 ) % dir_count] += mid;
		}
	}
	resolve_opposites( steps );
}

int distance( const std::array<int, dir_count>& steps )
{
	std::array<int, dir_count> copy = steps;
	// Now, resolve steps in each direction until there are (maximum) 2 *adjacent* directions containing any steps.
	while ( !ready( copy ) )
	{
		resolve( copy );
	}

	int total = 0;
	for ( int d = 0; d < dir_count; ++d )
	{
		total += copy[d];
	}
	return total;
}

void process( std::string input, bool part2 = false )
{
	std::array<int, dir_count> steps = {};
	std::string copy = input;
	int maxdistance = 0;
	char* token = strtok( const_cast<char*>( copy.c_str() ), ", " );
	while ( token )
	{
		int d = dir_count;
		for ( d = 0; d < dir_count; ++d )
		{
			if ( strcmp( token, dirs[d] ) == 0 )
			{
				break;
			}
		}
		assert( d < dir_count );
		++steps[d];

		token = strtok( nullptr, ", " );
		int dist = distance( steps );
		if ( dist > maxdistance )
		{
			maxdistance = dist;
		}
	}

	int total = distance( steps );

	if ( input.length() < 100 )
	{
		printf( "Result: %d, max %d (input %s)\n", total, maxdistance, input.c_str() );
	}
	else
	{
		printf( "Part 1: %d\nPart 2: %d\n", total, maxdistance );
	}
}

int main()
{
	process( "ne,ne,ne" );
	process( "ne,ne,sw,sw" );
	process( "ne,ne,s,s" );
	process( "se,sw,se,sw,sw" );
	if ( !load( "input.txt" ) )
	{
		return -1;
	}
	process( input );
    return 0;
}

