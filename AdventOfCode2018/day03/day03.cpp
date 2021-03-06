// day3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define WIDTH 1024
#define HEIGHT 1024

int main()
{
	const size_t size = sizeof( int ) * WIDTH * HEIGHT;
	int *claims = static_cast<int*>(malloc( size ));
	memset( claims, 0, size );
	int count = 0;
	char line[80];
	std::set<int> goodIds;
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			int id, left, top, width, height;
			sscanf( thisLine, "#%d @ %d,%d: %dx%d", &id, &left, &top, &width, &height );
			goodIds.insert( id );
			assert( left + width <= WIDTH );
			assert( top + height <= HEIGHT );
			for ( int x = left; x < left + width; ++x )
			{
				for ( int y = top; y < top + height; ++y )
				{
					int index = y * WIDTH + x;
					int previous_id = claims[index];
					if ( previous_id != 0 )
					{
						if ( previous_id != -1 )
						{
							++count;
							claims[index] = -1;
							goodIds.erase( previous_id );
						}
						goodIds.erase( id );
					}
					else
					{
						claims[index] = id;
					}
				}
			}
		}
	}
	fclose( fp );
	free( claims );
	printf( "%d re-used tiles\n", count );
	printf( "%d good IDs:\n", static_cast<int>(goodIds.size()) );
	for ( int id : goodIds )
	{
		printf( "\t%d\n", id );
	}
	return 0;
}

