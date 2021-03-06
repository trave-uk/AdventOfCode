// day4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Read all lines in
// Sort the lines

// For each guard:
// store an array of 60 ints, and increment each one when guard is asleep
// (also store the number of times the guard has been on duty)
// As each minute is incremented, find max. Do both: highest number of times asleep, AND highest chance of sleeping (max / number of times)

struct Guard
{
	int id = -1;
	int shifts = 0;
	std::array<int, 60> minutes;
	int maxTimesAsleep = 0;
	int minuteMaxTimesAsleep = -1;
	int totalMinutesAsleep = 0;
};

int maxTimesAsleep = 0;
int guardMaxTimesAsleep = -1;
int minuteMaxTimesAsleep = -1;

int highestTotalMinutesAsleep = 0;
int guardHighestTotalMinutesAsleep = -1;

void work( Guard* currentGuard, int sleepMinute, int wakeMinute )
{
	for ( int i = sleepMinute; i < wakeMinute; ++i )
	{
		currentGuard->minutes[i]++;
		currentGuard->totalMinutesAsleep++;
		if ( currentGuard->totalMinutesAsleep > highestTotalMinutesAsleep )
		{
			highestTotalMinutesAsleep = currentGuard->totalMinutesAsleep;
			guardHighestTotalMinutesAsleep = currentGuard->id;
		}
		if ( currentGuard->minutes[i] > currentGuard->maxTimesAsleep )
		{
			currentGuard->maxTimesAsleep = currentGuard->minutes[i];
			currentGuard->minuteMaxTimesAsleep = i;
		}
		if ( currentGuard->maxTimesAsleep > maxTimesAsleep )
		{
			maxTimesAsleep = currentGuard->maxTimesAsleep;
			guardMaxTimesAsleep = currentGuard->id;
			minuteMaxTimesAsleep = currentGuard->minuteMaxTimesAsleep;
		}
	}
}

int main()
{
	char line[80];
	std::set<std::string> lines;
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			std::string thisString( thisLine );
			lines.insert( thisString );
		}
	}
	fclose( fp );

	std::map<int, Guard> guards;
	int guardId = -1;
	int sleepMinute = -1;
	Guard* currentGuard = nullptr;
	for ( std::string entry : lines )
	{
//		[1518 - 07 - 13 00:59] wakes up
//		[1518 - 06 - 16 00:49] falls asleep
//		[1518 - 08 - 17 00:01] Guard #3529 begins shift
		int year, month, day, hour, minute;
		sscanf( entry.c_str(), "[%d - %d - %d %d:%d]", &year, &month, &day, &hour, &minute );

		if ( strstr( entry.c_str(), "Guard #" ) != nullptr )
		{
			if ( sleepMinute != -1 && currentGuard )
			{
				work( currentGuard, sleepMinute, 60 );
			}
			guardId = atoi( entry.c_str() + strcspn( entry.c_str(), "#" ) + 1 );
			sleepMinute = -1;
			if ( guards.count( guardId ) == 0 )
			{
				Guard thisGuard = { guardId, 0 };
				guards.insert( std::pair<int, Guard>(guardId, thisGuard) );
			}
			std::map<int, Guard>::iterator iter = guards.find( guardId );
			currentGuard = &iter->second;
			assert( currentGuard );
			currentGuard->shifts++;
		}
		else if ( strstr( entry.c_str(), "falls asleep" ) != nullptr )
		{
			assert( sleepMinute == -1 );
			sleepMinute = minute;
		}
		else if ( strstr( entry.c_str(), "wakes up" ) != nullptr )
		{
			assert( sleepMinute != -1 );
			work( currentGuard, sleepMinute, minute );
			sleepMinute = -1;
		}
		else
		{
			printf( "Unhandled line %s\n", entry.c_str() );
			continue;
		}
	}
	std::map<int, Guard>::iterator iter = guards.find( guardMaxTimesAsleep );
	currentGuard = &iter->second;

	printf( "Guard %d was asleep %d times at minute %d\n", guardMaxTimesAsleep, maxTimesAsleep, minuteMaxTimesAsleep );
	printf( "%d * %d = %d\n", guardMaxTimesAsleep, minuteMaxTimesAsleep, guardMaxTimesAsleep * minuteMaxTimesAsleep );


	float highestChanceAsleep = 0.0f;
	int guardHighestChanceAsleep = -1;
	int minuteHighestChanceAsleep = -1;
	int shiftsHighestChanceAsleep = -1;
	int maxTimesHighestChanceAsleep = -1;
	for ( std::pair<int, Guard> iter : guards )
	{
		Guard& guard = iter.second;
		float chanceAsleep = float( guard.maxTimesAsleep ) / float( guard.shifts );
		if ( chanceAsleep > highestChanceAsleep )
		{
			highestChanceAsleep = chanceAsleep;
			guardHighestChanceAsleep = guard.id;
			minuteHighestChanceAsleep = guard.minuteMaxTimesAsleep;
			shiftsHighestChanceAsleep = guard.shifts;
			maxTimesHighestChanceAsleep = guard.maxTimesAsleep;
			currentGuard = &guard;
		}
	}

	printf( "Guard %d had a %.2f chance of being asleep (%d times out of %d) at minute %d\n", guardHighestChanceAsleep, highestChanceAsleep, maxTimesHighestChanceAsleep, shiftsHighestChanceAsleep, minuteHighestChanceAsleep );
	printf( "%d * %d = %d\n", guardHighestChanceAsleep, minuteHighestChanceAsleep, guardHighestChanceAsleep * minuteHighestChanceAsleep );

	{
		std::map<int, Guard>::iterator iter = guards.find( guardHighestTotalMinutesAsleep );
		currentGuard = &iter->second;
		printf( "Guard %d was asleep %d times altogether, maximum %d at minute %d\n", guardHighestTotalMinutesAsleep, highestTotalMinutesAsleep, currentGuard->maxTimesAsleep, currentGuard->minuteMaxTimesAsleep );
		printf( "%d * %d = %d\n", guardHighestTotalMinutesAsleep, currentGuard->minuteMaxTimesAsleep, guardHighestTotalMinutesAsleep * currentGuard->minuteMaxTimesAsleep );
	}
	return 0;
}

