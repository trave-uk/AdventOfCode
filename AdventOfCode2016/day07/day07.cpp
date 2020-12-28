// day07.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 28/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::vector<std::string> part1;
std::vector<std::string> part2;

int main()
{
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
				bool pass1 = false;
				bool fail1 = false;
				std::vector<std::pair<char, char>> inside2;
				std::vector<std::pair<char, char>> outside2;

				bool inner = false;
				for ( int i = 0; i < strlen( thisLine ) - 2; ++i )
				{
					char* patt = thisLine + i;
					if ( *patt == '[' )
					{
						inner = true;
					}
					else if ( *patt == ']' )
					{
						inner = false;
					}
					else
					{
						bool pattern1 = patt[0] == patt[3] && patt[1] == patt[2] && patt[0] != patt[1];
						if ( pattern1 )
						{
							if ( inner )
								fail1 = true;
							else
								pass1 = true;
						}
						bool pattern2 = patt[0] == patt[2] && patt[0] != patt[1];
						if ( pattern2 )
						{
							if ( inner )
							{
								inside2.push_back( std::make_pair( patt[0], patt[1] ) );
							}
							else
							{
								outside2.push_back( std::make_pair( patt[1], patt[0] ) );
							}
						}
					}
				}
				if ( pass1 && !fail1 )
				{
					part1.push_back( std::string( thisLine ) );
				}
				bool pass2 = false;
				for ( auto in : inside2 )
				{
					for ( auto out : outside2 )
					{
						if ( in == out )
						{
							pass2 = true;
						}
					}
				}
				if ( pass2 )
				{
					part2.push_back( std::string( thisLine ) );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %zd\n", part1.size() );
	printf( "Part 2: %zd\n", part2.size() );
	return 0;
}
