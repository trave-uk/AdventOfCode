// day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	std::set<int> inputs;
	int result = 0;
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example1.txt", "rt" );
//	FILE *fp = fopen( "example2.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (*thisLine)
			{
				inputs.insert(atoi(thisLine));
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// Part 1
	int x = 0;
	int n1 = 0;
	int n3 = 1; // device included
	for (int j : inputs)
	{
		int n = j - x;
		if (n == 1)
			++n1;
		else if (n == 3)
			++n3;
		x = j;
	}
	result = n1 * n3;
	printf( "Part 1: %d\n", result );

	// Part 2
	// Number of combinations of adaptors that can be made
	// * space can be 1, 2 or 3 between adaptors
	// * if already 3, then separates sections
	// * get separate lists for all sections (include 0 in the first section, but the device is in a section on its own so can be ignored)
	// * don't bother storing any other sections of length 1 either
	// * work out per section how many combinations work
	// * multiply values for each section together, using an __int64 (trillions!).
	x = 0;
	int maxsize = 0;
	std::vector<std::set<int>> sections;
	std::set<int> section;
	section.insert(x);
	for (int j : inputs)
	{
		int n = j - x;
		if (n == 3)
		{
			// close section
			if (section.size() > 2)
				sections.push_back(section);
			if (section.size() > maxsize)
				maxsize = section.size();
			section.clear();
		}
		section.insert(j);
		x = j;
	}
	if (section.size() > 2)
	{
		sections.push_back(section);
	}
	printf("...%d sections\n", sections.size());
	printf("...longest section %d values\n", maxsize);

	__int64 part2 = 1;
	for (std::set<int> section : sections)
	{
		// In each section, which combinations of entries in the middle can be removed but still be a valid list?
		// power of two possibilities: 2^(size-2)

		int possible = 1 << (section.size()-2);
		__int64 actual = 1;
		// now check each possibility, skipping 0 which will pass
		for (int i = 1; i < possible; ++i)
		{
			std::set<int> test = section;
			int bit = -1;
			for (int j : section)
			{
				if (j > *section.begin() && (i & (1 << bit)))
				{
					test.erase(j);
				}
				++bit;
			}
			int y = -1;
			bool pass = true;
			for (int j : test)
			{
				if (y == -1)
					y = j;
				else
				{
					int n = j - y;
					if (n > 3)
					{
						pass = false;
						break;
					}
					y = j;
				}
			}
			if (pass)
			{
				++actual;
			}
		}
		part2 *= actual;
	}
	printf("Part 2: %lld\n", part2);
	return 0;
}
