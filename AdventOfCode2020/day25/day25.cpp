// day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int transform(int subject, int privateX)
{
	int64 i = 1;
	for (int step = 0; step < privateX; ++step)
	{
		i *= subject;
		i %= 20201227;
	}
	return (int)i;
}

int Calculate(int public1, int public2)
{
	int i = 1;
	int private1 = -1;
	int private2 = -1;
	for (int step = 0; private1 == -1 || private2 == -1; ++step)
	{
		i *= 7;
		i %= 20201227;

		if (i == public1)
			private1 = step+1;
		if (i == public2)
			private2 = step+1;
	}

	int key1 = transform(public1, private2);
	int key2 = transform(public2, private1);
	assert(key1 == key2);
	return key1;
}

int main()
{
	std::vector<int> keys;

	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (*thisLine)
				keys.push_back(atoi(thisLine));
		}
	}
	fclose( fp );
	delete[] buffer;

	assert(keys.size() == 2);

	printf("result: %d\n", Calculate(keys[0], keys[1]));
	return 0;
}
