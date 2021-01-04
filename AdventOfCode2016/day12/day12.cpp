// day12.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 04/01/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int algo(int c)
{
	int a=0, b=0, d=0;
// cpy 1 a
	a = 1;
// cpy 1 b
	b = 1;
// cpy 26 d
	d = 26;
// jnz c 2 (_0)
	if ( c )
	{
// jnz 1 5 (_1)
// _0: cpy 7 c
		d += 7;
// _2: inc d
// dec c
// jnz c -2 (_2)
	}

	do 
	{
// _1: cpy a c
		c = a;
// _3: inc a
// dec b
// jnz b -2 (_3)
		a += b;
// cpy c b
		b = c;
// dec d
		--d;
// jnz d -6 (_1)
	} while ( d );
// cpy 18 c
// _5: cpy 11 d
// _4: inc a
// dec d
// jnz d -2 (_4)
// dec c
// jnz c -5 (_5)
	a += 11 * 18;
	return a;
}

int main()
{
	printf( "Part 1: %d\n", algo( 0 ) );
	printf( "Part 2: %d\n", algo( 1 ) );
	return 0;
}
