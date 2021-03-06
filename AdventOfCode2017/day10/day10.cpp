// day10.cpp : Defines the entry point for the console application.
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


void process( std::string input, bool part2 = false, int listsize = 256 )
{
	std::string copy = input;
	std::vector<int> lengths;
	if ( !part2 )
	{
		char* token = strtok( const_cast<char*>( copy.c_str() ), ", " );
		while ( token )
		{
			lengths.push_back( atoi( token ) );
			token = strtok( nullptr, ", " );
		}
	}
	else
	{
		for ( char c : input )
		{
			lengths.push_back( c );
		}
		lengths.push_back( 17 );
		lengths.push_back( 31 );
		lengths.push_back( 73 );
		lengths.push_back( 47 );
		lengths.push_back( 23 );
	}

	std::vector<int> list;
	list.reserve( listsize );
	for ( int i = 0; i < listsize; ++i )
	{
		list.push_back( i );
	}

	int repeats = part2 ? 64 : 1;
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

	if ( !part2 )
	{
		printf( "Input: %s\nPart 1 result = %d\n", input.c_str(), list[0] * list[1] );
	}
	else
	{
		std::string result;
		for ( int i = 0; i < listsize; i += 16 )
		{
			int _xor = 0;
			char hex[3];
			for ( int j = i; j < i + 16; ++j )
			{
				_xor ^= list[j];
			}
			assert( _xor < 256 );
			sprintf( hex, "%02x", _xor );
			result.append( hex );
		}
		printf( "Input: %s\nPart 2 result = %s\n", input.c_str(), result.c_str() );
	}
}

int main()
{
	process( "3, 4, 1, 5", false, 5 );
	if ( !load( "input.txt" ) )
	{
		return -1;
	}
	process( input );

	process( "", true );
	process( "AoC 2017", true );
	process( "1,2,3", true );
	process( "1,2,4", true );
	process( input, true );
    return 0;
}

