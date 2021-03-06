// day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = __int64;
Value one( 1 );
Value zero( 0 );
Value hundred( 100 );

using ValueP = __int64;
class cpu
{
public:
	enum result { FINISHED, NEEDINPUT, MAXOUTPUT };
	void init();
	void poke( size_t address, Value value );
	Value& getValue( Value input, int addressModes, int parameterIndex );
	void push_input( Value input ) { inputs.push_back( input ); }
	void push_string( const char* input );
	result process( std::deque<Value>& outputs, int maxout = 0 );
	Value& deref( ValueP address );

private:
	std::vector<Value> numbers;
	ValueP pc = 0;
	std::deque<int> inputs;
	ValueP rb = 0;
};


Value& cpu::deref( ValueP address )
{
	ValueP size( numbers.size() );
	if ( address >= size )
	{
		Value diff = address - size + 1;
		while ( diff > zero )
		{
			numbers.push_back( zero );
			diff -= one;
		}
	}
	return numbers[address];
}

Value& cpu::getValue( Value input, int addressModes, int parameterIndex )
{
	int parameterMode = 0;
	switch ( parameterIndex )
	{
		case 0:
			parameterMode = addressModes % 10;
			break;
		case 1:
			parameterMode = ( addressModes % 100 ) / 10;
			break;
		case 2:
			parameterMode = ( addressModes % 1000 ) / 100;
			break;
	}
	if ( parameterMode == 1 )
	{
		static Value s_input = 0;
		s_input = input;
		return s_input;
	}
	else
	{
		ValueP address = 0;
		if ( parameterMode == 0 )
			address = input;
		else if ( parameterMode == 2 )
			address = rb + ValueP( input );
		else
			assert( false );
		return deref( address );
	}
}

void cpu::init()
{
	char line[10000];
	FILE *fp = fopen( "input.txt", "rt" );
	numbers.reserve( 4000 );
	numbers.clear();
	//	int longest = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 10000, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// comma separated
			char* tok = strtok( line, ", " );
			while ( tok )
			{
				//				if ( strlen( tok ) >= longest )
				//				{
				//					longest = strlen( tok );
				//					printf( "Longest so far: %s\n", tok );
				//				}
								//				Value number = Value( tok );
				Value number = atoll( tok );
				numbers.push_back( number );
				tok = strtok( nullptr, ", " );
			}
		}
	}
	fclose( fp );
	pc = 0;
	rb = 0;
}

void cpu::poke( size_t address, Value value )
{
	assert( address < numbers.size() );
	numbers[address] = value;
}

void cpu::push_string( const char* input )
{
	while ( *input )
	{
		push_input( *input );
		++input;
	}
}

cpu::result cpu::process( std::deque<Value>& outputs, int maxout )
{
	int opcode = 0;
	Value returnValue = -1;
	do
	{
		opcode = deref( pc++ );
		int addressModes = opcode / 100;
		opcode %= 100;
		if ( opcode == 99 )
			break;
		switch ( opcode )
		{
			case 1:
			{
				Value input1 = deref( pc++ );
				Value input2 = deref( pc++ );
				Value output = deref( pc++ );
				//printf( "ADD%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = i1 + i2;
				//printf( "%lld + %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 2:
			{
				Value input1 = deref( pc++ );
				Value input2 = deref( pc++ );
				Value output = deref( pc++ );
				//printf( "MULT%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = i1 * i2;
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 3: // input
			{
				if ( inputs.size() == 0 )
				{
					pc--;
					return NEEDINPUT;
				}
				Value output = deref( pc++ );
				Value& out = getValue( output, addressModes, 0 );
				assert( inputs.size() > 0 );
				out = inputs.front();
				inputs.pop_front();
				break;
			}
			case 4: // output
			{
				Value input = deref( pc++ );
				Value result = getValue( input, addressModes, 0 );
				// 				char a[100];
				// 				_i128toa( result, a, 10 );
				// 				printf( "Output: %s\n", a );
				outputs.push_back( result );
				if ( maxout > 0 && outputs.size() >= maxout )
				{
					return MAXOUTPUT;
				}
				//				printf( "Output: %lld\n", result );
				break;
			}
			case 5: // jump if true
			{
				Value input = deref( pc++ );
				Value address = deref( pc++ );
				//printf( "JT%02d %d %d\n", addressModes, input, address );
				Value i = getValue( input, addressModes, 0 );
				Value a = getValue( address, addressModes, 1 );
				if ( i != zero )
				{
					pc = a;
				}
				break;
			}
			case 6: // jump if false
			{
				Value input = deref( pc++ );
				Value address = deref( pc++ );
				//printf( "JF%02d %d %d\n", addressModes, input, address );
				Value i = getValue( input, addressModes, 0 );
				Value a = getValue( address, addressModes, 1 );
				if ( i == zero )
				{
					pc = a;
				}
				break;
			}
			case 7: // less then
			{
				Value input1 = deref( pc++ );
				Value input2 = deref( pc++ );
				Value output = deref( pc++ );
				//printf( "LT%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = ( i1 < i2 );
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 8: // equals
			{
				Value input1 = deref( pc++ );
				Value input2 = deref( pc++ );
				Value output = deref( pc++ );
				//printf( "EQUALS%d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = ( i1 == i2 );
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 9: // adjust relative base
			{
				Value input = deref( pc++ );
				Value i = getValue( input, addressModes, 0 );
				rb = rb + ValueP( i );
				break;
			}
			case 99:
				//printf( "STOP\n" );
				break;
			default:
				printf( "Unknown op: %d\n", opcode );
				break;
		}
	} while ( opcode != 99 );
	//	printf( "Answer = %lld\n", deref(0) );

	// finished
	return FINISHED;
}


void gotoxy( int x, int y )
{
	static HANDLE h = NULL;
	if ( !h )
		h = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD c = { x, y };
	SetConsoleCursorPosition( h, c );
}

cpu CPU;
std::deque<Value> outputs;

// A indicates whether there is ground one tile away
// B indicates whether there is ground two tiles away
// C indicates whether there is ground three tiles away
// D indicates whether there is ground four tiles away
// E indicates whether there is ground five tiles away.
// F indicates whether there is ground six tiles away.
// G indicates whether there is ground seven tiles away.
// H indicates whether there is ground eight tiles away.
// I indicates whether there is ground nine tiles away.
// J = Jump
// T = Temp
// AND X Y sets Y to true if both X and Y are true; otherwise, it sets Y to false.
// OR X Y sets Y to true if at least one of X or Y is true; otherwise, it sets Y to false.
// NOT X Y sets Y to true if X is false; otherwise, it sets Y to false.

int main()
{
	CPU.init();
	// J = (!C && D) || !A
// 	CPU.push_string( "NOT C J\n" );
// 	CPU.push_string( "AND D J\n" );
// 	CPU.push_string( "NOT A T\n" );
// 	CPU.push_string( "OR T J\n" );
// 	CPU.push_string( "WALK\n" );

	// #####.#..########
	//    o   o   o   o
	// #####.##.#.#..###
	// #####..###.#..###
	//    o   o   o
	// #####.##...######*
	// #####...##.##.###
	// J = (!E && !F && !G && D && H) || (!C && D && H) || (!B && !I && D && H) || !A || (!B && D && !E) 
 	CPU.push_string( "NOT E J\n" );
 	CPU.push_string( "NOT F T\n" );
 	CPU.push_string( "AND T J\n" );
 	CPU.push_string( "NOT G T\n" );
 	CPU.push_string( "AND T J\n" );
	CPU.push_string( "AND D J\n" );
	CPU.push_string( "AND H J\n" );

	CPU.push_string( "NOT C T\n" );
	CPU.push_string( "AND D T\n" );
 	CPU.push_string( "AND H T\n" );
	CPU.push_string( "OR T J\n" );

	CPU.push_string( "OR I T\n" );
	CPU.push_string( "OR B T\n" );
	CPU.push_string( "NOT T T\n" ); // T = !B && !I
	CPU.push_string( "AND D T\n" );
	CPU.push_string( "AND H T\n" );
	CPU.push_string( "OR T J\n" );

	CPU.push_string( "NOT A T\n" );
	CPU.push_string( "OR T J\n" );

	CPU.push_string( "OR B T\n" );
	CPU.push_string( "OR E T\n" ); // T = B || E
	CPU.push_string( "NOT T T\n" ); // T = !B && !E
	CPU.push_string( "AND D T\n" );
	CPU.push_string( "OR T J\n" );

	//    o   o   o   o
	// #####.##.#.#..###
	//    o   o   o   o
	   // #####.###..#..###
	// (D && H && !G && !F && !I)
// 	CPU.push_string( "OR G T\n" );
// 	CPU.push_string( "OR F T\n" );
// 	CPU.push_string( "OR I T\n" ); // T = G || F || I
//  	CPU.push_string( "NOT T T\n" ); // T = !G && !F && !I
// 	CPU.push_string( "AND D T\n" );
// 	CPU.push_string( "AND H T\n" );
// 	CPU.push_string( "OR T J\n" );

	CPU.push_string( "RUN\n" );
	CPU.process( outputs );

	for ( Value output : outputs )
	{
		if ( output < 128 )
		{
			printf( "%c", output );
		}
		else
		{
			printf( "%lld\n", output );
		}
	}
}
