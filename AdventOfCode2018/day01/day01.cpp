// day1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "set"

int main()
{
	std::set<int> seen;
	int total = 0;
	char line[80];
	int delta;
	bool bFoundDuplicate = false;
	seen.insert( 0 );
	while ( !bFoundDuplicate )
	{
		FILE *fp = fopen( "input.txt", "rt" );
		while ( !feof( fp ) )
		{
			char* thisLine = fgets( line, 80, fp );
			if ( thisLine )
			{
				delta = atoi( line );
				total += delta;
				if ( seen.count( total ) == 1 )
				{
					bFoundDuplicate = true;
					printf( "Duplicate: %d\n", total );
				}
				else
				{
					seen.insert( total );
				}
			}
		}
	}
	printf( "Total = %d\n", total );
    return 0;
}

