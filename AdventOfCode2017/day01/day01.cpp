// day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	int total1 = 0;
	int total2 = 0;
	if ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			int len = strlen( thisLine );
			for ( int i = 0; i < len; ++i )
			{
				char next = thisLine[i + 1];
				if ( !next ) next = *thisLine;
				if ( thisLine[i] == next )
					total1 += next - '0';
				char half = half = thisLine[ (i + len / 2) % len ];
				if ( thisLine[i] == half)
					total2 += half - '0';
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	printf( "Total 1 = %d\n", total1 );
	printf( "Total 2 = %d\n", total2 );
	return 0;
}

