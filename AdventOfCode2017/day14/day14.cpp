// day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


void knothash( std::string input, std::string& output, bool binaryout )
{
	const int listsize = 256;
	std::string copy = input;
	std::vector<int> lengths;
	for ( char c : input )
	{
		lengths.push_back( c );
	}
	lengths.push_back( 17 );
	lengths.push_back( 31 );
	lengths.push_back( 73 );
	lengths.push_back( 47 );
	lengths.push_back( 23 );

	std::vector<int> list;
	list.reserve( listsize );
	for ( int i = 0; i < list.capacity(); ++i )
	{
		list.push_back( i );
	}

	int repeats = 64;
	int skipsize = 0;
	int index = 0;
	for ( int repeat = 0; repeat < repeats; ++repeat )
	{
		for ( int length : lengths )
		{
			assert( length <= listsize );
			int start = index;
			int end = ( index + length - 1 ) % listsize;
			for ( int i = 0; i < length / 2; ++i )
			{
				int t = list[start];
				list[start] = list[end];
				list[end] = t;
				++start;
				if ( start >= listsize )
					start -= listsize;
				--end;
				if ( end < 0 )
					end += listsize;
			}
			index += length + skipsize;
			index %= listsize;
			++skipsize;
			skipsize %= listsize;
		}
	}

	output.clear();
	for ( int i = 0; i < listsize; i += 16 )
	{
		int _xor = 0;
		for ( int j = i; j < i + 16; ++j )
		{
			_xor ^= list[j];
		}
		assert( _xor < 256 );
		if ( binaryout )
		{
			char bin[9];
			for ( int bit = 0; bit < 8; ++bit )
			{
				if ( _xor & ( 1 << (7-bit) ) )
				{
					bin[bit] = '#';
				}
				else
				{
					bin[bit] = '.';
				}
				bin[8] = '\0';
			}
			output.append( bin );
		}
		else
		{
			char hex[3];
			sprintf( hex, "%02x", _xor );
			output.append( hex );
		}
	}
}

std::vector<std::string> map;

void floodfill( int x, int y )
{
	if ( x < 0 || x > 127 || y < 0 || y > 127 )
		return;

	if ( map[y][x] != '#' )
		return;

	map[y][x] = '*';
	floodfill( x - 1, y );
	floodfill( x + 1, y );
	floodfill( x, y - 1 );
	floodfill( x, y + 1 );
}

int process( std::string input )
{
	map.clear();
	int blocks = 0;
	printf( "\n%s:\n", input.c_str() );
	for ( int i = 0; i < 128; ++i )
	{
		std::string hashinput = input + "-" + std::to_string(i);
		std::string hash;
		knothash( hashinput, hash, true );
		for ( char c : hash )
		{
			if ( c == '#' || c == '1' )
			{
				++blocks;
			}
		}
		map.push_back( hash );
//		printf( "%s\n", hash.c_str() );
	}
	printf( "Part 1: %d\n", blocks );

	int regions = 0;
	for ( int y = 0; y < 128; ++y )
	{
		for ( int x = 0; x < 128; ++x )
		{
			if ( map[y][x] == '#' )
			{
				floodfill( x, y );
				++regions;
			}
		}
	}
	printf( "Part 2: %d\n", regions );

	return 0;
}

int main()
{
	process( "flqrgnkx" );	// <-- example
	process( "nbysizxe" );	// <-- puzzle input
    return 0;
}

