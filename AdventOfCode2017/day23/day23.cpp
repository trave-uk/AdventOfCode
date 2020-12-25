// day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = __int64;

std::vector<std::string> assembly;

bool load( const char* filename )
{
	assembly.clear();
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
				std::string line( thisLine );
				assembly.push_back( line );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}


#define CASE(x) (x[0] + x[1] * 256 + x[2] * 65536)

class Processor
{
public:
	Processor( int id, std::queue<Value>& input_queue,
		std::queue<Value>& output_queue )
		: inputs( input_queue )
		, outputs( output_queue )
	{
		registers['p'] = id;
	}

	std::map<char, Value> registers;
	std::queue<Value>& inputs;
	std::queue<Value>& outputs;
	Value pc = 0;
	Value count = 0;
	Value lastSound = 0;
	bool finished = false;

	Value value( const char* val )
	{
		if ( isalpha( *val ) )
		{
			if ( registers.count( *val ) == 0 )
			{
				return 0;
			}
			else
			{
				return registers[*val];
			}
		}
		else
		{
			return ( atoll( val ) );
		}
	}

	int process( int part )
	{
		int result = 0;
		finished = false;

		while ( !finished && pc >= 0 && pc < assembly.size() )
		{
			std::string& line = assembly[pc];
			char reg = line[4];
			if ( registers.count( reg ) == 0 )
			{
				registers.insert( std::make_pair( reg, 0 ) );
				if ( !isalpha( reg ) )
				{
					// Hack: if a number is used, store it as a register with its own value
					registers[reg] = reg - '0';
				}
			}
			const char *val = line.c_str() + 6;
			switch ( CASE( line.c_str() ) )
			{
				case CASE( "set" ):
					registers[reg] = value( val );
					break;
				case CASE( "mul" ):
					registers[reg] *= value( val );
					++result;
					break;
				case CASE( "sub" ):
					registers[reg] -= value( val );
					break;
// 				case CASE( "add" ):
// 					registers[reg] += value( val );
// 					break;
// 				case CASE( "mod" ):
// 					registers[reg] %= value( val );
// 					break;
// 				case CASE( "snd" ):
// 					++count;
// 					lastSound = registers[reg];
// 					//					printf( "snd %lld\n", registers[reg] );
// 					if ( part == 2 )
// 					{
// 						outputs.push( registers[reg] );
// 					}
// 					break;
// 				case CASE( "rcv" ):
// 					if ( part == 2 )
// 					{
// 						if ( inputs.empty() )
// 						{
// 							finished = true;
// 						}
// 						else
// 						{
// 							registers[reg] = inputs.front();
// 							inputs.pop();
// 						}
// 					}
// 					//					printf( "rcv %lld\n", registers[reg] );
// 					if ( part == 1 && registers[reg] != 0 )
// 					{
// 						finished = true;
// 					}
// 					break;
// 				case CASE( "jgz" ):
// 					if ( registers[reg] > 0 )
// 					{
// 						pc += value( val );
// 						continue;
// 					}
// 					break;
				case CASE( "jnz" ):
					if ( registers[reg] != 0 )
					{
						pc += value( val );
						continue;
					}
					break;
				default:
					assert( false );
					finished = true;
					break;
			}
			if ( !finished )
			{
				++pc;
			}
		}
		return result;
//		return pc < 0 || pc >= assembly.size();
	}
};

int main()
{
	static const char* filename = "input.txt";

	if ( load( filename ) )
	{
		std::queue<Value> p0_to_p1;
		std::queue<Value> p1_to_p0;
		// part 1
		Processor p( 0, p0_to_p1, p1_to_p0 );
		int part1 = p.process( 1 );
		printf( "Part 1: %d\n", part1 );

 		// part 2: see comment block below working out what the algorithm is calculating
		int part2 = 0;
		for ( int b = 108400; b <= 125400; b += 17 )
		{
			// sqrt(125400) ~= 354
			for ( int t = 2; t < 355; ++t )
			{
				if ( (b % t) == 0 )
				{
					// not prime
					++part2;;
					break;
				}
			}
		}
		printf( "Part 2: %d\n", part2 );
	}
	return 0;
}

// Part 2
/*
	// Count how many numbers between 108400 and 125400 (step 17) are *not* prime numbers

				int b=0, c=0, d=0, e=0, f=0, g=0, h=0;
	set b 84	b = 84;				b = (part == 1) ? 84 : 108400;
	set c b		c = b;				c = (part == 1) ? 84 : 125400;
	jnz a :1	if (a) 
	jnz 1 :8	{
:1	mul b 100		b *= 100;
	sub b -100000	b += 100000
	set c b			c = b;
	sub c -17000	c += 17000;
				}

								for ( int b = 108400; b <= 125400; b += 17)
								{
:8	set f 1		f = 1;
	set d 2		d = 2;				for ( int d = 2; d != b; ++d ) {
:4	set e 2		e = 2;					for ( int e = 2; e != b; ++e) {
	set g d		g = d;						if (d * e == b) ++h;
	mul g e		g *= e;
	sub g b		g -= b;
	jnz g :2	if ( g == 0 )
	set f 0		{ f = 0; }
:2	sub e -1	++e;
	set g e		g = e - b;
										}
	sub g b
	jnz g :3	if ( g != 0 ) goto :3
	sub d -1	++d;
	set g d		g = d;
	sub g b		g -= b;				} 
	jnz g :4
	jnz f :5
	sub h -1	++h;
:5	set g b
	sub g c
	jnz g :6					}
	jnz 1 :7
:6	sub b -17	b += 17;
	jnz 1 :8
:7
*/
