// day17.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <forward_list>

std::forward_list<int> buffer;

void process( int input, int last = 2017 )
{
	int zeropos = 0;
	buffer.clear();
	buffer.push_front( 0 );
	int position = 0;
	std::forward_list<int>::iterator iter = buffer.begin();
	for ( int value = 1; value <= last; ++value )
	{
		position += input;
		for ( int step = input; step > 0; --step )
		{
			++iter;
			if ( iter == buffer.end() )
			{
				iter = buffer.begin();
			}
		}
		position %= value; // buffer.size()
		if ( *iter == 0 )
		{
			zeropos = position;
			printf( "Inserting %d after 0\n", value );;
		}
		++position;
		iter = buffer.insert_after( iter, value );
	}
	// Now report the contents of the buffer near position
	for ( int i = 0; i <= 3; ++i )
	{
		if ( i == 0 )
		{
			printf( "(%d) ", *iter );
		}
		else
		{
			printf( "%d ", *iter );
		}
		++iter;
	}
	printf( "\n" );
}

int main()
{
	process( 3 );
	process( 312 );

	process( 312, 50000000 );
	return 0;
}

