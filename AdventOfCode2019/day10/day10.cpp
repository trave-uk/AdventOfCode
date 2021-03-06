// day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31 };

void _switch( int &a, int &b )
{
	int t = a;
	a = b;
	b = t;
}

// greatest common divisor
int gcd( int a, int b )
{
	if ( !a )
		return b;
	if ( !b )
		return a;
	// Using the Euclidean algorithm https://en.wikipedia.org/wiki/Euclidean_algorithm
	if ( a > b )
	{
		_switch ( a, b );
	}

	int result = 1;
	do
	{
		result = a;
		while ( b >= a )
		{
			b -= a;
		}
		_switch ( a, b );
	} while ( a > 0 );
	return result;
}

const int limit = 50;
int main()
{
	assert( gcd( 10, 4 ) == 2 );
	assert( gcd( 100, 3 ) == 1 );
	assert( gcd( 252, 105 ) == 21 );
	char asteroids[limit][limit];;
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example1.txt", "rt" );
//	FILE *fp = fopen( "example2.txt", "rt" );
//	FILE *fp = fopen( "example3.txt", "rt" );
//	FILE *fp = fopen( "example4.txt", "rt" );
//	FILE *fp = fopen( "example5.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int row = 0, width = 0, height = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			for ( int col = 0; col < strlen( thisLine ); ++col )
			{
				char pt = thisLine[col];
				if ( pt != '.' && pt != '#' )
					break;
				if ( col >= width )
					width = col + 1;
				if ( row >= height )
					height = row + 1;
				assert( row < limit && col < limit );
				asteroids[row][col] = pt;
			}
			++row;
		}
	}
	fclose( fp );
	delete[] buffer;

	int mostseen = 0;
	int finalx = 0, finaly = 0;
	for ( row = 0; row < height; ++row )
	{
		for ( int col = 0; col < width; ++col )
		{
			if ( asteroids[row][col] != '#' )
				continue;

			int seen = 0;
			for ( int y = 0; y < height; ++y )
			{
				for ( int x = 0; x < height; ++x )
				{
					if ( asteroids[y][x] != '#' )
						continue;

					// Skip self
					if ( x == col && y == row )
						continue;

					int divisor = gcd( abs( x - col ), abs( y - row ) );
					int dx = (x - col) / divisor;
					int dy = (y - row) / divisor;
					int ax = col, ay = row;
					bool blocked = false;
					while ( ax != x || ay != y)
					{
						ax += dx;
						ay += dy;
						if ( ax == x && ay == y)
						{
							break;
						}
						if ( asteroids[ay][ax] == '#' )
						{
							blocked = true;
							break;
						}
					}
					if ( !blocked )
					{
						++seen;
					}
				}
			}
			if ( seen >= mostseen )
			{
				mostseen = seen;
				finalx = col;
				finaly = row;
				printf( "Most seen so far is %d, from (%d,%d)\n", seen, col, row );
			}
		}
	}

	printf( "Placing monitoring station at (%d,%d), seeing %d other asteroids\n", finalx, finaly, mostseen );

	class asteroid
	{
	public:
		float angle;
		int x;
		int y;
		float distance;

		bool operator<( const asteroid& other ) const
		{
			return angle < other.angle || ( angle == other.angle && distance < other.distance );
		}
	};
	std::set<float> angles;
	using roidmap = std::multimap<float, asteroid>;
	roidmap asteroidmap;
	static const float pi = static_cast<float>( M_PI );
	// Work out the angle of the vector between every other asteroid and the monitoring station
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			if ( y == finaly && x == finalx )
				continue;
			if ( asteroids[y][x] != '#' )
				continue;
			float dy = float( y ) - finaly;
			float dx = float( x ) - finalx;
			float angle = 180.0f * atan2f( dx, -dy ) / pi;
			if ( angle < 0.0f )
				angle += 360.0f;
			if ( angles.count( angle ) == 0 )
				angles.insert( angle );
			asteroid a;
			a.angle = angle;
			a.x = x;
			a.y = y;
			a.distance = sqrtf( dx*dx + dy * dy );
			asteroidmap.insert( std::make_pair( angle, a ) );
		}
	}
	// Now iterate through the angles
	int index = 1;
	int result = -1;
	while ( asteroidmap.size() > 0 )
	{
		for ( float angle : angles )
		{
			std::pair<roidmap::iterator, roidmap::iterator> it = asteroidmap.equal_range( angle );
			int count = 0;
			float nearest = 100000.0f;
			roidmap::iterator this1 = asteroidmap.end();
			for ( roidmap::iterator i = it.first; i != it.second; ++i )
			{
				asteroid &a = i->second;
				if ( a.distance < nearest )
				{
					nearest = a.distance;
					this1 = i;
				}
				++count;
			}
			if ( this1 != asteroidmap.end() )
			{
				asteroid &a = this1->second;
				printf( "%d: (%d,%d)\n", index, a.x, a.y );
				if ( index == 200 )
				{
					result = a.x * 100 + a.y;
				}
				++index;
				asteroidmap.erase( this1 );
			}
		}
	}
	printf( "Result 2: %d\n", result );
}

