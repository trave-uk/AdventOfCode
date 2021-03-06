// day9.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// 465 players; last marble is worth 71498 points

const int nPlayers = 465;
const int lastMarble = 71498;

struct player
{
	size_t score = 0;
};

template <size_t nPlayers, int lastMarble>
class game
{
public:
	game()
	{
		int player = 0;
		std::list<int>::iterator current;
		for ( int marble = 0; marble <= lastMarble; ++marble )
		{
			if ( marble < 2 )
			{
				marbles.push_back( marble );
				current = marbles.end();
				--current;
			}
			else if ( ( marble % 23) == 0 )
			{
				// score
				players[player].score += marble;
				// find marble 7 to the left
//				int deleteThis = (current - 7 + marbles.size()) % marbles.size();
				std::list<int>::iterator deleteIter = current;	
				moveBy( deleteIter, -7 );
				players[player].score += *deleteIter;
				std::list<int>::iterator newIter = marbles.erase( deleteIter );
				if ( newIter == marbles.end() )
					current = marbles.begin();
				else
					current = newIter;
				if ( players[player].score > maxScore )
				{
					maxScore = players[player].score;
				}
			}
			else 
			{
				// insert between 1 and 2 marbles to the right
//				int after = (current + 2) % marbles.size();
				std::list<int>::iterator afterIter = current;
				moveBy( afterIter, 2);
				std::list<int>::iterator newIter = marbles.insert( afterIter, marble );
				current = newIter;
			}
			player = (player + 1) % nPlayers;
		}
		printf( "%zd players, last marble = %d, maximum score = %zd\n", nPlayers, lastMarble, maxScore );
	}

	void moveBy( std::list<int>::iterator& from, int offset )
	{
		if ( offset > 0 )
		{
			for ( int i = 0; i < offset; ++i )
			{
				++from;
				if ( from == marbles.end() )
				{
					from = marbles.begin();
				}
			}
		}
		else
		{
			for ( int i = 0; i > offset; --i )
			{
				if ( from == marbles.begin() )
				{
					from = marbles.end();
				}
				--from;
			}
		}
	}
private:
	size_t maxScore = 0;
	std::array<player, nPlayers> players;
	std::list<int> marbles;

};

#define playGame( nPlayers, lastMarble ) \
{ \
	game<nPlayers, lastMarble> instance; \
	game.play(); \
	printf( "test1: %d players, last marble = %d, maximum score = %d\n", nPlayers, lastMarble, game.maxScore ); \
	return game.maxScore; \
}

int main()
{
	game<9, 25> instance1;
	game<10, 1618 > instance2;
	game<13, 7999 > instance3;
	game<17, 1104 > instance4;
	game<21, 6111 > instance5;
	game<30, 5807 > instance6;
	game<465, 71498 > instance7;
	game<465, 7149800 > instance8;
	return 0;
}

