// day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	int result1 = -1;
	std::vector<int> ids;
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				int id = 0;
				assert( strlen( thisLine ) == 10 );
				for ( int i = 0; i < 10; ++i )
				{
					id *= 2;
					switch ( thisLine[i] )
					{
						case 'F':
						case 'L':
							break;
						case 'B':
						case 'R':
							id += 1;
							break;
						default:
							printf( "Unexpected '%c'\n", thisLine[i] );
							break;
					}
				}
				//printf( "%s: row %d, column %d, seat ID %d\n", thisLine, id / 8, id % 8, id );
				if ( id > result1 )
					result1 = id;
				ids.push_back( id );
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "result 1: %d\n", result1 );

	int result2 = -1;
	std::set<int> remainingSeats;
	// NB: part 1 contains the highest seat ID
	for ( int i = 0; i < result1; i += 8 )
	{
		for ( int j = i; j < i + 8; ++j )
		{
			remainingSeats.insert( j );
		}
	}

	for ( int id : ids )
	{
		if ( remainingSeats.count( id ) > 0 )
		{
			remainingSeats.erase( id );
		}
	}

	for ( int i : remainingSeats )
	{
		if ( remainingSeats.count( i - 1 ) == 0 && remainingSeats.count( i + 1 ) == 0 )
		{
			assert( result2 == -1 );
			result2 = i;
		}
	}
	printf( "result 2: %d\n", result2 );

	return 0;
}
