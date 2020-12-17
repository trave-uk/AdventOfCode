// day22.cpp : Defines the entry point for the console application.
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
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "result: %d\n", result );
	return 0;
}
