// day02.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 26/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Keypad = std::array<std::array<char, 5>, 5>;

Keypad p1keypad =
{{
	{{ 0,  0,   0,   0,  0 }},
	{{ 0, '1', '2', '3', 0 }},
	{{ 0, '4', '5', '6', 0 }},
	{{ 0, '7', '8', '9', 0 }},
	{{ 0,  0,   0,   0,  0 }}
}};

Keypad p2keypad =
{{
	{{  0,   0,  '1',  0,   0 }},
	{{  0,  '2', '3', '4',  0 }},
	{{ '5', '6', '7', '8', '9' }},
	{{  0,  'A', 'B', 'C',  0 }},
	{{  0,   0,  'D',  0,   0 }}
}};

int main()
{
	for ( int part = 1; part <= 2; ++part )
	{
		const Keypad& keypad = ( part == 1 ) ? p1keypad : p2keypad;
		int x = ( part == 1 ) ? 2 : 0;
		int y = 2;
		char* buffer = new char[65536];

		//FILE *fp = fopen( "example.txt", "rt" );
		FILE *fp = fopen( "input.txt", "rt" );
		std::string code;
		while ( !feof( fp ) )
		{
			char* thisLine = fgets( buffer, 65536, fp );
			if ( thisLine )
			{
				thisLine[strcspn( thisLine, "\n\r" )] = '\0';
				if ( *thisLine )
				{
					std::string instructions( thisLine );
					for ( char i : instructions )
					{
						switch ( i )
						{
							case 'L':
							{
								if ( x > 0 && keypad[y][x-1] )
								{
									--x;
								}
								break;
							}
							case 'R':
							{
								if ( x < 4 && keypad[y][x + 1] )
								{
									++x;
								}
								break;
							}
							case 'U':
							{
								if ( y > 0 && keypad[y - 1][x] )
								{
									--y;
								}
								break;
							}
							case 'D':
							{
								if ( y < 4 && keypad[y + 1][x] )
								{
									++y;
								}
								break;
							}
							default:
							{
								assert( false );
							}
						}
					}
					code.push_back( keypad[y][x] );
				}
			}
		}
		fclose( fp );
		delete[] buffer;

		printf( "Part %d: %s\n", part, code.data() );
	}
	return 0;
}
