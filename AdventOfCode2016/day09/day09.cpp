// day09.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 29/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 calculateLength( std::string input, int part )
{
	int64 length = 0;
	const char* pos = input.data();
	while ( *pos )
	{
		int num = 0, repeat = 0;
		if ( sscanf( pos, "(%dx%d)", &num, &repeat ) == 2 )
		{
			pos += strcspn( pos, ")" ) + 1;
			if ( part == 1 )
			{
				length += num * repeat;
			}
			else
			{
				std::string segment = std::string( pos ).substr( 0, num );
				int64 sublen = calculateLength( segment, part );
				length += sublen * repeat;
			}
			pos += num;
		}
		else
		{
			++length;
			++pos;
		}
	}
	return length;
}

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	for ( const char* filename : std::vector<const char*>( { "example.txt", "input.txt" } ) )
	{
		printf( "File %s...\n", filename );
		FILE *fp = fopen( filename, "rt" );
		while ( !feof( fp ) )
		{
			char* thisLine = fgets( buffer, 65536, fp );
			if ( thisLine )
			{
				thisLine[strcspn( thisLine, "\n\r" )] = '\0';
				if ( *thisLine )
				{
					std::string input( thisLine );
					printf( "%s... ->\n", input.substr( 0, 32 ).data() );
					printf( " Part 1: %zd\n", calculateLength( input, 1 ) );
					printf( " Part 2: %zd\n", calculateLength( input, 2 ) );
				}
			}
		}
		fclose( fp );
	}
	delete[] buffer;
	return 0;
}
