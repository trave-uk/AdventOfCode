// day23.cpp : Defines the entry point for the console application.
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
	enum result { STARTING, FINISHED, NEEDINPUT, MAXOUTPUT };
	void init();
	void poke( size_t address, Value value );
	Value& getValue( Value input, int addressModes, int parameterIndex );
	void push_input( Value input ) { inputs.push_back( input ); }
	size_t inputsize() const { return inputs.size(); }
	void push_string( const char* input );
	bool pop_output( Value& output );
	size_t outputsize() const { return outputs.size(); }
	result process( int maxout = 0 );
	Value& deref( ValueP address );
	result lastResult() const { return lastresult; }
	bool isidle() const	{ return outputsize() == 0 && inputsize() == 0 && lastResult() == cpu::NEEDINPUT; }


private:
	std::vector<Value> numbers;
	ValueP pc = 0;
	std::deque<Value> inputs;
	std::deque<Value> outputs;
	ValueP rb = 0;
	result lastresult = STARTING;
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

bool cpu::pop_output( Value& output )
{
	if ( outputs.size() )
	{
		output = outputs.front();
		outputs.pop_front();
		return true;
	}
	else
	{
		return false;
	}
}

cpu::result cpu::process( int maxout )
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
					return lastresult = NEEDINPUT;
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
					return lastresult = MAXOUTPUT;
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
	return lastresult = FINISHED;
}


void gotoxy( short x, short y )
{
	static HANDLE h = NULL;
	if ( !h )
		h = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD c = { x, y };
	SetConsoleCursorPosition( h, c );
}

cpu CPU[50];

int main()
{
	for ( int i = 0; i < 50; ++i )
	{
		CPU[i].init();
		CPU[i].push_input( i );
	}
	bool finished = false;
	Value natx = 0, naty = 0, firsty = 0, lasty = 0;
	do 
	{
		int idle = 0;
		for ( int i = 0; i < 50; ++i )
		{
			if ( CPU[i].lastResult() == cpu::NEEDINPUT && CPU[i].inputsize() == 0 )
			{
				CPU[i].push_input( -1 );
			}
			cpu::result status = CPU[i].process( 3 );
			if ( CPU[i].isidle() )
			{
				++idle;
			}
			Value address;
			while ( CPU[i].pop_output( address ) )
			{
				Value x, y;
				if ( !CPU[i].pop_output( x ) )
				{
					assert( false );
				}
				if ( !CPU[i].pop_output( y ) )
				{
					assert( false );
				}
				if ( address == 255 )
				{
					if ( firsty == 0 )
					{
						firsty = y;
						printf( "y = %lld sent to 255 first\n", firsty );
					}
					natx = x;
					naty = y;
				}
				else if ( address < 50 && address >= 0 )
				{
					CPU[address].push_input( x );
					CPU[address].push_input( y );
				}
				else
				{
					assert( false );
				}
			}
		}
		if ( idle == 50 )
		{
			if ( lasty != 0 && lasty == naty )
			{
				printf( "y = %lld sent by NAT to 0 twice in a row\n", naty );
				finished = true;
			}
			CPU[0].push_input( natx );
			CPU[0].push_input( naty );
			lasty = naty;
		}
	} while ( !finished );
}