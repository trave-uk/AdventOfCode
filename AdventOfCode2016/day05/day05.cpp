// day05.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 27/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Process( std::string input )
{
	std::string password1;
	std::string password2;
	password2.assign( 8, '_' );
	int p2chars = 0;
	MD5 md5;

	for ( int i = 0; p2chars < 8; ++i )
	{
		std::string s = input + std::to_string( i );
		char* h = md5.digestString( &(*s.begin()) );
		if ( strncmp( h, "00000", 5 ) == 0 )
		{
			// part 1
			if ( password1.length() < 8 )
			{
				password1.push_back( h[5] );
			}
			// part 2
			int index = h[5] - '0';
			if ( index >= 0 && index <= 7 && password2[index] == '_' )
			{
				password2[index] = h[6];
				++p2chars;
			}
		}
	}
	printf( "Door ID: %s, part 1 %s, part 2 %s\n", input.data(), password1.data(), password2.data() );
}

int main()
{
	Process( "abc" );
	Process( "ffykfhsq" );
	return 0;
}
