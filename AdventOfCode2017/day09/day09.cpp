// day09.cpp : Defines the entry point for the console application.
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

void process( std::string input )
{
	// Should only be one line
	int nesting = 0;
	int score = 0;
	int garbage = 0;
	const char *cursor = input.c_str();
	while ( *cursor )
	{
		if ( *cursor == '<' )
		{
			// Skip to the next '>', ignoring characters immediately after '!'
			while ( *cursor != '>' )
			{
				assert( *cursor );
				if ( *cursor == '!' )
					++cursor;
				else
					++garbage;
				++cursor;
			}
			--garbage;
		}
		else if ( *cursor == '{' )
		{
			++nesting;
			score += nesting;
		}
		else if ( *cursor == '}' )
		{
			--nesting;
		}
		else if ( *cursor != ',' )
		{
			assert( false );
		}
		++cursor;
	}
	assert( nesting == 0 );
	printf( "Score = %d, with %d non-cancelled characters of garbage\n", score, garbage );
}

int main()
{
	process( "{}" );
	process( "{{{}}}" );
	process( "{{},{}}" );
	process( "{{{},{},{{}}}}" );
	process( "{<a>,<a>,<a>,<a>}" );
	process( "{{<ab>},{<ab>},{<ab>},{<ab>}}" );
	process( "{{<!!>},{<!!>},{<!!>},{<!!>}}" );
	process( "{{<a!>},{<a!>},{<a!>},{<ab>}}" );

	printf( "\n ... \n\n" );

	process( "<>" );
	process( "<random characters>" );
	process( "<<<<>" );
	process( "<{!>}>" );
	process( "<!!>" );
	process( "<!!!>>" );
	process( "<{o\"i!a,<{i<a>" );

	printf( "\n ... \n\n" );

	if ( load( "input.txt" ) )
	{
		process( input );
	}
    return 0;
}

