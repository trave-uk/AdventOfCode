// day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );

	int total1 = 0;
	int total2 = 0;

	std::set<char> orGroup;
	std::string andGroup;
	bool bNewAnd = true;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( !*thisLine )
			{
				printf( "%zd + ", andGroup.length() );
				total1 += orGroup.size();
				total2 += andGroup.length();
				orGroup.clear();
				andGroup.clear();
				bNewAnd = true;
			}
			else
			{
				std::string line( thisLine );
				for ( char c : line )
				{
					orGroup.insert( c );
				}
				if ( bNewAnd )
				{
					andGroup = line;
					bNewAnd = false;
				}
				else
				{
					std::string combined;
					for ( char c : line )
					{
						for ( char d : andGroup )
						{
							if ( c == d )
								combined.push_back( c );
						}
					}
					andGroup = combined;
				}
			}
		}
	}
	printf( "%zd\n", andGroup.length() );
	total1 += orGroup.size();
	total2 += andGroup.length();
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %d\n", total1 );
	printf( "Part 2: %d\n", total2 );
	return 0;
}

