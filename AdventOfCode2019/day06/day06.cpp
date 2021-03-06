// day06.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

union ID
{
	ID() : id( 0 ) {}
	ID( const char*o ) : id( 0 ) { strcpy( object, o ); }
	int id;
	char object[4];
};

struct orbit
{
	orbit( ID i ) : id( i ), parent( nullptr ), child( nullptr ), next( nullptr ) {}
	ID id;
	orbit* parent;
	orbit* child;
	orbit* next;
};

using orbitmap = std::map<int, orbit>;
orbitmap orbits;

void addOrbit( char* middle, char* around )
{
	ID middleId(middle);
	orbitmap::iterator midIter = orbits.find( middleId.id );
	if ( midIter == orbits.end() )
	{
		// not found yet
		orbit mid( middleId );
		std::pair<orbitmap::iterator, bool> returned = orbits.insert( { middleId.id, mid } );
		midIter = returned.first;
	}

	ID childId( around );
	orbitmap::iterator childIter = orbits.find( childId.id );
	if ( childIter == orbits.end() )
	{
		// not found yet
		orbit chld( childId );
		std::pair<orbitmap::iterator, bool> returned = orbits.insert( { childId.id, chld } );
		childIter = returned.first;
	}

	childIter->second.parent = &midIter->second;

	if ( midIter->second.child )
	{
		orbit* prevChild = midIter->second.child;
		while ( prevChild->next )
		{
			prevChild = prevChild->next;
		}
		prevChild->next = &childIter->second;
	}
	else
	{
		midIter->second.child = &childIter->second;
	}
}

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// e.g. "A)B"
			char* tok1 = strtok( thisLine, ")" );
			char* tok2 = strtok( NULL, ")" );
			addOrbit( tok1, tok2 );
		}
	}
	fclose( fp );
	delete[] buffer;
	
	int count = 0;
	for ( orbitmap::iterator iter = orbits.begin(); iter != orbits.end(); iter++ )
	{
		// Count how many parents
		orbit* from = &iter->second;
		while ( from->parent )
		{
			++count;
			from = from->parent;
		}
	}

	printf( "Total orbits = %d\n", count );

	// Find YOU
	ID youId( "YOU" );
	orbitmap::iterator youIter = orbits.find( youId.id );
	orbit* you = &youIter->second;

	// Find SAN
	ID sanId( "SAN" );
	orbitmap::iterator sanIter = orbits.find( sanId.id );
	orbit *san = &sanIter->second;

	// Need to find the common parent of YOU and SAN.
	// Then find the distance from each to that parent, less 1
	// The distance to travel is the two distances added together

	std::set<int> foundIds;
	orbit *youParent = you;
	orbit *sanParent = san;
	orbit *common = nullptr;
	do 
	{
		if ( youParent )
		{
			youParent = youParent->parent;
			if ( youParent )
			{
				if ( foundIds.count( youParent->id.id ) )
				{
					common = youParent;
					break;
				}
				foundIds.insert( youParent->id.id );
			}
		}
		if ( sanParent )
		{
			sanParent = sanParent->parent;
			if ( sanParent )
			{
				if ( foundIds.count( sanParent->id.id ) )
				{
					common = sanParent;
					break;
				}
				foundIds.insert( sanParent->id.id );
			}
		}
	} while ( true );

	int transfers = 0;
	youParent = you->parent;
	sanParent = san->parent;
	while ( youParent != common )
	{
		++transfers;
		youParent = youParent->parent;
	}
	while ( sanParent != common )
	{
		++transfers;
		sanParent = sanParent->parent;
	}
	printf( "%d orbit transfers for YOU to get to SAN\n", transfers );
	return 0;
}

