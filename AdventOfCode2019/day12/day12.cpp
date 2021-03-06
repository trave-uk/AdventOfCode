// day12.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using i64 = long long;

struct body
{
	union
	{
		int pos[3];
		struct  
		{
			int x, y, z;	// position
		};
	};
	union
	{
		int vel[3];
		struct
		{
			int dx, dy, dz;	// velocity
		};
	};
};

#define _ARRAYSIZE_(x) (sizeof(x)/sizeof(*x))

body bodies[4] = {};

void _switch( i64 &a, i64 &b )
{
	i64 t = a;
	a = b;
	b = t;
}

// greatest common divisor
i64 gcd( i64 a, i64 b )
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

	i64 result = 1;
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

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" ); const int steps = 1000, report = 100;
//	FILE *fp = fopen( "example1.txt", "rt" ); const int steps = 10, report = 1;
//	FILE *fp = fopen( "example2.txt", "rt" ); const int steps = 100, report = 10;
	int i = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( strlen( thisLine ) > 0 )
			{
				// "<x=0, y=0, z=0>"
				assert( i < _ARRAYSIZE_( bodies ) );
				body& b = bodies[i];
				sscanf( thisLine, "<x=%d, y=%d, z=%d>", &b.x, &b.y, &b.z );
				++i;
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// copy original state
	body copy[4] = {};
	for ( i = 0; i < _ARRAYSIZE_( bodies ); ++i )
	{
		copy[i] = bodies[i];
	}

	i64 found[3] = {};
	int axes = 0;
	i64 step = 0;
	while (true) //(step <= steps)
	{
// 		// report
// 		if ( step % report == 0 )
// 		{
// 			printf( "After %lld steps:\n", step );
// 			for ( i = 0; i < ARRAYSIZE( bodies ); ++i )
// 			{
// 				body& b = bodies[i];
// 				printf( "pos=<x=% 3d, y=% 3d, z=% 3d>, vel=<x=% 3d, y=% 3d, z=% 3d>\n", b.x, b.y, b.z, b.dx, b.dy, b.dz );
// 			}
// 			printf( "\n" );
// 		}
// 
// 		if ( step == steps )
// 			break;

		// process each pair of bodies
		for ( i = 0; i < _ARRAYSIZE_( bodies ); ++i )
		{
			body& b1 = bodies[i];
			for ( int j = i+1; j < _ARRAYSIZE_( bodies ); ++j )
			{
				body& b2 = bodies[j];
				for ( int axis = 0; axis < 3; ++axis )
				{
					int d = 0;
					// modify velocity +-1 towards other body in this axis
					if ( b1.pos[axis] < b2.pos[axis] )
					{
						d = 1;
					}
					else if ( b1.pos[axis] > b2.pos[axis] )
					{
						d = -1;
					}
					b1.vel[axis] += d;
					b2.vel[axis] -= d;
				}
			}
		}

		// apply velocity
		for ( i = 0; i < _ARRAYSIZE_( bodies ); ++i )
		{
			body& b = bodies[i];
			for ( int axis = 0; axis < 3; ++axis )
			{
				b.pos[axis] += b.vel[axis];
			}
		}

		++step;

		for ( int axis = 0; axis < 3; ++axis )
		{
			if ( found[axis] > 0 )
				continue;

			bool same = true;
			for ( i = 0; i < _ARRAYSIZE_( bodies ); ++i )
			{
				if ( bodies[i].pos[axis] != copy[i].pos[axis] || bodies[i].vel[axis] != copy[i].vel[axis] )
				{
					same = false;
					break;
				}
			}
			if ( same )
			{
				found[axis] = step;
				printf( "Axis %d repeated after %lld steps\n", axis, found[axis] );
				++axes;
				if ( axes == _ARRAYSIZE_( found ) )
				{
					printf( "All axes found!\n" );
					i64 gcd1 = gcd( found[0], found[1] );
					i64 total = ( found[0] / gcd1 ) * found[1];
					i64 gcd2 = gcd( total, found[2] );
					total *= found[2] / gcd2;
					printf( "Will repeat after %lld steps\n", total );
				}
			}
		}
		if ( memcmp( bodies, copy, sizeof( bodies ) ) == 0 )
		{
			printf( "Repeated after %lld steps\n", step );
			break;
		}
	}

	// Report energy:

	// Energy after 100 steps:
	// pot:  8 + 12 +  9 = 29;   kin: 7 +  3 + 0 = 10;   total: 29 * 10 = 290
	// pot: 13 + 16 +  3 = 32;   kin: 3 + 11 + 5 = 19;   total: 32 * 19 = 608
	// pot: 29 + 11 +  1 = 41;   kin: 3 +  7 + 4 = 14;   total: 41 * 14 = 574
	// pot: 16 + 13 + 23 = 52;   kin: 7 +  1 + 1 =  9;   total: 52 *  9 = 468
	// Sum of total energy: 290 + 608 + 574 + 468 = 1940
	printf( "Energy after %lld steps:\n", step );
	int sum = 0;
	for ( i = 0; i < _ARRAYSIZE_( bodies ); ++i )
	{
		body& b = bodies[i];
		int pot = abs( b.x ) + abs( b.y ) + abs( b.z );
		int kin = abs( b.dx ) + abs( b.dy ) + abs( b.dz );
		int total = pot * kin;
		printf( "pot: % 2d + % 2d + % 2d =% 3d;   kin:  % 2d + % 2d + % 2d =% 3d;   total:% 3d *% 3d = %d\n", abs( b.x ), abs( b.y ), abs( b.z ), pot, abs( b.dx ), abs( b.dy ), abs( b.dz ), kin, pot, kin, total );
		sum += total;
	}
	printf( "Sum of total energy: %d\n", sum );
	return 0;
}

