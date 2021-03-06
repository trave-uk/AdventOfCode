// day18.cpp : Defines the entry point for the console application.
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
		: inputs(input_queue)
		, outputs(output_queue)
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

	bool process( int part )
	{
		finished = false;

		while ( !finished && pc >= 0 && pc < assembly.size() )
		{
			std::string& line = assembly[pc];
			char reg = line[4];
			if ( registers.count( reg ) == 0 )
			{
				registers.insert( std::make_pair( reg, 0 ) );
				if (!isalpha(reg))
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
					break;
				case CASE( "add" ):
					registers[reg] += value( val );
					break;
				case CASE( "mod" ):
					registers[reg] %= value( val );
					break;
				case CASE( "snd" ):
					++count;
					lastSound = registers[reg];
//					printf( "snd %lld\n", registers[reg] );
					if ( part == 2 )
					{
						outputs.push( registers[reg] );
					}
					break;
				case CASE( "rcv" ):
					if ( part == 2 )
					{
						if ( inputs.empty() )
						{
							finished = true;
						}
						else
						{
							registers[reg] = inputs.front();
							inputs.pop();
						}
					}
//					printf( "rcv %lld\n", registers[reg] );
					if ( part == 1 && registers[reg] != 0 )
					{
						finished = true;
					}
					break;
				case CASE( "jgz" ):
					if ( registers[reg] > 0 )
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
		return pc < 0 || pc >= assembly.size();
	}

};

int main()
{
	static const char* filename =
 		"input.txt";
//		"example.txt";
//		"example2.txt";

	if ( load( filename ) )
	{
		std::queue<Value> p0_to_p1;
		std::queue<Value> p1_to_p0;
		// part 1
		Processor p( 0, p0_to_p1, p1_to_p0 );
		p.process(1);
		printf( "Part 1: %lld\n", p.lastSound );

		// part 2
		Processor p0( 0, p1_to_p0, p0_to_p1 );
		Processor p1( 1, p0_to_p1, p1_to_p0 );
		bool p0finished = false;
		bool p1finished = false;
		do 
		{
//			printf( "Program 0\n" );
			p0finished = p0.process( 2 );
			if (p0_to_p1.empty())
				break;
//			printf( "Program 1\n" );
			p1finished = p1.process( 2 );
		} while ( !p1_to_p0.empty() );
		printf( "Part 2: %lld\n", p1.count );
	}
	return 0;
}

