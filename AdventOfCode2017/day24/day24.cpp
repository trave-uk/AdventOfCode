// day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using ComponentMap = std::map<int, std::vector<int>>;
using Component = std::pair<int, int>;
using UsedComponents = std::set<Component>;
using Bridge = std::vector<Component>;

ComponentMap components;

int longest = 0;
int highest = 0;

int makeBridges( Bridge bridge = Bridge(), UsedComponents used = UsedComponents(), int score = 0, int length = 0 )
{
	if ( length > longest )
	{
		longest = length;
		highest = 0;
	}
	int high = score;
	int port = bridge.empty() ? 0 : bridge.back().second;
	for ( int p2 : components[port] )
	{
		Component c = std::make_pair( port, p2 );
		if ( used.count( c ) )
			continue;
		// Not used this component yet
		Bridge newBridge = bridge;
		newBridge.push_back( c );
		UsedComponents newUsed = used;
		newUsed.insert( c );
		newUsed.insert( std::make_pair( p2, port ) );

		int newScore = makeBridges( newBridge, newUsed, score + port + p2, length + 1 );
		if ( newScore > high )
			high = newScore;
	}
	if ( length == longest && high > highest )
	{
		highest = high;
	}
	return high;
}

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			assert( isdigit( *thisLine ) );
			int a, b;
			int count = sscanf( thisLine, "%d/%d", &a, &b );
			assert( count == 2 );
			components[a].push_back( b );
			components[b].push_back( a );
		}
	}
	fclose( fp );
	delete[] buffer;

	printf( "Part 1: %d\n", makeBridges() );
	printf( "Part 2: %d\n", highest );
	return 0;
}
