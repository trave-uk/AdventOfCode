// day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using i64 = long long;

struct rule_type
{
	std::set<i64> offsetsWithPlants;
	bool result = false;
};
std::set<i64> potsWithPlants;
std::vector<rule_type> rules;
i64 minPot = 0;
i64 maxPot = 0;

void print_row( i64 gen )
{
	printf( "%0zd: ", gen );
	for ( i64 pot = -10; pot < maxPot; ++pot )
	{
		if ( potsWithPlants.count( pot ) > 0 )
			putchar( '#' );
		else
			putchar( '.' );
	}
	putchar( '\n' );
}

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';

			if ( strncmp( thisLine, "initial state", 13 ) == 0 )
			{
				thisLine += strcspn(thisLine, "#.");
				i64 pot = 0;
				while ( *thisLine )
				{
					if ( *thisLine == '#' )
					{
						potsWithPlants.insert( pot );
						maxPot = pot;
					}
					++thisLine;
					++pot;
				}
			}
			else if ( *thisLine )
			{
				rule_type rule;
				char pattern[10];
				char result;
				sscanf( thisLine, "%s => %c", pattern, &result );
				rule.result = (result == '#');
				for ( i64 offset = -2; offset <= 2; ++offset )
				{
					if ( pattern[offset + 2] == '#' )
						rule.offsetsWithPlants.insert( offset );
				}
				rules.push_back( rule );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	i64 gen;
	for ( gen = 0; gen < 1000; ++gen )
	{
		if ( gen == 20 )
		{
			print_row( gen );
			i64 total = 0;
			for ( i64 pot : potsWithPlants )
			{
				total += pot;
			}
			printf( "Result 1 = %zd\n", total );
		}
		std::set<i64> newPotsWithPlants;
		i64 nextMaxPot = maxPot;
		for ( i64 pot = minPot - 2; pot <= maxPot + 2; ++pot )
		{
			std::set<i64> offsetsWithPlants;
			for ( i64 offset = -2; offset <= 2; ++offset )
			{
				if ( potsWithPlants.count( pot + offset ) > 0 )
				{
					offsetsWithPlants.insert( offset );
				}
			}
			// Find the rule that matches this pot;
			for ( rule_type rule : rules )
			{
				if ( rule.offsetsWithPlants == offsetsWithPlants )
				{
					if ( rule.result )
					{
						newPotsWithPlants.insert( pot );
						if ( pot < minPot )
							minPot = pot;
						if ( pot > nextMaxPot )
							nextMaxPot = pot;
					}
				}
			}
		}
		maxPot = nextMaxPot;
		potsWithPlants = newPotsWithPlants;
	}
	print_row( gen );

	i64 total = 0;
	for ( i64 pot : potsWithPlants )
	{
		// After 1000 generations (actually much less) the pattern is fixed, and the plants move one place to the right on successive generations.
		total += pot + 50000000000 - 1000;
	}
	printf( "Result 2 = %zd\n", total );
	getchar();
}

