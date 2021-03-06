// day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


// 0 <-> 2
// 1 <-> 1
// 2 <-> 0, 3, 4

std::map<int, std::vector<int>> neighbourhood;

bool load( const char *filename )
{
	neighbourhood.clear();
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
				char* _key = strtok( thisLine, " <->" );
				int key = atoi( _key );
				char* neighbour = strtok( nullptr, ", " );
				std::vector<int> neighbours;
				while ( neighbour )
				{
					neighbours.push_back( atoi( neighbour ) );
					neighbour = strtok( nullptr, ", " );
				}
				neighbourhood.insert( std::make_pair( key, neighbours ) );
			}
		}
	}
	delete[] buffer;
	fclose( fp );
	return true;
}

std::set<int> unreached;
std::set<int> reachable;

void process()
{
	// Part 1: find all programs reachable from 0.
	reachable.clear();
	unreached.clear();
	for ( auto& p : neighbourhood )
	{
		unreached.insert( p.first );
	}
	std::queue<int> q;
	int groups = 0;
	bool done0 = false;
	while ( !unreached.empty() )
	{
		int first = *unreached.begin();
		assert( first == 0 || done0 );
		q.push( first );
		++groups;
		while ( !q.empty() )
		{
			int n = q.front();
			q.pop();
			if ( reachable.count( n ) > 0 )
				continue;
			reachable.insert( n );
			unreached.erase( n );
			std::vector<int>& neighbours = neighbourhood[n];
			for ( int nn : neighbours )
			{
				q.push( nn );
			}
		}
		if ( !done0 && reachable.count( 0 ) > 0 )
		{
			done0 = true;
			printf( "Part 1: %lld programs reachable from 0\n", reachable.size() );
		}
	}
	printf( "Part 2: %d groups of programs found\n", groups );
}

int main()
{
	if ( load( "example.txt" ) )
	{
		process();
	}
	if ( load( "input.txt" ) )
	{
		process();
	}
	return 0;
}

