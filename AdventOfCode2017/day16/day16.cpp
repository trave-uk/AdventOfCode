// day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::string input;

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
				input = thisLine;
			}
		}
	}
	return true;
}

template <typename T>
void applymap( std::vector<T>& dancers, const std::vector<int>& map )
{
	assert( dancers.size() == map.size() );
	std::vector<T> copy = dancers;
	int i = 0;
	for ( int pos : map )
	{
		dancers[pos] = copy[i];
		++i;
	}
}

void squaremap( std::vector<int>& map )
{
	std::vector<int> mapp = map;
	std::vector<int> copy = map;
	int i = 0;
	for ( int pos : mapp )
	{
		map[i] = copy[pos];
		++i;
	}
}

void applyMoves( std::string& output, std::string input, int dancer_count, bool bIncludePartners )
{
	std::deque<char> dancers;
	for ( int i = 0; i < dancer_count; ++i )
	{
		dancers.push_back( 'a' + i );
	}

	std::string copy = input;

	char* token = strtok( const_cast<char*>(copy.c_str()), "," );
	while ( token )
	{
		switch ( *token )
		{
			case 's': // spin
			{
				int count = atoi( token + 1 );
				assert( count > 0 && count <= dancer_count );
				for ( ; count > 0; --count )
				{
					char dancer = dancers.back();
					dancers.pop_back();
					dancers.push_front( dancer );
				}
				break;
			}
			case 'x': // exchange
			{
				int x1 = atoi( token + 1 );
				int x2 = atoi( token + strcspn( token, "/" ) + 1 );
				assert( x1 >= 0 && x1 < dancer_count );
				assert( x2 >= 0 && x2 < dancer_count );
				char d1 = dancers[x1];
				dancers[x1] = dancers[x2];
				dancers[x2] = d1;
				break;
			}
			case 'p': // partner
			{
				// part 2 runs an even number of times, so "partner" swaps are cancelled out
				if ( bIncludePartners )
				{
					char d1 = token[1];
					char d2 = token[3];
					assert( token[2] == '/' && d1 >= 'a' && d2 >= 'a' && d1 < 'a' + dancer_count && d2 < 'a' + dancer_count );
					for ( int i = 0; i < dancers.size(); ++i )
					{
						if ( dancers[i] == d1 )
						{
							dancers[i] = d2;
						}
						else if ( dancers[i] == d2 )
						{
							dancers[i] = d1;
						}
					}
				}
				break;
			}
		}
		token = strtok( nullptr, "," );
	}
	output.clear();
	for ( char d : dancers )
	{
		output.append( 1, d );
	}
}

void process( std::string input, int dancer_count, int repeats )
{
	std::string output;
	applyMoves( output, input, dancer_count, true );
	printf( "Part 1: %s\n", output.c_str() );

	// Part 2: Apply this 1000000000 (`repeats`) times.
	int bitset = repeats;

	// First, get a mapping of one iteration (ignoring partner moves)
	applyMoves( output, input, dancer_count, false );
	std::vector<int> map;
	map.insert( map.end(), output.length(), 0 );
	int i = 0;
	for ( char d : output )
	{
		int other = d - 'a';
		map[other] = i;
		++i;
	}

	// Now, for each bit set in "repeats", apply the map. Square the map to get the map to apply for the next bit.
	std::vector<char> dancers2;
	for ( int i = 0; i < dancer_count; ++i )
	{
		dancers2.push_back( 'a' + i );
	}
	do 
	{
		int bit = bitset & 1;
		bitset >>= 1;
		if ( bit )
		{
			applymap( dancers2, map );
		}
		squaremap( map );
	} while ( bitset );
	dancers2.push_back( '\0' );
	printf( "Part 2: %s\n", &dancers2.front() );
}

int main()
{
	process( "s1,x3/4,pe/b", 5, 2 );
	if ( load( "input.txt" ) )
	{
		process( input, 16, 1000000000 );
	}
    return 0;
}

