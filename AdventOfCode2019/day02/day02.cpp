// day2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Value = _int64;

Value process( int a, int b )
{
	char line[1024];
	std::vector<Value> numbers;
	FILE *fp = fopen( "example.txt", "rt" );
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
	numbers[1] = a;
	numbers[2] = b;
	int pc = 0;
	Value opcode = 0;
	do 
	{
		opcode = numbers[pc++];
		if ( opcode == 99 )
			break;
		Value input1 = numbers[pc++];
		Value input2 = numbers[pc++];
		Value output = numbers[pc++];
		switch ( opcode )
		{
			case 1: 
			{
				//printf( "ADD %d %d -> %d\n", input1, input2, output );
				Value i1 = numbers[input1];
				Value i2 = numbers[input2];
				Value out = numbers[output];
				numbers[output] = i1 + i2;
				//printf( "%lld + %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
				break;
			}
			case 2:
			{
				//printf( "MULT %d %d -> %d\n", input1, input2, output );
				Value i1 = numbers[input1];
				Value i2 = numbers[input2];
				Value out = numbers[output];
				numbers[output] = i1 * i2;
				//printf( "%lld * %lld = %lld (was %lld)\n", i1, i2, numbers[output], out );
				break;
			}
			case 99:
				//printf( "STOP\n" );
				break;
			default:
				printf( "Unknown op: %lld\n", opcode );

		}
	} while ( opcode != 99 );
//	printf( "Answer = %lld\n", numbers[0] );

	return numbers[0];
}

int main()
{
	for ( int a = 0; a < 100; ++a )
	{
		for ( int b = 0; b < 100; ++b )
		{
			Value answer = process( a, b );
			printf( "%d, %d -> %lld\n", a, b, answer );
			if ( answer == 19690720l )
			{
				printf( " WINNER \n" );
			}
		}
	}
}
