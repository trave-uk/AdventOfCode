// day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum OP
{
	NEWSTACK,
	CUT,
	DEALINCREMENT
};
struct shuffle
{
	OP op;
	__int64 amount;
};
std::vector<shuffle> operations;

void _switch( __int64 &a, __int64 &b )
{
	__int64 t = a;
	a = b;
	b = t;
}

// greatest common divisor
__int64 gcd( __int64 a, __int64 b )
{
	if ( !a )
		return b;
	if ( !b )
		return a;
	// Using the Euclidean algorithm https://en.wikipedia.org/wiki/Euclidean_algorithm
	if ( a > b )
	{
		_switch( a, b );
	}

	__int64 result = 1LL;
	do
	{
		result = a;
		while ( b >= a )
		{
			b -= a;
		}
		_switch( a, b );
	} while ( a > 0 );
	return result;
}

bool load( const char *filename )
{
	operations.clear();
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
				std::string line = thisLine;
				shuffle s;
				// deal into new stack
				if ( line.compare( "deal into new stack" ) == 0 )
				{
					s.op = NEWSTACK;
					s.amount = 0LL;
				}
				// deal with increment %i
				else if ( line.compare( 0, 20, "deal with increment " ) == 0 )
				{
					s.op = DEALINCREMENT;
					std::string incrementstring = line.substr( 20 );
					s.amount = std::stoi( incrementstring );
				}
				// cut %i
				else if ( line.compare( 0, 4, "cut " ) == 0 )
				{
					s.op = CUT;
					std::string cutstring = line.substr( 4 );
					s.amount = std::stoi( cutstring );
				}
				else
				{
					assert( false );
				}
				operations.push_back( s );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

void deal( __int64* cards, __int64 ncards, __int64 start, __int64 increment )
{
	assert( gcd( ncards, increment ) == 1 );
	__int64* copy = new __int64[ncards];
	memcpy( copy, cards, ncards * sizeof( __int64 ) );
	__int64 index = start;
	for ( __int64 i = 0LL; i < ncards; ++i )
	{
		cards[index] = copy[i];
		index += ncards - increment;
		index %= ncards;
	}
	delete[] copy;
}

void cut( __int64* cards, __int64 ncards, __int64 amount )
{
	if ( amount < 0 )
	{
		amount += ncards;
	}
	assert( amount >= 0 && amount < ncards );
	__int64* half = new __int64[amount];
	memcpy( half, cards, amount * sizeof( __int64 ) );
	memmove( cards, cards + amount, ( ncards - amount ) * sizeof( __int64 ) );
	memcpy( cards + ncards - amount, half, amount * sizeof( __int64 ) );
	delete[] half;
}

void test( const char* filename, __int64 ncards, __int64 repeats = 1, __int64 target = -1 )
{
	printf( "Testing %s with %lld cards, %lld repeats\n", filename, ncards, repeats );
	load( filename );
	__int64 *cards = new __int64[ncards];
	for ( __int64 i = 0LL; i < ncards; ++i )
	{
		cards[i] = i;
	}

	// process
	for ( __int64 i = 0LL; i < repeats; ++i )
	{
		for ( shuffle op : operations )
		{
			switch ( op.op )
			{
				case NEWSTACK:
					// invert stack
					deal( cards, ncards, ncards - 1, ncards - 1 );
					break;

				case DEALINCREMENT:
					// deal with increment %i
					deal( cards, ncards, 0, op.amount );
					break;

				case CUT:
					// cut %i
					cut( cards, ncards, op.amount );
					break;
			}
		}
	}
	// output
	if ( target == -1 )
	{
		printf( "Result: " );
		for ( __int64 i = 0LL; i < ncards; ++i )
		{
			printf( "%lld ", cards[i] );
		}
		printf( "\n\n" );
	}
	else
	{
		for ( __int64 i = 0LL; i < ncards; ++i )
		{
			if ( cards[i] == target )
			{
				printf( "%lld found in position %lld\n\n", target, i );
			}
		}
	}
	delete[] cards;
}

// __int64 mulmod( __int64 a, __int64 b, __int64 mod )
// {
// 	__int64 res = 0LL; // Initialize result 
// 	a = a % mod;
// 	while ( b > 0 )
// 	{
// 		// If b is odd, add 'a' to result 
// 		if ( b % 2 == 1 )
// 			res = ( res + a ) % mod;
// 
// 		// Multiply 'a' with 2 
// 		a = ( a * 2 ) % mod;
// 
// 		// Divide b by 2 
// 		b /= 2LL;
// 	}
// 
// 	// Return result 
// 	return res % mod;
// }
// 
__int64 index;
inline void calculate()
{
//#define NCARDS 119315717514047LL
#define NCARDS 10007LL

	index *= 65LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 25LL; index %= NCARDS;
	index += 6735LL; index %= NCARDS;
	index *= 3LL; index %= NCARDS;
	index += NCARDS - 8032LL; index %= NCARDS;
	index *= 71LL; index %= NCARDS;
	index += 4990LL; index %= NCARDS;
	index *= 66LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += 8040LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 18LL; index %= NCARDS;
	index += 8746LL; index %= NCARDS;
	index *= 42LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 17LL; index %= NCARDS;
	index += 8840LL; index %= NCARDS;
	index *= 55LL; index %= NCARDS;
	index += 4613LL; index %= NCARDS;
	index *= 10LL; index %= NCARDS;
	index += 5301LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 21LL; index %= NCARDS;
	index += 5653LL; index %= NCARDS;
	index *= 2LL; index %= NCARDS;
	index += NCARDS - 5364LL; index %= NCARDS;
	index *= 72LL; index %= NCARDS;
	index += 3468LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += 9661LL; index %= NCARDS;
	index *= 63LL; index %= NCARDS;
	index += NCARDS - 6794LL; index %= NCARDS;
	index *= 43LL; index %= NCARDS;
	index += NCARDS - 2935LL; index %= NCARDS;
	index *= 66LL; index %= NCARDS;
	index += 1700LL; index %= NCARDS;
	index *= 6LL; index %= NCARDS;
	index += NCARDS - 5642LL; index %= NCARDS;
	index *= 64LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += 5699LL; index %= NCARDS;
	index *= 43LL; index %= NCARDS;
	index += 9366LL; index %= NCARDS;
	index *= 42LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += NCARDS - 2364LL; index %= NCARDS;
	index *= 13LL; index %= NCARDS;
	index += NCARDS - 8080LL; index %= NCARDS;
	index *= 2LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += 9602LL; index %= NCARDS;
	index *= 51LL; index %= NCARDS;
	index += NCARDS - 3214LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += 2995LL; index %= NCARDS;
	index *= 57LL; index %= NCARDS;
	index += 8169LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += NCARDS - 362LL; index %= NCARDS;
	index *= 41LL; index %= NCARDS;
	index += 4547LL; index %= NCARDS;
	index *= 56LL; index %= NCARDS;
	index += 1815LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += NCARDS - 1554LL; index %= NCARDS;
	index *= 71LL; index %= NCARDS;
	index += NCARDS - 2884LL; index %= NCARDS;
	index *= 44LL; index %= NCARDS;
	index += 2423LL; index %= NCARDS;
	index *= 4LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 20LL; index %= NCARDS;
	index += 2242LL; index %= NCARDS;
	index *= 48LL; index %= NCARDS;
	index += 716LL; index %= NCARDS;
	index *= 29LL; index %= NCARDS;
	index += 6751LL; index %= NCARDS;
	index *= 45LL; index %= NCARDS;
	index += 9511LL; index %= NCARDS;
	index *= 75LL; index %= NCARDS;
	index += 440LL; index %= NCARDS;
	index *= 35LL; index %= NCARDS;
	index += NCARDS - 6861LL; index %= NCARDS;
	index *= 52LL; index %= NCARDS;
	index += 4702LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 28LL; index %= NCARDS;
	index += NCARDS - 305LL; index %= NCARDS;
	index *= 16LL; index %= NCARDS;
	index += NCARDS - 7094LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index += NCARDS - 5175LL; index %= NCARDS;
	index *= 30LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 61LL; index %= NCARDS;
	index += NCARDS - 1831LL; index %= NCARDS;
	index = NCARDS - index - 1LL;
	index *= 25LL; index %= NCARDS;
	index += NCARDS - 4043LL; index %= NCARDS;
}

std::set<__int64> values;

void test3( __int64 repeats, __int64 target )
{
	printf( "Testing %s with %lld cards, %lld repeats\n", "input.txt", NCARDS, repeats );
//	values.clear();
	index = target;
//	values.insert( index );
	for ( __int64 i = repeats; i > 0LL; --i )
	{
		calculate();
// 		if ( values.count( index ) == 1 )
// 		{
// 			printf( "Repeated %lld at iteration %lld\n", index, i );
// 		}
// 		values.insert( index );
		// 		if ( index == target )
// 		{
// 			// This sequence has repeated, and will continue to repeat.
// 			const __int64 times = repeats - i + 1LL;
// 			printf( "After %lld times, %lld is back where it started\n", times, target );
// 			i %= times;
// 		}
	}
	printf( "%lld found in position %lld\n\n", target, index );
}

void test2( const char* filename, __int64 ncards, __int64 repeats, __int64 target )
{
	printf( "Testing %s with %lld cards, %lld repeats\n", filename, ncards, repeats );
	load( filename );

	index = target;
	for ( __int64 i = repeats; i > 0LL; --i )
	{
		for ( shuffle op : operations )
		{
			switch ( op.op )
			{
				case NEWSTACK:
					index = ncards - index - 1LL;
					break;
				case DEALINCREMENT:
					index *= op.amount;
					index %= ncards;
					break;
				case CUT:
					index += ncards - op.amount;
					index %= ncards;
					break;
			}
		
		}
		if ( index == target )
		{
			// This sequence has repeated, and will continue to repeat.
			const __int64 times = repeats - i + 1LL;
			printf( "After %lld times, %lld is back where it started\n", times, target );
			i %= times;
		}
	}
	printf( "%lld found in position %lld\n\n", target, index );
}

// C function for extended Euclidean Algorithm (used to 
// find modular inverse. 
__int64 gcdExtended( __int64 a, __int64 b, __int64 *x, __int64 *y )
{
	// Base Case 
	if ( a == 0 )
	{
		*x = 0, *y = 1;
		return b;
	}

	__int64 x1, y1; // To store results of recursive call 
	__int64 _gcd = gcdExtended( b%a, a, &x1, &y1 );

	// Update x and y using results of recursive 
	// call 
	*x = y1 - ( b / a ) * x1;
	*y = x1;

	return _gcd;
}

// Function to find modulo inverse of b. It returns 
// -1 when inverse doesn't 
__int64 modInverse( __int64 b, __int64 m )
{
	__int64 x, y; // used in extended GCD algorithm 
	__int64 g = gcdExtended( b, m, &x, &y );

	// Return -1 if b and m are not co-prime 
	if ( g != 1 )
		return -1;

	// m is added to handle negative x 
	return ( x%m + m ) % m;
}

// To compute (a * b) % mod 
__int64 mulmod( __int64 a, __int64 b, __int64 mod )
{
	__int64 res = 0; // Initialize result 
	a = a % mod;
	while ( b > 0 )
	{
		// If b is odd, add 'a' to result 
		if ( b % 2 == 1 )
			res = ( res + a ) % mod;

		// Multiply 'a' with 2 
		a = ( a * 2 ) % mod;

		// Divide b by 2 
		b /= 2;
	}

	// Return result 
	return res % mod;
}

// Function to compute a/b under modulo m 
__int64 modDivide( __int64 a, __int64 b, __int64 m )
{
	a = a % m;
	__int64 inv = modInverse( b, m );
	assert( inv != -1 );
	return mulmod( inv, a, m );
}

__int64 _ncards = 0;

void test4( const char* filename, __int64 ncards, __int64 repeats, __int64 index )
{
	printf( "Testing %s with %lld cards, %lld repeats\n", filename, ncards, repeats );
	load( filename );

	_ncards = ncards;
	// Convert the 100 operations in file into a single expression.
	struct commonop
	{
		commonop() : add( 0 ), mul( 1 ) {}
		__int64 add;
		__int64 mul;
		commonop& operator*=( commonop& other )
		{
			add = mulmod( add, other.mul, _ncards );
			mul = mulmod( mul, other.mul, _ncards );
			add += other.add;
			if ( add < 0 )
			{
				add += _ncards;
			}
			add %= _ncards;
			if ( mul < 0 )
			{
				mul += _ncards;
			}
			mul %= _ncards;
			return *this;
		}
	};
	commonop forwards;
	for ( shuffle op : operations )
	{
		commonop cop;
		switch ( op.op )
		{
			case NEWSTACK:
				cop.add = ncards - 1;
				cop.mul = ncards - 1;
				break;
			case DEALINCREMENT:
				cop.mul = op.amount;
				cop.add = 0;
				break;
			case CUT:
				cop.mul = 1;
				cop.add = (-op.amount);
				break;
		}
		forwards *= cop;
	}
	if ( repeats == 1 )
	{
		// part 1 -> index = start position, what's end position?
		__int64 part1 = ( index * forwards.mul + forwards.add ) % ncards;
		printf( "Part 1: %lld\n", part1 );
	}
	else
	{
		// part 2 -> index = end position, what's start position?
		// first, multiply forwards up to repeats, by squaring
		commonop combined;// = forwards.power( repeats );
		__int64 reps = repeats;
		commonop total;
		commonop thisbit = forwards;
		for ( int bit = 0; reps != 0 && bit < 64; ++bit )
		{
			if ( reps & ( 1LL << bit ) )
			{
				reps &= ~( 1LL << bit );
				total *= thisbit;
			}
			if ( reps )
			{
				commonop square = thisbit;
				square *= thisbit;
				thisbit = square;
			}
		}
		// then (hard), invert the combined operation
		__int64 x = (index + ncards - total.add) % ncards;
		// now, ({answer} * total.mul) % ncards = x;
		__int64 part2 = modDivide( x, total.mul, ncards );
		printf( "Part 2: %lld\n", part2 );
	}
}

int main()
{
// 	test( "example1.txt", 10, 10, 9 );
// 	test( "example2.txt", 10, 10, 9 );
// 	test( "example3.txt", 10, 10, 9 );
// 	test( "example4.txt", 10, 10, 9 );
//	test( "input.txt", 10007, 1, 2019 );
// 	test2( "example1.txt", 10, 10, 9 );
// 	test2( "example2.txt", 10, 10, 9 );
// 	test2( "example3.txt", 10, 10, 9 );
// 	test2( "example4.txt", 10, 10, 9 );
	test2( "input.txt", 10007, 1, 2019 );
	test3( 1, 2019 );
	test4( "input.txt", 10007, 1, 2019 );
	test4( "input.txt", 119315717514047LL, 101741582076661LL, 2020 );
//	test3( 101741582076661LL, 2020 );
	return 0;
}

// part 2: 13673334784545 too low.
//		   97362085572260 too high
//		   96959315590030 correct
