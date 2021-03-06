// day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct password
{
	int min;
	int max;
	char req;
	char password[80];
};

bool checkPassword1( const password& p )
{
	int count = 0;
	for ( int i = 0; i < strlen( p.password ); ++i )
	{
		if ( p.password[i] == p.req )
			++count;
	}
	return ( count >= p.min && count <= p.max );
}

bool checkPassword2( const password& p )
{
	int count = 0;
	for ( int i = p.min; i <= p.max; i += p.max-p.min)
	{
		if ( p.password[i-1] == p.req )
			++count;
	}
	return ( count == 1 );
}

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int total1 = 0;
	int total2 = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			password p;
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			sscanf( thisLine, "%d-%d %c: %s", &p.min, &p.max, &p.req, p.password );
			if ( checkPassword1( p ) )
			{
				++total1;
			}
			if ( checkPassword2( p ) )
			{
				++total2;
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	printf( "1: %d valid passwords\n", total1 );
	printf( "2: %d valid passwords\n", total2 );

	return 0;
}

