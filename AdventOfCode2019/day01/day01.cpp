// day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "set"

int main()
{
	int score = 0;
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char line[80];
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			int mass = atoi( line );
			int originalmass = mass;
			int fuel = 0;
			int thisfuel = 0;
			do 
			{
				fuel = (mass / 3) - 2;
				if ( fuel > 0 )
				{
					thisfuel += fuel;
					mass = fuel;
				}
			} while (fuel > 0);
			printf( "%d -> %d\n", originalmass, thisfuel );
			score += thisfuel;
		}
	}
	printf( "Total = %d\n", score );
    return 0;
}

