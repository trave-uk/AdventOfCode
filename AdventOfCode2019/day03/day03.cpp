// day03.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define WIDTH 1024
#define HEIGHT 1024

const int size = 32768;
const int halfsize = size / 2;

int main()
{
	FILE *fp = fopen( "input.txt", "rt" );
	char line[2048];
	int* map = (int*)calloc( size, size*sizeof(int) );
	int lineid = 1;
	int closest = size * size;
	int max1 = 0;
	while ( !feof( fp ) && lineid <= 2 )
	{
		int x = 0;
		int y = 0;
		int steps = 0;
		char* thisLine = fgets( line, 2048, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
		}
		// comma separated
		char* tok = strtok( line, ", " );
		while ( tok )
		{
			assert( lineid <= 2 );
			int distance = atoi( tok+1 );
			int dx = 0, dy = 0;
			switch ( tok[0] )
			{
				case 'U': dy = -1; break;
				case 'D': dy = 1; break;
				case 'L': dx = -1; break;
				case 'R': dx = 1; break;
				default:
					printf( "Unknown token: %s", tok );
					break;
			}
			for ( int i = 0; i < distance; ++i )
			{
				x += dx; y += dy; steps += 1;
				assert( abs( x ) < halfsize && abs( y ) < halfsize );
				int index = (y + halfsize) * size + (x + halfsize);
				int value = (lineid - 1)*max1 + steps;
				if ( lineid == 1 )
				{
					max1 = steps;
				}
				else if ( map[index] != 0 && map[index] < max1 )
				{
					int totalsteps = steps + map[index];
					if ( totalsteps < closest )
					{
						closest = totalsteps;
					}
					printf( " Cross found (%d, %d) (steps %d; closest now %d)\n", x, y, totalsteps, closest );
				}
				map[index] = value;
			}
			
			tok = strtok( nullptr, ", " );
		}
		++lineid;
	}
	fclose( fp );
	free( map );
	printf( "Closest distance: %d\n", closest );
	return 0;
}

