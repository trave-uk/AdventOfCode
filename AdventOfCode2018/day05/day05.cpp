// day5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int testLetter(char letter)
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	if ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		assert( thisLine );
		thisLine[strcspn( thisLine, "\n\r" )] = '\0';
	}
	fclose( fp );

	int len;
	bool bFinished = false;
	while (!bFinished)
	{
		// Find any matching pairs
		len = static_cast<int>(strlen( buffer ));
		bFinished = true;
		for ( int i = 0; i < len-1; ++i )
		{
			if ( tolower( buffer[i] ) == tolower( letter ) )
			{
				memcpy( buffer + i, buffer + i + 1, len - i );
				--len;
			}
			if ( buffer[i] != buffer[i + 1] && tolower( buffer[i] ) == tolower( buffer[i + 1] ) )
			{
				memcpy( buffer + i, buffer + i + 2, len - i );
				len -= 2;
				--i;
				bFinished = false;
			}
		}
	}
	printf( "Removing '%c', length is %d\n", letter, len );
	return len;
}

int main()
{
	int min = 50000;
	char letter = '\0';
	for ( char c = 'a'; c <= 'z'; ++c )
	{
		int this1 = testLetter( c );
		if ( this1 < min )
		{
			min = this1;
			letter = c;
		}
	}
	printf( "Lowest: %d (removing '%c')\n", min, letter );
}