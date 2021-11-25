// day19.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 22/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"


struct Elf
{
	Elf(int _number)
		: number( _number )
		, next( nullptr )
	{}

	int number;
	Elf* next;
};
Elf& operator++( Elf& a )
{
	return *a.next;
}

void Process( std::string title, int numElves, bool bPart2 = false )
{
	std::vector<Elf> elves;
	for ( int i = 1; i <= numElves; ++i )
	{
		elves.push_back( Elf( i ) );
	}
	for ( Elf& elf : elves )
	{
		elf.next = ( &elf ) + 1;
	}
	Elf& elf = elves.back();
	elf.next = &elves[0];

	int count = numElves;
	Elf* pElf = &elves[0];
	while ( pElf->next != pElf )
	{
		if ( bPart2 )
		{
			// Remove the opposite node. NB: count >= 2
			Elf* pPrev = pElf;
			int c = count / 2 - 1; 
			while ( c > 0 )
			{
				pPrev = pPrev->next;
				--c;
			}
			Elf* pNext = pPrev->next;
			pPrev->next = pNext->next;
		}
		else
		{
			// Remove the next node
			Elf* pNext = pElf->next;
			pElf->next = pNext->next;
		}
		pElf = pElf->next;
		--count;
	}
	printf( "%s Part %d: %d -> %d\n", title.c_str(), bPart2 ? 2 : 1, numElves, pElf->number );
}

int main()
{
	Process( "Example", 5 );
	Process( "My", 3012210 );

	Process( "Example", 5, true );
	Process( "My", 3012210, true );

	return 0;
}
