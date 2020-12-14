// day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int start = 0;
struct Bus
{
	Bus(int n, int i) { num = n; index = i; }
	int num;
	int index;
};
std::vector<Bus> buses;
int highest = -1;
int highest_index = 0;
__int64 earliest = 0;

int main()
{
	int result = 0;
	char* buffer = new char[65536];
//	FILE *fp = fopen("example.txt", "rt"); earliest = 1000;
	FILE *fp = fopen("input.txt", "rt"); earliest = 1000000000000000l;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (!start)
			{
				start = atoi(thisLine);
			}
			else
			{
				assert(buses.empty());
				int index = 0;
				char* bus = strtok(thisLine, ",");
				while (bus)
				{
					if (*bus != 'x')
					{
						assert(isdigit(*bus));
						int b = atoi(bus);
						buses.push_back(Bus(b, index));
						if (b > highest)
						{
							highest = b;
							highest_index = index;
						}
					}
					bus = strtok(nullptr, ",");
					++index;
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	for (int time = start; result == 0; ++time)
	{
		for (const Bus& bus : buses)
		{
			if ((time % bus.num) == 0)
			{
				result = (time - start) * bus.num;
				break;
			}
		}
	}
	printf( "Part 1: %d\n", result );

	__int64 part2 = 0;
	
	for (__int64 time = (earliest/highest + 1) * highest; part2 == 0; time += highest)
	{
		__int64 st = time - highest_index;
		int index = 0;
		bool fail = false;
		for (const Bus& bus : buses)
		{
			__int64 t = st + bus.index;
			if ((t % bus.num) != 0)
			{
				fail = true;
				break;
			}
		}
		if (!fail)
		{
			part2 = st;
		}
	}
	printf("Part 2: %lld\n", part2);
	return 0;
}
