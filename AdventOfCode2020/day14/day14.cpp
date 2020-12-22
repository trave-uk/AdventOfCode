// day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = int64;

std::map<Value, Value> memory;
std::map<Value, Value> memory2;

int main()
{
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	//FILE *fp = fopen( "example2.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	Value maskAnd = 0x7ffffffff;
	std::vector<Value> andBitValues;
	Value maskOr = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (*thisLine)
			{
				if (strncmp(thisLine, "mask = ", 7) == 0)
				{
					maskAnd = 0x7ffffffff;
					maskOr = 0;
					andBitValues.clear();
					Value bitValue = 1;
					assert(thisLine[43] == '\0');
					for (int i = 0; i < 36; ++i)
					{
						char bit = thisLine[42 - i];
						if (bit != 'X')
						{
							maskAnd = maskAnd & ~bitValue;
							if (bit == '1')
								maskOr = maskOr | bitValue;
						}
						else
						{
							andBitValues.push_back(bitValue);
						}
						bitValue = bitValue << 1;
					}
				}
				else
				{
					Value address;
					Value val;
					int matches = sscanf(thisLine, "mem[%lld] = %lld", &address, &val);
					assert(matches == 2);

					// Part 1
					Value valPart1 = val & maskAnd;
					valPart1 = valPart1 | maskOr;
					if (memory.count(address) > 0)
					{
						memory[address] = valPart1;
					}
					else
					{
						memory.insert({ address, valPart1 });
					}
					// Part 2
					address = address | maskOr;
					std::vector<Value> addresses;
					addresses.push_back(address);
					for (Value bitValue : andBitValues)
					{
						std::vector<Value> newAddresses;
						for (Value a : addresses)
						{
							a = a ^ bitValue;
							newAddresses.push_back(a);
						}
						for (Value a : newAddresses)
						{
							addresses.push_back(a);
						}
					}
					for (Value a : addresses)
					{
						if (memory2.count(a) > 0)
						{
							memory2[a] = val;
						}
						else
						{
							memory2.insert({ a, val });
						}
					}
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// Part 1
	Value part1 = 0;
	for (auto it : memory)
	{
		part1 += it.second;
	}

	printf( "Part 1: %lld\n", part1 );

	// Part 2
	Value part2 = 0;
	for (auto it : memory2)
	{
		part2 += it.second;
	}

	printf("Part 2: %lld\n", part2);
	return 0;
}
