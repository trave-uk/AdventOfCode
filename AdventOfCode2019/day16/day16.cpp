// day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::vector<char> input;
std::vector<char> copy;

void process( std::vector<char>& input, int offset )
{
	int limit = max( offset, input.size() / 2 );
	static const std::array<int, 4> base = { 0, 1, 0, -1 };
	for ( int i = offset; i < limit; ++i )
	{
		int total = 0;
		for ( int j = i; j < input.size(); ++j )
		{
			int index = ( ( j + 1 ) / ( i + 1 ) ) % 4;
			total += int( input[j] ) * base[index];
		}
		total = abs( total );
		total %= 10;
		input[i] = total;
	}
	for ( int i = input.size() - 2; i >= limit; --i )
	{
		input[i] += input[i + 1];
		input[i] %= 10;
	}
}

int main()
{
	char* line = new char[10000];
//	FILE *fp = fopen( "example1.txt", "rt" );
//	FILE *fp = fopen( "example2.txt", "rt" );
//	FILE *fp = fopen( "example3.txt", "rt" );
//	FILE *fp = fopen( "example4.txt", "rt" );
//	FILE *fp = fopen( "example5.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 10000, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			while ( *thisLine )
			{
				input.push_back( *thisLine - '0' );
				++thisLine;
			}
		}
	}
	// for part 2, get the offset from the first 7 digits of the input.
	line[7] = '\0';
	int offset = atoi( line );
	fclose( fp );
	delete[] line;

	// for part 2, duplicate the data 10k times!
// 	input.reserve( input.size() * 10000 );
// 	copy = input;
// 	for ( int i = 0; i < 9999; ++i )
// 	{
// 		for ( int c : copy )
// 		{
// 			input.push_back( c );
// 		}
// 	}
	// for part 1, don't duplicate the data and use offset of 0
	offset = 0;
	for ( int stage = 1; stage <= 100; ++stage )
	{
		process( input, offset );
		printf( "%d: %d%d%d%d%d%d%d%d\n", stage, input[offset], input[offset + 1], input[offset + 2], input[offset + 3], input[offset + 4], input[offset + 5], input[offset + 6], input[offset + 7] );
	}
}
