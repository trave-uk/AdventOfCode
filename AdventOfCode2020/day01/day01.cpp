// day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
//	FILE *fp = fopen( "example.txt", "rt" );
	std::set<int> numbers;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				int val = atoi( thisLine );
				numbers.insert( val );
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	for ( int val1 : numbers )
	{
		for ( int val2 : numbers )
		{
			if ( val2 <= val1 )
				continue;

			if ( val1 + val2 == 2020 )
			{
				printf( "%d * %d = %d\n", val1, val2, val1 * val2 );
			}
		}
	}

	for ( int val1 : numbers )
	{
		for ( int val2 : numbers )
		{
			if ( val2 <= val1 )
				continue;

			for ( int val3 : numbers )
			{
				if ( val3 <= val2 )
					continue;

				if ( val1 + val2 + val3 == 2020 )
				{
					printf( "%d * %d * %d = %d\n", val1, val2, val3, val1 * val2 * val3 );
				}
			}
		}
	}
	return 0;
}

