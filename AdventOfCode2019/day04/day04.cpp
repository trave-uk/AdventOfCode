// day04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	int start = 240920;
	int end = 789857;
	int found = 0;
	for ( int i = start; i <= end; ++i )
	{
		char number[7];
		sprintf( number, "%d", i );
		char chr = '0';
		bool increase = true;
		bool doubles = false;
		int dupes = 0;
		for ( int j = 0; j < strlen( number ); ++j )
		{
			char newchr = number[j];
			if ( newchr < chr )
			{
				increase = false;
				break;
			}
			if ( newchr == chr )
			{
				++dupes;
			}
			else
			{
				if ( dupes == 1 )
					doubles = true;
				dupes = 0;
			}
			chr = newchr;
		}
		if ( dupes == 1 )
			doubles = true;
		if ( increase && doubles )
		{
			printf( "%d\n", i );
			++found;
		}
	}
	printf( "%d found\n", found );
	return 0;
}

