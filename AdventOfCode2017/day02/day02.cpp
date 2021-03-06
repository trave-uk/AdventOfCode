// day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	int total1 = 0;
	int total2 = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			std::set<int> numbers;
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';

			int minimum = INT_MAX;
			int maximum = 0;
			thisLine = strtok( thisLine, "\n\t\r " );
			while ( thisLine )
			{
				int number;
				sscanf( thisLine, "%d", &number );
				numbers.insert( number );
				if ( number < minimum )
					minimum = number;
				if ( number > maximum )
					maximum = number;
				thisLine = strtok( nullptr, "\n\r \t" );
			}
			total1 += maximum - minimum;

			for ( int i : numbers )
			{
				for ( int j : numbers )
				{
					if ( i != j && i % j == 0 )
					{
						total2 += i / j;
					}
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	printf( "Total 1 = %d\n", total1 );
	printf( "Total 2 = %d\n", total2 );

	return 0;
}

