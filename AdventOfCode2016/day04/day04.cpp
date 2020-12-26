// day04.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 26/12/2020 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Room
{
	std::string name;
	std::string encryptedName;
	int sectorID;
	std::string checksum;
};

std::vector<Room> rooms;

int main()
{
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				Room room;
				char* encryptedName = thisLine;
				char* sectorID = thisLine + strcspn( thisLine, "0123456789" );
				assert( sectorID[-1] == '-' );
				sectorID[-1] = '\0';
				char* checksum = sectorID + strcspn( sectorID, "[]" );
				assert( *checksum == '[' );
				*checksum = '\0';
				++checksum;
				assert( checksum[5] == ']' );
				checksum[5] = '\0';

				room.encryptedName = std::string( encryptedName );
				room.checksum = std::string( checksum );
				room.sectorID = atoi( sectorID );

				std::string letters = room.encryptedName;
				std::sort( letters.begin(), letters.end() );
				char c = '\0';
				int count = 0;
				std::set<std::pair<int, char>> counts;
				for ( char l : letters )
				{
					if ( l != c )
					{
						if ( c != '-' && count > 0 )
						{
							counts.insert( std::make_pair( 100 - count, c ) );
						}
						count = 0;
						c = l;
					}
					++count;
				}
				if ( c != '-' && count > 0 )
				{
					counts.insert( std::make_pair( 100 - count, c ) );
				}

				std::string chksum;
				for ( auto x : counts )
				{
					chksum.push_back( x.second );
					if ( chksum.length() == 5 )
					{
						break;
					}
				}
				if ( room.checksum == chksum )
				{
					rooms.push_back( room );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	int part1 = 0;
	for ( Room room : rooms )
	{
		part1 += room.sectorID;
	}
	printf( "Part 1: %d\n", part1 );

	std::map<std::string, int> firstWords;
	for ( Room& room : rooms )
	{
		std::string firstWord;
		room.name = room.encryptedName;
		int len = 0;
		for ( char& c : room.name )
		{
			if ( c == '-' )
			{
				if ( firstWord.empty() )
				{
					firstWord = room.name.substr(0, len);
				}
				c = ' ';
			}
			else
			{
				int x = c + ( room.sectorID % 26 );
				if ( x > 'z' )
				{
					x -= 26;
				}
				c = x;
			}
			++len;
		}
		if ( firstWord == std::string( "northpole" ) )
		{
			printf( "%s : %d\n", room.name.data(), room.sectorID );
		}
		++firstWords[firstWord];
	}

	return 0;
}
