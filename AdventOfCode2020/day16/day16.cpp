// day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Range
{
	int start;
	int end;

	bool match(int val)
	{
		return start <= val && val <= end;
	}
};

struct Rule
{
	std::string name;
	Range ranges[2];

	int index = -1;

	bool match(int val)
	{
		return ranges[0].match(val) || ranges[1].match(val);
	}
};

using Ticket = std::vector<int>;

std::vector<Rule> rules;
Ticket yourTicket;
std::vector<Ticket> tickets;

int main()
{
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	//FILE *fp = fopen( "example2.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	bool your = false;
	bool nearby = false;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			
			// zone: 36-909 or 927-970
			Rule r;
			char name[80];
			int found = sscanf(thisLine, "%[^:]: %d-%d or %d-%d", name, &r.ranges[0].start, &r.ranges[0].end, &r.ranges[1].start, &r.ranges[1].end);
			if (found == 5)
			{
				r.name = name;
				rules.push_back(r);
			}
			else if (*thisLine)
			{
				if (strcmp(thisLine, "your ticket:") == 0)
				{
					your = true;
				}
				else if (strcmp(thisLine, "nearby tickets:") == 0)
				{
					your = false;
					nearby = true;
				}
				else
				{
					// Assume this is a ticket.
					// 446,499,748,453,135,109,525,721,179,796,622,944,175,303,882,287,177,185,828,423
					Ticket t;
					char* val = strtok(thisLine, ",");
					while (val)
					{
						t.push_back(atoi(val));
						val = strtok(nullptr, ",");
					}
					assert(t.size() == rules.size());
					if (your)
					{
						yourTicket = t;
					}
					else
					{
						assert(nearby);
						tickets.push_back(t);
					}
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// Part 1
	int part1 = 0;
	for (auto iter = tickets.begin(); iter != tickets.end(); )
	{
		Ticket& t = *iter;
		bool remove = false;
		for (int val : t)
		{
			bool found = false;
			for (Rule r : rules)
			{
				if (r.match(val))
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				part1 += val;
				// remove this ticket
				remove = true;
			}
		}
		if (remove)
		{
			iter = tickets.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	printf( "Part 1: %d\n", part1 );

	// Part 2:
	std::set<int> knownIndices;
	while (knownIndices.size() < rules.size())
	{
		bool stuck = true;
		for (int i = 0; i < rules.size(); ++i)
		{
			std::set<int> test;
			for (int t = 0; t < rules.size(); ++t)
			{
				if (knownIndices.count(t) == 0)
					test.insert(t);
			}

			for (Ticket t : tickets)
			{
				int ri = 0;
				for (Rule r : rules)
				{
					if (!r.match(t[i]))
					{
						test.erase(ri);
					}
					++ri;
				}
			}
			if (test.size() == 1)
			{
				int ri = *test.begin();
				Rule &r = rules[ri];
				assert(r.index == -1);
				r.index = i;
				knownIndices.insert(ri);
				stuck = false;
			}
		}
		assert(!stuck);
	}

	__int64 part2 = 1L;
	for (Rule r : rules)
	{
		if (r.name.find("departure") == 0)
		{
			part2 *= yourTicket[r.index];
		}
	}

	printf("Part 2: %lld\n", part2);

	return 0;
}
