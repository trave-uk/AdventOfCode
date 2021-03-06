// day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = __int64;

class cpu
{
public:
	void init();
	Value getValue( Value input, int addressModes, int parameterIndex );
	void push_input( Value input ) { inputs.push_back( input ); }
	bool process( Value& result );

private:
	std::vector<Value> numbers;
	int pc;
	std::deque<int> inputs;
};


Value cpu::getValue( Value input, int addressModes, int parameterIndex )
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
	else if ( parameterMode == 0 )
		return numbers[input];
	assert( false );
	return 0;
}

void cpu::init()
{
	char line[1024];
	FILE *fp = fopen( "input.txt", "rt" );
	numbers.reserve( 200 );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 1024, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// comma separated
			char* tok = strtok( line, ", " );
			while ( tok )
			{
				Value number = atoi( tok );
				numbers.push_back( number );
				tok = strtok( nullptr, ", " );
			}
		}
	}
	fclose( fp );
	pc = 0;
}

bool cpu::process( Value& result )
{
	Value opcode = 0;
	Value returnValue = -1;
	do
	{
		opcode = numbers[pc++];
		Value addressModes = opcode / 100;
		opcode %= 100;
		if ( opcode == 99 )
			break;
		switch ( opcode )
		{
			case 1:
			{
				Value input1 = numbers[pc++];
				Value input2 = numbers[pc++];
				Value output = numbers[pc++];
				//printf( "ADD%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value out = getValue( output, addressModes, 2 );
				numbers[output] = i1 + i2;
				//printf( "%lld + %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
				break;
			}
			case 2:
			{
				Value input1 = numbers[pc++];
				Value input2 = numbers[pc++];
				Value output = numbers[pc++];
				//printf( "MULT%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value out = getValue( output, addressModes, 2 );
				numbers[output] = i1 * i2;
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
				break;
			}
			case 3: // input
			{
				if ( inputs.size() == 0 )
				{
					pc--;
					return false;
				}
				Value output = numbers[pc++];
				Value out = getValue( output, addressModes, 0 );
				assert( inputs.size() > 0 );
				numbers[output] = inputs.front();
				inputs.pop_front();
				break;
			}
			case 4: // output
			{
				Value input = numbers[pc++];
				result = getValue( input, addressModes, 0 );
				printf( "Output: %d\n", result );
				break;
			}
			case 5: // jump if tree
			{
				Value input = numbers[pc++];
				Value address = numbers[pc++];
				//printf( "JT%02d %d %d\n", addressModes, input, address );
				Value i = getValue( input, addressModes, 0 );
				Value a = getValue( address, addressModes, 1 );
				if ( i )
				{
					pc = a;
				}
				break;
			}
			case 6: // jump if false
			{
				Value input = numbers[pc++];
				Value address = numbers[pc++];
				//printf( "JF%02d %d %d\n", addressModes, input, address );
				Value i = getValue( input, addressModes, 0 );
				Value a = getValue( address, addressModes, 1 );
				if ( !i )
				{
					pc = a;
				}
				break;
			}
			case 7: // less then
			{
				Value input1 = numbers[pc++];
				Value input2 = numbers[pc++];
				Value output = numbers[pc++];
				//printf( "LT%03d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value out = getValue( output, addressModes, 2 );
				numbers[output] = (i1 < i2);
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
				break;
			}
			case 8: // equals
			{
				Value input1 = numbers[pc++];
				Value input2 = numbers[pc++];
				Value output = numbers[pc++];
				//printf( "EQUALS%d %d %d -> %d\n", addressModes, input1, input2, output );
				Value i1 = getValue( input1, addressModes, 0 );
				Value i2 = getValue( input2, addressModes, 1 );
				Value out = getValue( output, addressModes, 2 );
				numbers[output] = (i1 == i2);
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
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
	//	printf( "Answer = %lld\n", numbers[0] );

	// finished
	return true;
}

Value tryOrder( std::array<int, 5>& order )
{
	std::array<cpu, 5> cpus;
	for ( int i = 0; i < order.size(); ++i )
	{
		cpus[i].init();
		cpus[i].push_input( order[i] );
	}
	bool finished = false;
	Value power = 0;

	do
	{
		for ( int i = 0; i < order.size(); ++i )
		{
			cpus[i].push_input( power );
			bool thisFinished = cpus[i].process( power );
			if ( thisFinished )
			{
				printf( "%d finished\n", i );
			}
			finished = finished || thisFinished;
		}
	} while ( !finished );
	return power;
}

int main()
{
	std::set<int> used;
	std::array<int, 5> order;
	std::vector<char> display( 6 );
	display.assign( 5, '0' );
	display.push_back( '\0' );
	// Iterate through all possible orders of 0-4
	Value highestOutput = 0;
	for ( int a = 5; a < 10; ++a )
	{
		order[0] = a;
		used.insert( a );
		display[0] = ( a + '0' );
		for ( int b = 5; b < 10; ++b )
		{
			if ( used.count( b ) != 0 )
				continue;
			order[1] = b;
			used.insert( b );
			display[1] = ( b + '0' );
			for ( int c = 5; c < 10; ++c )
			{
				if ( used.count( c ) != 0 )
					continue;
				order[2] = c;
				used.insert( c );
				display[2] = ( c + '0' );
				for ( int d = 5; d < 10; ++d )
				{
					if ( used.count( d ) != 0 )
						continue;
					order[3] = d;
					used.insert( d );
					display[3] = ( d + '0' );
					for ( int e = 5; e < 10; ++e )
					{
						if ( used.count( e ) != 0 )
							continue;
						order[4] = e;
						used.insert( e );
						display[4] = ( e + '0' );
//						printf( "%s\n", &display.front() );
						Value result = tryOrder( order );
						printf( "%s -> %d\n", &display.front(), result );
						if ( result > highestOutput )
							highestOutput = result;
						used.erase( e );
					}
					used.erase( d );
				}
				used.erase( c );
			}
			used.erase( b );
		}
		used.erase( a );
	}
	printf( "Highest result: %lld\n", highestOutput );
}
