// day7.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	std::set<char> steps;
	std::vector<char> order;
	std::multimap<char, char> rules;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			
			char before, after;
			sscanf( buffer, "Step %c must be finished before step %c can begin.", &before, &after );
			rules.insert( std::pair<char, char>( after, before ) );
			if ( steps.count( after ) == 0 )
				steps.insert( after );
			if ( steps.count( before ) == 0 )
				steps.insert( before );
		}
	}
	fclose( fp );
	delete[] buffer;

	while ( steps.size() > order.size() )
	{
		for ( char test : steps )
		{
			if ( std::find( order.begin(), order.end(), test ) != order.end() )
				continue;
			bool bOK = true;
			for ( auto iter = rules.lower_bound( test ); iter != rules.upper_bound( test ); ++iter )
			{
				if ( std::find( order.begin(), order.end(), iter->second ) == order.end() )
				{
					bOK = false;
					break;
				}
			}
			if ( bOK )
			{
				order.push_back( test );
				break;
			}
		}
	}
	order.push_back( '\0' );
	printf( "part 1: %s\n", &order.front() );

	int seconds = 0;
	std::vector<char> complete;
	std::set<char> started;
	// 5 workers concurrently, seconds = step - 'A' + 61
	struct worker
	{
		char step = '.';
		int seconds = 0;
	};
	std::array<worker, 5> workers;
	bool bSomething = false;
	while ( complete.size() < steps.size() )
	{
		// Employ any unemployed elfs
		for ( worker& elf : workers )
		{
			bool bOK = false;
			if ( elf.step == '.' )
			{
				for ( char test : steps )
				{
					if ( started.count( test ) > 0 )
						continue;
					bOK = true;
					for ( auto iter = rules.lower_bound( test ); iter != rules.upper_bound( test ); ++iter )
					{
						if ( std::find( complete.begin(), complete.end(), iter->second ) == complete.end() )
						{
							bOK = false;
							break;
						}
					}
					if ( bOK )
					{
						elf.step = test;
						elf.seconds = test - 'A' + 61;
						started.insert( test );
						bSomething = true;
						break;
					}
				}
			}
		}
		if ( bSomething )
		{
			complete.push_back( '\0' );
			printf( "%8d ", seconds );
			for ( worker elf : workers )
			{
				printf( "%c ", elf.step );
			}
			printf( "%s\n", &complete.front() );
			complete.pop_back();
		}
		bSomething = false;
		// Tick one second
		++seconds;
		for ( worker& elf : workers )
		{
			if ( elf.seconds > 0 )
			{
				--elf.seconds;
				if ( !elf.seconds )
				{
					complete.push_back( elf.step );
					elf.step = '.';
					bSomething = true;
				}
			}
		}
	}

	complete.push_back( '\0' );
	printf( "%8d ", seconds );
	for ( worker elf : workers )
	{
		printf( "%c ", elf.step );
	}
	printf( "%s\n", &complete.front() );
	return 0;
}

// try 1: BFKOVYAEGNTIXZHRMCJLSDUPWQ
// try 2: BFKEGNOVATIHXYZRMCJDLSUPWQ

// try 1: 1197 (too high)
// try 2: 1020
//        0 B F K V .
//       62 . F K V . B
//       66 . . K V . BF
//       71 E G O V . BFK
//       82 E G O A T BFKV
//      136 Y G O A T BFKVE
//      138 Y N O A T BFKVEG
//      143 Y N O . T BFKVEGA
//      146 Y N . . T BFKVEGAO
//      162 Y N I . . BFKVEGAOT
//      212 Y . I . . BFKVEGAOTN
//      221 . . I . . BFKVEGAOTNY
//      231 H X . . . BFKVEGAOTNYI
//      299 . X . . . BFKVEGAOTNYIH
//      315 Z . . . . BFKVEGAOTNYIHX
//      401 R . . . . BFKVEGAOTNYIHXZ
//      479 M . . . . BFKVEGAOTNYIHXZR
//      552 C J L . . BFKVEGAOTNYIHXZRM
//      615 . J L . . BFKVEGAOTNYIHXZRMC
//      622 D . L . . BFKVEGAOTNYIHXZRMCJ
//      624 D S . . . BFKVEGAOTNYIHXZRMCJL
//      686 . S . . . BFKVEGAOTNYIHXZRMCJLD
//      703 U . . . . BFKVEGAOTNYIHXZRMCJLDS
//      784 P . . . . BFKVEGAOTNYIHXZRMCJLDSU
//      860 W . . . . BFKVEGAOTNYIHXZRMCJLDSUP
//      943 Q . . . . BFKVEGAOTNYIHXZRMCJLDSUPW
//     1020 . . . . . BFKVEGAOTNYIHXZRMCJLDSUPWQ
