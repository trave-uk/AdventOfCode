// day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::vector<std::string> trees;

int countTrees( int dx, int dy )
{
	int total = 0;
	int x = 0;
	for ( int y = 0; y < trees.size(); y += dy, x += dx )
	{
		const std::string& line = trees[y];
		char p = line[x % line.length()];
		if ( p == '#' )
			++total;
	}
	printf( "Right %d, down %d: %d trees\n", dx, dy, total );
	return total;
}

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine && *thisLine)
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			std::string line( thisLine );
			trees.push_back( line );
		}
	}
	fclose( fp );
	delete[] buffer;

	long long int total1 = countTrees( 1, 1 );
	long long int total2 = countTrees( 3, 1 );
	long long int total3 = countTrees( 5, 1 );
	long long int total4 = countTrees( 7, 1 );
	long long int total5 = countTrees( 1, 2 );

	printf( "1: %d trees encountered\n", total2 );
	printf( "2: %lld", total1*total2*total3*total4*total5 );

	return 0;
}

