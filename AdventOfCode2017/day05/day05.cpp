// day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::vector<int> numbers;
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			int number = atoi( thisLine );
			numbers.push_back( number );
		}
	}
	fclose( fp );
	delete[] buffer;

	int index = 0;
	int steps = 0;
	int size = static_cast<int>(numbers.size());
	while ( index < size )
	{
		++steps;
		int newIndex = index + numbers[index];
		// for part 1, just increment here:
		if ( numbers[index] >= 3 )
			--numbers[index];
		else
			++numbers[index];
		index = newIndex;
	}
	printf( "%d steps: %d (target %d)\n", steps, index, size );
	getchar();
	return 0;
}
