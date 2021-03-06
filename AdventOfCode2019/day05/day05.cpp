// day05.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = int;

std::vector<Value> numbers;

Value getValue( Value input, int addressModes, int parameterIndex )
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

//Value process( int a, int b )
int main()
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

	//	printf( "%lld numbers read\n", numbers.size() );

		// Process
// 	numbers[1] = a;
// 	numbers[2] = b;
	int pc = 0;
	Value opcode = 0;
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
				Value output = numbers[pc++];
				Value out = getValue( output, addressModes, 0 );
				// The only input is 5
				numbers[output] = 5;
				break;
			}
			case 4: // output
			{
				Value input = numbers[pc++];
				printf( "Output: %d\n", getValue(input, addressModes, 0 ) );
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

	return 0;
}

// int main()
// {
// 	for ( int a = 0; a < 100; ++a )
// 	{
// 		for ( int b = 0; b < 100; ++b )
// 		{
// 			Value answer = process( a, b );
// 			printf( "%d, %d -> %lld\n", a, b, answer );
// 			if ( answer == 19690720l )
// 			{
// 				printf( " WINNER \n" );
// 			}
// 		}
// 	}
// }
