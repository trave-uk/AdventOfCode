// day4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	char line[80];
	int validCount1 = 0;
	int validCount2 = 0;
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			std::set<std::string> words;
			std::set<std::string> words2;
			bool valid = true;
			bool valid2 = true;
			char *word = strtok( thisLine, " " );
			do 
			{
				std::string thisWord( word );
				if ( words.count( thisWord ) > 0 )
					valid = false;
				words.insert( thisWord );

				std::sort( thisWord.begin(), thisWord.end() );
				if ( words2.count( thisWord ) > 0 )
					valid2 = false;
				words2.insert( thisWord );
			} while ( word = strtok( nullptr, " " ) );
			if ( valid )
				++validCount1;
			if ( valid2 )
				++validCount2;
		}
	}
	fclose( fp );

	printf( "Part 1: There are %d valid passphrases\n", validCount1 );
	printf( "Part 2: There are %d valid passphrases\n", validCount2 );
	return 0;
}

