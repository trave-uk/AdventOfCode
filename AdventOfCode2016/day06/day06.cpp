// day06.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 28/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::vector<std::map<char, int>> stats;

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				std::string s( thisLine );
				int i = 0;
				for ( char c : s )
				{
					if ( stats.size() <= i )
					{
						stats.push_back( std::map<char,int>() );
					}
					++stats[i][c];
					++i;
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	std::string part1;
	std::string part2;
	for ( auto v : stats )
	{
		int high = 0;
		int low = INT_MAX;
		char c1 = '\0';
		char c2 = '\0';
		for ( auto it : v )
		{
			if ( it.second > high )
			{
				high = it.second;
				c1 = it.first;
			}
			if ( it.second < low )
			{
				low = it.second;
				c2 = it.first;
			}
		}
		part1.push_back( c1 );
		part2.push_back( c2 );
	}
	printf( "Part 1: %s\n", part1.data() );
	printf( "Part 2: %s\n", part2.data() );
	return 0;
}

