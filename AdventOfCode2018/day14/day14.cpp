// day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int input = 607331;

const char * const start = "37";

int constexpr length( const char* str )
{
	return *str ? 1 + length( str + 1 ) : 0;
}

static constexpr int elf_count = length( start );

int main()
{
	std::array<int, elf_count> elves;
	std::vector<int> recipes;
	std::array<int, 5> tests2 = { 51589, 1245, 92510, 59414, input };

	std::string xStart( start );
	for ( auto x : xStart )
	{
		elves[recipes.size()] = recipes.size();
		recipes.push_back( x - '0' );
	}

	bool finished1 = false, finished2 = false;
	int len = recipes.size();
	while ( !finished1 || !finished2 )
	{
		int sum = 0;
		// Add new recipes
		for ( int elf : elves )
		{
			sum += recipes[elf];
		}
		std::string xNewRecipes = std::to_string( sum );
		for ( auto x : xNewRecipes )
		{
			recipes.push_back( x - '0' );
		}

		// Now each elf moves on by their current recipe
		for ( int elf = 0; elf < elves.size(); ++elf )
		{
			int index = elves[elf];
			index += recipes[index] + 1;
			index %= recipes.size();
			elves[elf] = index;
		}
		std::array<int, 5> tests = { 5, 9, 18, 2018, input };
		int newlen = recipes.size();
		for ( auto test : tests )
		{
			if ( len < test+10 && newlen >= test+10 )
			{
				printf( "After %d digits, the next ten digits are: ", test );
				for ( int index = test; index < test + 10; ++index )
				{
					int recipe = recipes[index];
					putchar( recipe + '0' );
				}
				putchar( '\n' );
				if ( test == input )
					finished1 = true;
			}
		}
		if ( newlen >= 10 )
		{
			for ( int t = 0; t < tests2.size(); ++t )
			{
				int test = tests2[t];
				if ( test == 0 )
					continue;
				std::string testString = std::to_string( test );
				while ( testString.length() < 5 )
				{
					testString = std::string( "0" ) + testString;
				}
				for ( int index = newlen - 10; index < newlen - 5; ++index )
				{
					bool success = true;
					for ( int index2 = index; index2 < index + testString.length(); ++index2 )
					{
						if ( recipes[index2] != testString[index2 - index] - '0' )
						{
							success = false;
							break;
						}
					}
					if ( success )
					{
						printf( "Found '%s' after %d recipes\n", testString.c_str(), index );
						if ( tests2[t] == input )
							finished2 = true;
						tests2[t] = 0;
						break;
					}
				}
			}
		}
		len = newlen;
	}
	getchar();
	return 0;
}

