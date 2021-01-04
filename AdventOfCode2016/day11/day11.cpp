// day11.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 04/01/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum TYPE
{
	GENERATOR,
	MICROCHIP
};

class Device
{
public:
	Device() = default;
	Device( const char* description )
	{
		char e[80];
		char t[80];
		if ( sscanf( description, "a %[^- ]-compatible %s", e, t ) == 2 )
		{
			type = MICROCHIP;
		}
		else if ( sscanf( description, "a %s %s", e, t ) == 2 )
		{
			type = GENERATOR;
		}
		else
		{
			assert( false );
		}

		element = std::string( e );
		id = nextId++;
	}

	bool operator<( const Device& other ) const
	{
		return id < other.id;
	}

	bool operator==( const Device& other ) const
	{
		return id == other.id;
	}

	int id;
	TYPE type;
	std::string element;

	static int nextId;
};

std::map<int, Device> devices;

int Device::nextId = 0;

const int FLOORS = 4;
using Floor = std::set<int>;
using Floors = std::array<Floor, FLOORS>;

using HASH = unsigned int;
HASH hash( int elevator, const Floors& floors )
{
	std::string ids = std::to_string(elevator) + ":";
	for ( const Floor& f : floors )
	{
		for ( int id : f )
		{
			ids += std::to_string( id ) + "_";
		}
		ids += ";";
	}
	return crc32buf( ids.data(), ids.length() );
}

// map of hash to score.
std::map<HASH, int> seenStates;

bool safe( const Floor& f )
{
	std::set<std::string> generators;
	std::set<std::string> microchips;
	for ( int id : f )
	{
		Device& d = devices[id];
		if ( d.type == GENERATOR )
		{
			generators.insert( d.element );
		}
		else
		{
			microchips.insert( d.element );
		}
	}
	for ( const std::string& m : microchips )
	{
		if ( !generators.count( m ) &&  !generators.empty() )
		{
			return false;
		}
	}
	return true;
}

bool finished( const Floors &floors )
{
	for ( int f = 0; f < FLOORS - 1; ++f )
	{
		if ( !floors[f].empty() )
		{
			return false;
		}
	}
	return true; // !floors[FLOORS - 1].empty();
}

struct Search
{
	Search( int e, Floors f, int s )
		: elevator( e )
		, floors( f )
		, score( s )
	{
	}

	int elevator;
	Floors floors;
	int score;
};

int search( int _elevator, const Floors& _floors, int _score = 0 )
{
	std::queue<Search> q;
	q.push( Search( _elevator, _floors, _score ) );

	while ( !q.empty() )
	{
		Search s = q.front();
		q.pop();

		HASH h = hash( s.elevator, s.floors );
		if ( seenStates.count( h ) )
		{
			if ( seenStates[h] <= s.score )
				continue;
		}

		seenStates[h] = s.score;

		if ( finished( s.floors ) )
		{
			return s.score;
		}

		const Floor& ef = s.floors[s.elevator];

		// generate a list of combinations of 1 or 2 devices from the elevator's floor, where removing the combination doesn't invalidate the floor.
		std::set<std::set<int>> moves;
		for ( int d : ef )
		{
			Floor f = ef;
			f.erase( d );

			if ( safe( f ) )
			{
				moves.insert( { d } );
			}
			for ( int e : ef )
			{
				if ( d == e )
				{
					continue;
				}
				Floor g = f;
				g.erase( e );
				if ( safe( g ) )
				{
					std::set<int> m = { d, e };
					if ( !moves.count( m ) )
					{
						moves.insert( m );
					}
				}
			}
		}

		// for each move gathered, try moving them up and down
		for ( int el = s.elevator - 1; el <= s.elevator + 1; el += 2 )
		{
			if ( el < 0 || el >= FLOORS )
			{
				continue;
			}
			for ( const std::set<int>& m : moves )
			{
				Floors ff = s.floors;
				for ( int d : m )
				{
					ff[s.elevator].erase( d );
					ff[el].insert( d );
				}
				if ( safe( ff[el] ) && safe( ff[s.elevator] ) )
				{
					q.push( Search( el, ff, s.score + 1 ) );
				}
			}
		}
	}
	return -1;
}

int main()
{

	Floors floors;
	int elevator = 0;

	for ( Floor f : floors )
	{
		f.clear();
	}
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int floor = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				// The first floor contains a hydrogen-compatible microchip and a lithium-compatible microchip.
				// The second floor contains a hydrogen generator.
				// The third floor contains a lithium generator.
				// The fourth floor contains nothing relevant.
				Floor& f = floors[floor];
				char* desc = thisLine;
				while ( desc )
				{
					desc = strstr( desc, " a " );
					if ( desc )
					{
						++desc;
						Device device( desc );
						f.insert( device.id );
						devices[device.id] = device;
					}
				}
				assert( safe( f ) );
				++floor;
			}
		}
	}
	assert( floor == floors.size() );
	fclose( fp );
	delete[] buffer;

	elevator = 0;
	int part1 = search( elevator, floors );
	printf( "Part 1: %d\n", part1 );

	// Add the following to floor 1:
	// An elerium generator.
	// An elerium-compatible microchip.
	// A dilithium generator.
	// A dilithium-compatible microchip.
	for ( const char* desc : { "a elerium generator", "a elerium-compatible microchip", "a dilithium generator", "a dilithium-compatible microchip" } )
	{
		Device d( desc );
		floors[0].insert( d.id );
		devices[d.id] = d;
	}
	int part2 = search( elevator, floors );
	printf( "Part 2: %d\n", part2 );
	return 0;
}
