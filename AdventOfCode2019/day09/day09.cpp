// day9.cpp : Defines the entry point for the console application.
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
	void init();
	Value& getValue( Value input, int addressModes, int parameterIndex );
	void push_input( Value input ) { inputs.push_back( input ); }
	bool process( Value& result );
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
			parameterMode = (addressModes % 100) / 10;
			break;
		case 2:
			parameterMode = (addressModes % 1000) / 100;
			break;
	}
	if ( parameterMode == 1 )
		return input;
	else
	{
		ValueP address = 0;
		if ( parameterMode == 0 )
			address = input;
		else if ( parameterMode == 2 )
			address = rb + ValueP(input);
		else
			assert( false );
		return deref( address );
	}
}

void cpu::init()
{
	char line[4096];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
//	FILE *fp = fopen( "..\\day07\\input.txt", "rt" );
	numbers.reserve( 200 );
	int longest = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 4096, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// comma separated
			char* tok = strtok( line, ", " );
			while ( tok )
			{
				if ( strlen( tok ) >= longest )
				{
					longest = strlen( tok );
					printf( "Longest so far: %s\n", tok );
				}
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

bool cpu::process( Value& result )
{
	int opcode = 0;
	Value returnValue = -1;
	do
	{
		opcode = deref(pc++);
		int addressModes = opcode / 100;
		opcode %= 100;
		if ( opcode == 99 )
			break;
		switch ( opcode )
		{
			case 1:
			{
				Value input1 = deref( pc++ );
				Value input2 = deref(pc++);
				Value output = deref(pc++);
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
				Value input1 = deref(pc++);
				Value input2 = deref(pc++);
				Value output = deref(pc++);
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
					return false;
				}
				Value output = deref(pc++);
				Value& out = getValue( output, addressModes, 0 );
				assert( inputs.size() > 0 );
				out = inputs.front();
				inputs.pop_front();
				break;
			}
			case 4: // output
			{
				Value input = deref(pc++);
				result = getValue( input, addressModes, 0 );
// 				char a[100];
// 				_i128toa( result, a, 10 );
// 				printf( "Output: %s\n", a );
				printf( "Output: %lld\n", result );
				break;
			}
			case 5: // jump if true
			{
				Value input = deref(pc++);
				Value address = deref(pc++);
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
				Value input = deref(pc++);
				Value address = deref(pc++);
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
				Value input1 = deref(pc++);
				Value input2 = deref(pc++);
				Value output = deref(pc++);
				//printf( "LT%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = (i1 < i2);
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 8: // equals
			{
				Value input1 = deref(pc++);
				Value input2 = deref(pc++);
				Value output = deref(pc++);
				//printf( "EQUALS%d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value& out = getValue( output, addressModes, 2 );
				out = (i1 == i2);
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, deref(output), out );
				break;
			}
			case 9: // adjust relative base
			{
				Value input = deref(pc++);
				Value i = getValue( input, addressModes, 0 );
				rb = rb + ValueP(i);
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
	return true;
}

int main()
{
	cpu CPU;
	CPU.init();
	CPU.push_input( 2 );
	Value result;
	CPU.process(result);
	return 0;
}

