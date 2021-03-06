// day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


std::vector<int> banks;
int sum = 0;

bool load( const char *filename )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				char *token = strtok( thisLine, " \t" );
				while ( token )
				{
					int number = atoi( token );
					banks.push_back( number );
					sum += number;
					token = strtok( nullptr, " \t" );
				}
			}
		}
	}
	printf( "Number of banks loaded: %lld, sum: %d\n", banks.size(), sum );
	return true;
}

void process_banks( std::vector<int>& banks )
{
	// Find biggest bank
	int bank_count = banks.size();
	int most = 0;
	int bank = -1;
	for ( int i = 0; i < bank_count; ++i )
	{
		if ( banks[i] > most )
		{
			most = banks[i];
			bank = i;
		}
	}
	// Redistribute these
	banks[bank] = 0;
	for ( int i = 0; i < most; ++i )
	{
		++bank;
		bank %= bank_count;
		++banks[bank];
	}
}

void process()
{
	std::vector<int> banks1 = banks;
	std::vector<int> banks2 = banks;

	int steps1 = 0, steps2 = 0;
	while ( 1 )
	{
		process_banks( banks2 );
		++steps2;
		if ( banks1 == banks2 )
			break;
		process_banks( banks2 );
		++steps2;
		if ( banks1 == banks2 )
			break;
		process_banks( banks1 );
		++steps1;
		if ( banks1 == banks2 )
			break;
	}
	printf( "Repetition detected at step %d (with step %d, so period %d)\n", steps2, steps1, steps2 - steps1 );
}

INT64 hash( const std::vector<int>& banks )
{
	// bank[0] + bank[1] * sum + bank[2] * sum * sum + ...
	INT64 total = 0;
	INT64 multiplier = 1;

	for ( int bank : banks )
	{
		total += bank * multiplier;
		multiplier *= 16;
	}
	return total;
}

std::set<INT64> hashes;

void process2()
{
	INT64 repeat = 1311786128638799361L;
	int steps = 0;
	int seen = -1;
	while ( 1 )
	{
		process_banks( banks );
		++steps;
		INT64 test = hash( banks );
		if ( seen == -1 && test == repeat )
		{
			printf( "Hash seen at %d\n", steps );
			seen = steps;
		}
		if ( hashes.count( test ) >= 1 )
		{
			printf( "Hash %lld seen before\n", test );
			break;
		}
		hashes.insert( test );
	}
	printf( "Repetition detected at step %d, with period %d\n", steps, steps - seen );
}

int main()
{
//	if ( load( "example.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		printf( "Press return to continue...\n" );
		getchar();
		process2();
		printf( "Press return to continue...\n" );
		getchar();
	}
	return 0;
}

