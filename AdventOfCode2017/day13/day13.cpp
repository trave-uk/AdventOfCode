// day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct layer
{
	layer() {}
	layer( int _depth ) : depth( _depth ) {}
	int depth = 0;
	int position = 0;
	int direction = 1;
};

std::map<int, layer> layers;
int totaldepth = 0;

bool load( const char *filename )
{
	layers.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				char* _key = strtok( thisLine, ": " );
				int key = atoi( _key );
				char* _depth = strtok( nullptr, ": " );
				int depth = atoi( _depth );
				assert( depth > 1 );
				layer l( depth );
				assert( !strtok( nullptr, ": " ) );
				layers.insert( std::make_pair( key, l ) );
				if ( key >= totaldepth )
					totaldepth = key + 1;
			}
		}
	}
	delete[] buffer;
	fclose( fp );
	return true;
}

int greatestdepth = -1;

void reset()
{
	for ( auto& l : layers )
	{
		l.second.position = 0;
		l.second.direction = 1;
	}
}

bool process( int delay = 0 )
{
	reset();
	int severity = 0;
	for ( int p = 0; p < totaldepth + delay; ++p )
	{
		if ( p >= delay && layers.count( p-delay ) > 0 )
		{
			if ( layers[p-delay].position == 0 )
			{
				if ( delay )
				{
					if ( p - delay > greatestdepth )
					{
						printf( "%d: %d\n", delay, p - delay );
						greatestdepth = p - delay;
					}
					return false;
				}
				severity += p * layers[p].depth;
			}
		}
		for ( auto& l : layers )
		{
			l.second.position += l.second.direction;
			if ( l.second.position == 0 || l.second.position == l.second.depth - 1 )
				l.second.direction *= -1;
		}
	}
	if ( !delay )
		printf( "Part 1: severity %d\n", severity );
	return severity == 0;
}

void part2()
{
	bool result = false;
	int delay = 183124-1680;// 4;// 226756; //229416;
	greatestdepth = -1;
	while ( !result )
	{
		delay += 1680;
		result = process( delay );
	}
	printf( "Part 2: delay %d picoseconds\n", delay );
}

int main()
{
// 	if ( load( "example.txt" ) )
// 	{
// 		part2();
// 	}
	if ( load( "input.txt" ) )
	{
		part2();
	}
	return 0;
}

