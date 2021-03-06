// day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


std::vector<std::string> map;

bool load( const char *filename )
{
	map.clear();
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
				map.push_back( std::string( thisLine ) );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

int evaluate( std::vector<std::string> m )
{
	int total = 0;
	int bit = 0;
	for ( std::string line : m )
	{
		for ( char tile : line )
		{
			if ( tile == '#' )
				total += 1 << bit;
			bit++;
		}
	}
	return total;
}

std::set<int> seen;

bool bug( int x, int y )
{
	if ( x < 0 || x > 4 || y < 0 || y > 4 )
		return false;
	return map[y][x] == '#';
}

int adjacent( int x, int y )
{
	return bug( x - 1, y ) + bug( x + 1, y ) + bug( x, y - 1 ) + bug( x, y + 1 );
}

std::vector<std::string> next;

void process()
{
	seen.clear();
	// life-ish rules
	next = map;
	do
	{
		int biodiversity = evaluate( map );
		if ( seen.count( biodiversity ) == 1 )
		{
			printf( "Part 1: %d\n", biodiversity );
			break;
		}
		seen.insert( biodiversity );
		int y = 0;
		for ( std::string& line : map )
		{
			int x = 0;
			for ( char& tile : line )
			{
				int count = adjacent( x, y );
				if ( tile == '#' )
				{
					// A bug dies unless there is exactly one bug adjacent to it.
					if ( count != 1 )
						next[y][x] = '.';
				}
				else
				{
					// An empty space becomes infested with a bug if exactly one or two bugs are adjacent to it.
					if ( count >= 1 && count <= 2 )
						next[y][x] = '#';
				}
				++x;
			}
			++y;
		}
		map = next;
	} while ( true );
}

std::deque<std::vector<std::string>> maps;
std::deque<std::vector<std::string>> nexts;

int bug2( int x, int y, int depth, int dx=0, int dy=0 )
{
	int depths = maps.size();
	if ( depth < 0 || depth >= depths )
		return 0;
	if ( x < 0 || x > 4 || y < 0 || y > 4 )
	{
		if ( x < 0 )
			return bug2( 1, 2, depth - 1 );
		if ( x > 4 )
			return bug2( 3, 2, depth - 1 );
		if ( y < 0 )
			return bug2( 2, 1, depth - 1 );
		if ( y > 4 )
			return bug2( 2, 3, depth - 1 );
	}
	if ( x == 2 && y == 2 )
	{
		int d = depth + 1;
		if ( dx < 0 )
		{
			return bug2( 4, 0, d ) + bug2( 4, 1, d ) + bug2( 4, 2, d ) + bug2( 4, 3, d ) + bug2( 4, 4, d );
		}
		if ( dx > 0 )
		{
			return bug2( 0, 0, d ) + bug2( 0, 1, d ) + bug2( 0, 2, d ) + bug2( 0, 3, d ) + bug2( 0, 4, d );
		}
		if ( dy < 0 )
		{
			return bug2( 0, 4, d ) + bug2( 1, 4, d ) + bug2( 2, 4, d ) + bug2( 3, 4, d ) + bug2( 4, 4, d );
		}
		if ( dy > 0 )
		{
			return bug2( 0, 0, d ) + bug2( 1, 0, d ) + bug2( 2, 0, d ) + bug2( 3, 0, d ) + bug2( 4, 0, d );
		}
		assert( false );
	}
	return maps[depth][y][x] == '#';
}

int adjacent2( int x, int y, int depth )
{
	return bug2( x - 1, y, depth, -1, 0 ) + bug2( x + 1, y, depth, 1, 0 ) + bug2( x, y - 1, depth, 0, -1 ) + bug2( x, y + 1, depth, 0, 1 );
}

void report( int minutes )
{
	printf( "After %d minutes:\n\n", minutes );
	int depth = -minutes;
	for ( std::vector<std::string> m : maps )
	{
		if ( evaluate( m ) )
		{
			printf( "Depth %d:\n", depth );
			for ( std::string line : m )
			{
				printf( "%s\n", line.c_str() );
			}
			printf( "\n" );
		}
		++depth;
	}
}

void process2( int minutes = 100)
{
	// life-ish rules
	std::vector<std::string> empty;
	for ( int i = 0; i < 5; ++i )
		empty.push_back( "....." );
	empty[2][2] = '?';

	maps.push_back( map );
	maps[0][2][2] = '?';
	for ( int minute = 0; minute < minutes; ++minute )
	{
//		report( minute );
//		if ( ( minute % 2 ) == 0 )
		{
			maps.push_back( empty );
			maps.push_front( empty );
		}
		nexts = maps;

		int depth = 0;
		for ( std::vector<std::string> m : maps )
		{
			int y = 0;
			for ( std::string& line : m )
			{
				int x = 0;
				for ( char& tile : line )
				{
					if ( x != 2 || y != 2 )
					{
						int count = adjacent2( x, y, depth );
						if ( tile == '#' )
						{
							assert( nexts[depth][y][x] == '#' );
							// A bug dies unless there is exactly one bug adjacent to it.
							if ( count != 1 )
								nexts[depth][y][x] = '.';
						}
						else
						{
							assert( nexts[depth][y][x] == '.' );
							// An empty space becomes infested with a bug if exactly one or two bugs are adjacent to it.
							if ( count >= 1 && count <= 2 )
								nexts[depth][y][x] = '#';
						}
					}
					++x;
				}
				++y;
			}
			++depth;
		}
		maps = nexts;
	}
	report( minutes );
	int result = 0;
	// count
	for ( std::vector<std::string> m : maps )
	{
		for ( std::string& line : m )
		{
			for ( char& tile : line )
			{
				if ( tile == '#' )
					++result;
			}
		}
	}
	printf( "Part 2: %d\n", result );
}

int main()
{
//	const char* filename = "example.txt";
	const char* filename = "input.txt";
// 	if ( load( filename ) )
// 	{
// 		process();
// 	}

	if ( load( filename ) )
	{
		process2( 200 );
		// 2030 too high
		// 1995 correct
	}
	return 0;
}
