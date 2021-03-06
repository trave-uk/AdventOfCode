// day6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct pos
{
	int x = 0;
	int y = 0;
	bool operator<( const pos& pos1) const
	{
		return pos1.x < x || (pos1.x == x && pos1.y < y);
	}
};

int getManhattanDistance( pos& pos1, pos& pos2 )
{
	return abs( pos1.x - pos2.x ) + abs( pos1.y - pos2.y );
}

int main()
{
	std::set<pos> positions;
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	pos bounds;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			pos thisPos;
			sscanf( thisLine, "%d, %d", &thisPos.x, &thisPos.y );
			if ( thisPos.x > bounds.x )
				bounds.x = thisPos.x;
			if ( thisPos.y > bounds.y )
				bounds.y = thisPos.y;
			positions.insert( thisPos );
		}
	}
	fclose( fp );
	delete[] buffer;
	
// 	int* map = new int[bounds.x * bounds.y];
// 	memset( map, 0, bounds.x * bounds.y * sizeof( int ) );
// 	for ( pos coord : positions )
// 	{
// 
// 	}
	pos test;
	int *counts = new int[positions.size()];
	memset( counts, 0, sizeof( int ) * positions.size() );
	std::set<int> infinites;

	int part2RegionSize = 0;
	for ( test.x = 0; test.x <= bounds.x; ++test.x )
	{
		for ( test.y = 0; test.y <= bounds.y; ++test.y )
		{
			int minDistance = INT_MAX;
			int minIndex = -1;
			int index = 0;
			int totalDistance = 0;
			for ( pos coord : positions )
			{
				int distance = getManhattanDistance( coord, test );
				if ( distance < minDistance )
				{
					minDistance = distance;
					minIndex = index;
				}
				++index;
				totalDistance += distance;
			}
			assert( minIndex != -1 );
			++counts[minIndex];
			if ( test.x == 0 || test.y == 0 || test.x == bounds.x || test.y == bounds.y )
			{
				if ( infinites.count( minIndex ) == 0 )
				{
					infinites.insert( minIndex );
				}
			}
			if ( totalDistance < 10000 )
			{
				++part2RegionSize;
			}
		}
	}
	int maxArea = 0;
	for ( int index = 0; index < positions.size(); ++index )
	{
		if ( infinites.count( index ) > 0 )
		{
			continue;
		}
		if ( counts[index] > maxArea )
		{
			maxArea = counts[index];
		}
	}
	printf( "Largest area = %d\n", maxArea );
	printf( "Region within 10000 of everywhere is size %d\n", part2RegionSize );
	return 0;
}

