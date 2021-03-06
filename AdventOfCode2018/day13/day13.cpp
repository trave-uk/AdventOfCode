// day13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

char map[150][150];
int max_x = 0, max_y = 0;
struct cart_t
{
	int x = 0;
	int y = 0;
	int direction = 0;	// 0=up, 1=right, 2=down, 3=left
	int nextTurn = -1;	// +1 = turn right, 0 = straight on, -1 = turn left
	bool operator<( const cart_t& other ) const { return (y < other.y) || (y == other.y && x < other.x); }
};
std::set<cart_t> carts;

void print_map()
{
	for ( int y = 0; y <= max_y; ++y )
	{
		for ( int x = 0; x <= max_x; ++x )
		{
			char tile = map[y][x];
			for ( cart_t cart : carts )
			{
				if ( cart.x == x && cart.y == y )
				{
					switch ( cart.direction )
					{
						case 0:
							tile = '^';
							break;
						case 1:
							tile = '>';
							break;
						case 2:
							tile = 'v';
							break;
						case 3:
							tile = '<';
							break;
					}
				}
			}
			putchar( tile );
		}
		putchar( '\n' );
	}
	getchar();
}

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int y = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				for ( int x = 0; x < strlen( thisLine ); ++x )
				{
					assert( x < 150 && y < 150 );
					char tile = thisLine[x];
					int direction = -1;
					switch ( tile )
					{
						case '^':
							direction = 0;
							tile = '|';
							break;
						case '>':
							direction = 1;
							tile = '-';
							break;
						case 'v':
							direction = 2;
							tile = '|';
							break;
						case '<':
							direction = 3;
							tile = '-';
							break;
					}
					map[y][x] = tile;
					if ( x > max_x )
						max_x = x;
					if ( y > max_y )
						max_y = y;
					if ( direction != -1 )
					{
						cart_t cart;
						cart.x = x;
						cart.y = y;
						cart.direction = direction;
						carts.insert( cart );
					}
				}
				++y;
			}
		}
	}
	fclose( fp );
	delete[] buffer;
//	print_map();
	// Process all carts until there is a collision
	bool bCollision = false;
	while ( carts.size() > 1 )
	{
		std::set<cart_t> oldCarts = carts;
		std::set<cart_t> newCarts;
		for ( cart_t cart : carts )
		{
			if ( oldCarts.count( cart ) == 0 )
				continue;
			oldCarts.erase( cart );
			char track = map[cart.y][cart.x];
			char expected = '\0';
			switch ( cart.direction )
			{
				case 0:
					expected = '|';
					--cart.y;
					break;
				case 1:
					expected = '-';
					++cart.x;
					break;
				case 2:
					expected = '|';
					++cart.y;
					break;
				case 3:
					expected = '-';
					--cart.x;
					break;
			}
			assert( track == expected || track == '\\' || track == '/' || track == '+' );
			if ( oldCarts.count( cart ) > 0 || newCarts.count( cart ) > 0 )
			{
				bCollision = true;
				printf( "Collision found at (%d,%d)\n", cart.x, cart.y );
				oldCarts.erase( cart );
				newCarts.erase( cart );
				continue;
			}
			int turn = 0;
			switch ( map[cart.y][cart.x] )
			{
				case '+':
					// Crossroads
					turn = cart.nextTurn;
					++cart.nextTurn;
					if ( cart.nextTurn > 1 )
						cart.nextTurn = -1;
					break;
				case '/':
					if ( cart.direction == 0 || cart.direction == 2 )
						turn = 1;
					else
						turn = -1;
					break;
				case '\\':
					if ( cart.direction == 0 || cart.direction == 2 )
						turn = -1;
					else
						turn = 1;
					break;
				default:
					break;
			}
			cart.direction = ((cart.direction + turn) + 4) % 4;
			newCarts.insert( cart );
		}
		carts = newCarts;
//		print_map();
	}
	for ( auto cart : carts )
	{
		printf( "Remaining cart at (%d,%d)\n", cart.x, cart.y );
	}
	getchar();
	return 0;
}

