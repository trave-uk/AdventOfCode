// day18.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 22/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int Process(std::string title, const char* filename, int numRows)
{
	std::string input;
	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				input = std::string(thisLine);
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int count = 0;
	std::string lastRow = input;
	for (int row = 0; row < numRows; ++row)
	{
		std::string nextRow;
		if (!row)
		{
			nextRow = lastRow;
		}
		else
		{
			for (int i = 0; i < lastRow.length(); ++i)
			{
				bool leftTrap = (i > 0 && lastRow[i - 1] == '^');
				bool rightTrap = (i < lastRow.length() - 1 && lastRow[i + 1] == '^');
				if (leftTrap == rightTrap)
				{
					nextRow.append(1, '.');
				}
				else
				{
					nextRow.append(1, '^');
				}
			}
		}
		for (char c : nextRow)
		{
			if (c == '.')
			{
				++count;
			}
		}
		lastRow = nextRow;
	}

	printf( "%s: %d\n", title.c_str(), count );
	return 0;
}

int main()
{
	Process("Example", "example.txt", 10);
	Process("Part 1", "input.txt", 40);
	Process("Part 2", "input.txt", 400000);
}