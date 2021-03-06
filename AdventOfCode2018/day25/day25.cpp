// day25.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct pos
{
	int x;
	int y;
	int z;
	int t;
	bool operator<( const pos& other ) const
	{
		if ( x < other.x ) return true;
		if ( x == other.x && y < other.y ) return true;
		if ( x == other.x && y == other.y && z < other.z ) return true;
		return (x == other.x && y == other.y && z == other.z && t < other.t);
	}
	std::set<int> neighbours;
	int constellation = -1;
};

std::vector<pos> stars;

using constellation_t = std::vector<int>;
std::vector<constellation_t> constellations;

inline int distance( const pos& p1, const pos& p2 )
{
	return abs( p1.x - p2.x ) + abs( p1.y - p2.y ) + abs( p1.z - p2.z ) + abs( p1.t - p2.t );
}

void process()
{
	// find immediate neighbours
	for ( pos& s : stars )
	{
		int index = &s - &stars.front();
		// find all neighbours (less than 3 away)
		for ( int i = 0; i< stars.size(); ++i )
		{
			if ( i == index )
				continue;
			if ( s.neighbours.count( i ) > 0 )
				continue;
			pos& t = stars[i];
			if ( distance( s, t ) <= 3 )
			{
				s.neighbours.insert( i );
				t.neighbours.insert( index );
			}
		}
	}

	// Now include neighbours of neighbours
	int c_index = 0;
	for ( pos& s : stars )
	{
		if ( s.constellation != -1 )
			continue;
		constellation_t c;
		std::queue<int> q;
		for ( int i : s.neighbours )
		{
			q.push( i );
		}
		s.constellation = c_index;
		while ( !q.empty() )
		{
			int i = q.front();
			q.pop();
			c.push_back( i );
			pos& t = stars[i];
			t.constellation = c_index;
			for ( int j : t.neighbours )
			{
				auto found = std::find( c.begin(), c.end(), j );
				if ( found == c.end() )
				{
					q.push( j );
				}
			}
		}
		constellations.push_back( c );
		++c_index;
	}

	printf( "%d constellations found\n", c_index );
}

bool load( const char *filename )
{
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
				// Process each line...
				pos p;
				sscanf( thisLine, "%d,%d,%d,%d", &p.x, &p.y, &p.z, &p.t );
				stars.push_back( p );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}


int main()
{
	//if ( load( "example4.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		printf( "Press return to continue...\n" );
		getchar();
	}
	return 0;
}
