// day10.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 30/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Bot
{
	bool highOutput = false;
	int highId = -1;
	bool lowOutput = false;
	int lowId = -1;
	std::set<int> values;
};

std::map<int, Bot> bots;
std::map<int, int> outputs;

void ProcessBot( int botId );

void GiveValue( bool output, int id, int value )
{
	if ( output )
	{
		outputs[id] = value;
	}
	else
	{
		Bot& bot = bots[id];
		bot.values.insert( value );
		ProcessBot( id );
	}
}

void ProcessBot( int botId )
{
	Bot& bot = bots[botId];
	if ( bot.highId == -1 || bot.lowId == -1 || bot.values.size() < 2 )
	{
		return;
	}
	auto it = bot.values.begin();
	int low = *it;
	it++;
	int high = *it;
	bot.values.clear();
	GiveValue( bot.lowOutput, bot.lowId, low );
	GiveValue( bot.highOutput, bot.highId, high );
	if ( ( low == 17 && high == 61 ) || ( low == 2 && high == 5 ) )
	{
		printf( "Part 1: bot %d comparing %d and %d\n", botId, low, high );
	}
}

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				// "bot 128 gives low to bot 56 and high to bot 91"
				// "value 23 goes to bot 8"
				char lowType[7];
				char highType[7];
				int thisBot, lowId, highId;
				int value;
				if ( sscanf( thisLine, "bot %d gives low to %s %d and high to %s %d", &thisBot, lowType, &lowId, highType, &highId ) == 5 )
				{
					Bot& bot = bots[thisBot];
					bot.highId = highId;
					bot.lowId = lowId;
					bot.highOutput = highType[0] == 'o';
					bot.lowOutput = lowType[0] == 'o';
					ProcessBot( thisBot );
				}
				else if ( sscanf( thisLine, "value %d goes to bot %d", &value, &thisBot ) == 2 )
				{
					GiveValue( false, thisBot, value );
				}
				else
				{
					assert( false );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 2: %d\n", outputs[0] * outputs[1] * outputs[2] );
	return 0;
}
