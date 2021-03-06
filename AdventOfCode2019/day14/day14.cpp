// day14.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct quantity
{
	quantity() : id( 0 ), amount( 0 ) {}
	quantity( const char*n, __int64 a ) : id( 0 ), amount( a ) { strcpy( name, n ); }
	union
	{
		char name[8];
		__int64 id;
	};
	__int64 amount;
};

struct reaction
{
	void parse( char *line );

	// How much extra have we already produced?
	__int64 overflow = 0;

	// What is produced by this reaction (e.g. 4 BDRP)
	quantity output;

	// What is used up by this reaction
	__int64 input_count = 0;
	quantity inputs[10];
};


void reaction::parse( char* line )
{
	char* _inputs = strtok( line, "=>" );
	char* _output = strtok( nullptr, "=>" );

	char* x;
	char* input = strtok_s( _inputs, ",", &x );
	while ( input )
	{
		char* y;
		char* amount = strtok_s( input, " ", &y );
		char* name = strtok_s( nullptr, " ", &y );
		assert( strlen( name ) < 8 );
		assert( input_count < 10 );
		inputs[input_count].amount = atoi( amount );
		strcpy( inputs[input_count].name, name );
		++input_count;

		input = strtok_s( nullptr, ",", &x );
	}

	char* amount = strtok( _output, " " );
	char* name = strtok( nullptr, " " );
	assert( strlen( name ) < 8 );
	output.amount = atoi( amount );
	strcpy( output.name, name );
}

std::map<__int64, reaction> reactions;

void reset()
{
	for ( auto& r : reactions )
	{
		r.second.overflow = 0;
	}
}

__int64 cost( const quantity& q, __int64 m = 1 )
{
	if ( strcmp( q.name, "ORE" ) == 0 )
	{
		return m * q.amount;
	}

	__int64 output = 0;
	__int64 mult = 1;
	__int64 amount = q.amount * m;
	reaction& r = reactions[q.id];
	if ( amount <= r.overflow )
	{
		r.overflow -= amount;
		return 0;
	}
	amount -= r.overflow;
	if ( r.output.amount < amount )
	{
		mult = ( r.output.amount + amount - 1 ) / r.output.amount;
	}
	r.overflow = mult * r.output.amount - amount;
	for ( __int64 i = 0; i < r.input_count; ++i )
	{
		quantity& input = r.inputs[i];
		output += cost( input, mult );
	}
	return output;
}

__int64 cost( __int64 f )
{
	quantity fuel( "FUEL", f );
	return cost( fuel );
}

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
//	FILE *fp = fopen( "example1.txt", "rt" );
//	FILE *fp = fopen( "example2.txt", "rt" );
//	FILE *fp = fopen( "example3.txt", "rt" );
// 	FILE *fp = fopen( "example4.txt", "rt" );
// 	FILE *fp = fopen( "example5.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			// e.g. "3 PTZH, 14 MHDKS, 9 MPBVZ => 4 BDRP"
			reaction r;
			r.parse( thisLine );
			reactions.insert( std::make_pair( r.output.id, r ) );
		}
	}
	fclose( fp );
	delete[] buffer;

	quantity fuel("FUEL", 1);
	__int64 c = cost( fuel );
	printf( "%lld ORE => %lld %s\n", c, fuel.amount, fuel.name );
	
	__int64 target = 1000000000000LL;
	__int64 low = target / c;
	__int64 lowc = cost( low );
	assert( lowc < target );
	__int64 high = low * 2;
	__int64 highc = cost( high );
	assert( highc > target );
	do 
	{
		reset();
		__int64 mid = ( low + high ) / 2;
		c = cost( mid );
		if ( c < target )
		{
			low = mid;
		}
		else if ( c > target )
		{
			high = mid;
		}
		else
		{
			// unlikely
			low = high = mid;
		}
		printf( "%lld ORE => %lld FUEL\n", c, mid );
	} while (low < high-1);
	c = cost( low );
	printf( "%lld ORE => %lld FUEL\n", c, low );
	return 0;
}

