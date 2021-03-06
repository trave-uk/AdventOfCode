// day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


const __int64 factorA = 16807;
const __int64 factorB = 48271;

const __int64 startA = 116;
const __int64 startB = 299;

const __int64 divisor = 2147483647;

class generator
{
public:
	generator( __int64 start )
	{
		value = start;
	}
	union
	{
		__int64 value;
		unsigned short low16;
	};
	inline generator& operator*= ( __int64 other )
	{
		value *= other;
		value %= divisor;
		return *this;
	}
	void tick( __int64 factor, unsigned short _and )
	{
		do 
		{
			*this *= factor;
		} while (low16 & _and);
	}
};

generator A( startA );
generator B( startB );

__int64 count = 0;

inline void tick()
{
	A *= factorA;
	B *= factorB;

	const unsigned short match = A.low16 ^ B.low16;
	if ( !match )
	{
		++count;
	}
}

inline void tick2()
{
	A.tick( factorA, 3 );
	B.tick( factorB, 7 );

	const unsigned short match = A.low16 ^ B.low16;
	if ( !match )
	{
		++count;
	}
}

int main()
{
	for ( int i = 0; i < 40000000; ++i )
	{
		tick();
	}
	printf( "Part 1: %lld\n", count );

	A = generator( startA );
	B = generator( startB );
	count = 0;
	for ( int i = 0; i < 5000000; ++i )
	{
		tick2();
	}
	printf( "Part 2: %lld\n", count );
	return 0;
}

