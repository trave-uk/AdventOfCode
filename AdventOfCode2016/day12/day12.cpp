// day12.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen( "example.txt", "rt" );
	//FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{

			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %d\n", result );
	return 0;
}
