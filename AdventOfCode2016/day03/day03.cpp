// day03.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int main()
{
	int part1 = 0;
	int part2 = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	std::vector<std::vector<int>> tris;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				std::vector<int> tri;
				char* tok = strtok( thisLine, " " );
				while ( tok )
				{
					assert( isdigit( *tok ) );
					int side = atoi( tok );
					tri.push_back( side );
					tok = strtok( nullptr, " " );
				}
				assert( tri.size() == 3 );
				tris.push_back( tri );
				std::sort( tri.begin(), tri.end() );
				if ( tri[0] + tri[1] > tri[2] )
				{
					++part1;
				}
				if ( tris.size() == 3 )
				{
					for ( int i = 0; i < 3; ++i )
					{
						tri.clear();
						for ( int j = 0; j < 3; ++j )
						{
							tri.push_back( tris[j][i] );
						}
						assert( tri.size() == 3 );
						std::sort( tri.begin(), tri.end() );
						if ( tri[0] + tri[1] > tri[2] )
						{
							++part2;
						}
					}
					tris.clear();
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %d\n", part1 );
	printf( "Part 2: %d\n", part2 );
	return 0;
}
// 1266 too low