// day18.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum tile : char
{
	start = '@',
	wall = '#',
//	door = '|',
	key = '~',
	corridor = '.'
};

struct _key
{
	int x, y;	// position of key
	int dx, dy;	// position of door
};

tile map[100][100];
unsigned int search[100][100];

int width = 0, height = 0, nkeys = 0;
_key keys[26];

struct _start
{
	int x = 0;
	int y = 0;
};

std::vector<_start> starts;

struct foundkey
{
	int distance = 0;
	std::bitset<26> doors;
};

// pairs stores the distance from each key to each other, including which doors are between them
// pairs[26] to pairs[29] contain the distance from the start positions to each key
foundkey pairs[30][26];

void floodfill( int x, int y, unsigned int distance, int sourcekey, std::bitset<26> doors )
{
	if ( distance == 0 )
		memset( search, -1, sizeof(search) );
	tile here = map[y][x];
	if ( here == wall )
		return;
	if ( distance >= search[y][x] )
		return;
	if ( isupper( here ) )
		doors.set( here - 'A' );
	search[y][x] = distance;
	if ( distance > 0 && islower( here ) )
	{
		int destkey = here - 'a';
		pairs[sourcekey][destkey].distance = distance;
		pairs[sourcekey][destkey].doors = doors;
	}
	floodfill( x - 1, y, distance + 1, sourcekey, doors );
	floodfill( x + 1, y, distance + 1, sourcekey, doors );
	floodfill( x, y - 1, distance + 1, sourcekey, doors );
	floodfill( x, y + 1, distance + 1, sourcekey, doors );
}

bool load( const char* filename )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	starts.clear();
	height = 0;
	nkeys = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				strcpy( (char*)( map[height] ), thisLine );
				width = max( width, strlen( thisLine ) );
				for ( int i = 0; i < width; ++i )
				{
					if ( map[height][i] == start )
					{
						_start st;
						st.x = i;
						st.y = height;
						starts.push_back( st );
						map[height][i] = corridor;
					}
					if ( isalpha( thisLine[i] ) )
					{
						int k = toupper( thisLine[i] ) - 'A';
						if ( k >= nkeys )
							nkeys = k + 1;
						if ( isupper( thisLine[i] ) ) // door
						{
							keys[k].dx = i;
							keys[k].dy = height;
							//							map[height][i] = door;
						}
						else // key
						{
							keys[k].x = i;
							keys[k].y = height;
						}
					}
				}
				++height;
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

void parse()
{
	// now floodfill from each key to find all other keys, filling in the "pairs" collection	
	std::bitset<26> doors;	// empty bitset initially for each floodfill
	assert( starts.size() + 26 <= ARRAYSIZE( pairs ) );
	int startindex = 26;
	for ( _start st : starts )
	{
		floodfill( st.x, st.y, 0, startindex, doors );
		++startindex;
	}
	for ( int i = 0; i < nkeys; ++i )
	{
		_key& k = keys[i];
		floodfill( k.x, k.y, 0, i, doors );
	}
}

struct state
{
	int robot;
	std::bitset<26> unlocks;	// which keys have been found
	int keyindex;
	int distance = 0;		// how far has been traveled
	bool operator<( const state& other ) const { return distance < other.distance; }
};

std::priority_queue<state> pendingstates;

void process()
{
	int closest[4] = { INT_MAX, INT_MAX, INT_MAX, INT_MAX };
	int keycounts[4] = {};
	state s;
	int robot = 0;
	int startindex = 26;
	for ( _start st : starts )
	{
		assert( startindex < ARRAYSIZE( pairs ) );
		for ( int k = 0; k < nkeys; ++k )
		{
			if ( pairs[startindex][k].distance > 0 )
			{
				++keycounts[robot];
			}
		}
		s.robot = robot++;
		s.keyindex = startindex++;
		pendingstates.push( s );
	}

	while ( pendingstates.size() > 0 )
	{
		s = pendingstates.top();
		pendingstates.pop();
		if ( s.distance >= closest[s.robot] )
			continue;
		if ( s.unlocks.count() == keycounts[s.robot] )
		{
			if ( s.distance < closest[s.robot] )
			{
				closest[s.robot] = s.distance;
				printf( "All robot %d keys @ step %d\n", s.robot, closest[s.robot] );
			}
			continue;
		}
//		for ( int robot = 0; robot < s.robotindices.size(); ++robot )
		{
			for ( int k = 0; k < nkeys; ++k )
			{
				// ignore this key
				if ( k == s.keyindex )
					continue;
				// ignore keys we already have
				if ( s.unlocks.test( k ) )
					continue;
				// ignore keys we cannot reach
				foundkey &p = pairs[s.keyindex][k];
				if ( p.distance == 0 )
					continue;
// 				std::bitset<26> unlockeddoors = s.unlocks & p.doors;
// 				if ( unlockeddoors.count() != p.doors.count() )
// 					continue;
				// add this key to the queue
				state s2;
				s2.robot = s.robot;
				s2.distance = s.distance + p.distance;
				s2.unlocks = s.unlocks;
				s2.unlocks.set( k );
				s2.keyindex = k;
				pendingstates.push( s2 );
			}
		}
	}
	printf( "Shortest distance = %d + %d + %d + %d = %d\n", closest[0], closest[1], closest[2], closest[3], closest[0] + closest[1] + closest[2] + closest[3] );
}

int main()
{
	static const char* filename =
// 		"input.txt";
//		"example.txt";
//		"example2.txt";
// 		"example3.txt";
// 		"example4.txt";

 		"input2.txt";
//		"example21.txt";
// 		"example22.txt";
// 		"example23.txt";
// 		"example24.txt";

	if ( load( filename ) )
	{
		parse();
		process();
	}
	return 0;
}

