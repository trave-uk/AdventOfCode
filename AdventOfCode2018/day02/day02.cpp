// day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int count_differences( const char* line1, const char* line2 )
{
	int len1 = static_cast<int>(strlen( line1 ));
	int len2 = static_cast<int>(strlen( line2 ));
	int len = std::min( len1, len2 );
	int diffs = abs( len1 - len2 );
	for ( int i = 0; i < len; ++i )
	{
		if ( line1[i] != line2[i] )
			++diffs;
	}
	return diffs;
}

int main()
{
	char line[80];
	int instancesOf2 = 0;
	int instancesOf3 = 0;
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			std::map<char, int> chars;
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// count number of occurrences of each character in line
			for ( int i = 0; i < strlen( thisLine ); ++i )
			{
				char key = thisLine[i];
				auto iter = chars.find( key );
				if ( iter == chars.end() )
				{
					chars.insert( std::pair<char, int>( key, 1 ) );
				}
				else
				{
					chars.insert_or_assign( key, iter->second + 1 );
				}
			}
			// Now find if any characters occur 2 or 3 times;
			for ( std::pair<char, int> element : chars )
			{
				if ( element.second == 2 )
				{
					++instancesOf2;
					break;
				}
			}
			for ( std::pair<char, int> element : chars )
			{
				if ( element.second == 3 )
				{
					++instancesOf3;
					break;
				}
			}
		}
	}
	fclose( fp );
	printf( " Checksum = %d * %d = %d\n", instancesOf2, instancesOf3, instancesOf2*instancesOf3 );

	std::set<std::string> strings;
	fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char *thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			std::string thisString( thisLine );
			for ( std::string testString : strings )
			{
				int diffs = count_differences( thisString.c_str(), testString.c_str() );
				if ( diffs == 1 )
				{
					printf( "Found a match:\n\t%s\n\t%s\n", thisString.c_str(), testString.c_str() );
				}
			}
			strings.insert( thisString );
		}
	}
	fclose( fp );
	return 0;
}

