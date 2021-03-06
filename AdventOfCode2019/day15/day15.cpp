// day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#define _ARRAYSIZE_(x) (sizeof(x)/sizeof(*x))

using Value = __int64;
Value one( 1 );
Value zero( 0 );
Value hundred( 100 );

using ValueP = __int64;
class cpu
{
public:
	enum result { FINISHED, NEEDINPUT, MAXOUTPUT };
	void init();
	void poke( size_t address, Value value );
	Value& getValue( Value input, int addressModes, int parameterIndex );
	void push_input( Value input ) { inputs.push_back( input ); }
	result process( std::deque<int>& outputs, int maxout = 0 );
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
	//	FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	//	FILE *fp = fopen( "..\\day07\\input.txt", "rt" );
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

cpu::result cpu::process( std::deque<int>& outputs, int maxout )
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
					return NEEDINPUT;
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
					return MAXOUTPUT;
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
	return FINISHED;
}


void gotoxy( int x, int y )
{
	static HANDLE h = NULL;
	if ( !h )
		h = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD c = { x, y };
	SetConsoleCursorPosition( h, c );
}

enum class DIR
{
	NORTH = 1,
	SOUTH = 2,
	WEST = 3,
	EAST = 4
};

enum class STATUS
{
	STOPPED_WALL = 0,
	MOVED_SPACE = 1,
	MOVED_TARGET = 2
};

enum class TILE : char
{
	UNKNOWN = 0,
	SPACE = 1,
	WALL = 2,
	TARGET = 3,
	START = 4
};
cpu CPU;
std::deque<int> outputs;

const int _SIZE = 42;
TILE map[_SIZE][_SIZE];
int search[_SIZE][_SIZE];
const int _START = _SIZE / 2;
int min_x = _START;
int max_x = _START;
int min_y = _START;
int max_y = _START;

void print( int bx, int by, DIR dir, bool force = false )
{
	static int last_min_y = -1;
	static int last_min_x = -1;
	int y1 = min_y;
	int y2 = max_y;
	int x1 = min_x;
	int x2 = max_x;
	if ( !force && last_min_x == min_x && last_min_y == min_y )
	{
		y1 = max( min_y, by - 1 );
		y2 = min( max_y, by + 1 );
		x1 = max( min_x, bx - 1 );
		x2 = min( max_x, bx + 1 );
	}
	last_min_x = min_x;
	last_min_y = min_y;
	for ( int y = y1; y <= y2; ++y )
	{
		gotoxy( 0, y - min_y );
		for ( int x = x1; x <= x2; ++x )
		{
//			gotoxy( x - min_x, y - min_y );
			switch ( map[y][x] )
			{
				case TILE::UNKNOWN:
					printf( " " );
					break;
				case TILE::SPACE:
					if ( search[y][x] != -1 )
					{
						printf( "x" );
					}
					else if ( bx == x && by == y )
					{
						switch ( dir )
						{
							case DIR::NORTH:
								printf( "^" );
								break;
							case DIR::SOUTH:
								printf( "v" );
								break;
							case DIR::WEST:
								printf( "<" );
								break;
							case DIR::EAST:
								printf( ">" );
								break;
						}
					}
					else
					{
						printf( "." );
					}
					break;
				case TILE::WALL:
					printf( "#" );
					break;
				case TILE::TARGET:
					printf( "@" );
					break;
				case TILE::START:
					printf( "O" );
					break;
			}
		}
	}
}

void move( DIR dir, int &x, int &y )
{
	switch ( dir )
	{
		case DIR::NORTH:
			y -= 1;
			assert( y >= 0 );
			if ( y < min_y )
				min_y = y;
			break;
		case DIR::SOUTH:
			y += 1;
			assert( y < _SIZE );
			if ( y > max_y )
				max_y = y;
			break;
		case DIR::WEST:
			x -= 1;
			assert( x >= 0 );
			if ( x < min_x )
				min_x = x;
			break;
		case DIR::EAST:
			x += 1;
			assert( x < _SIZE );
			if ( x > max_x )
				max_x = x;
			break;
	}
}

void turn( bool bRight, DIR& dir )
{
	switch ( dir )
	{
		case DIR::NORTH:
			if ( bRight )
				dir = DIR::EAST;
			else
				dir = DIR::WEST;
			break;
		case DIR::SOUTH:
			if ( bRight )
				dir = DIR::WEST;
			else
				dir = DIR::EAST;
			break;
		case DIR::WEST:
			if ( bRight )
				dir = DIR::NORTH;
			else
				dir = DIR::SOUTH;
			break;
		case DIR::EAST:
			if ( bRight )
				dir = DIR::SOUTH;
			else
				dir = DIR::NORTH;
			break;
	}
}

void floodfill( int x, int y, int distance, int part = 1 )
{
	static int furthest = 0;
	if ( map[y][x] == TILE::WALL )
		return;
	if ( search[y][x] == -1 || search[y][x] > distance )
	{
		if ( distance > furthest && part == 2 )
		{
			gotoxy( 0, max_y - min_y + 3 );
			printf( "Furthest distance: %d\n", distance );
			furthest = distance;
		}
		search[y][x] = distance;
		print( x, y, DIR::NORTH, true );
		if ( map[y][x] == TILE::TARGET && part == 1)
		{
			gotoxy( 0, max_y - min_y + 2 );
			printf( "Closest path: %d steps    \n", distance );
		}
		int d = distance + 1;
		floodfill( x - 1, y, d, part );
		floodfill( x + 1, y, d, part );
		floodfill( x, y - 1, d, part );
		floodfill( x, y + 1, d, part );
	}
}

// 1: find shortest distance to the target from the start
//
// So, first, find the target, by moving around.
// (stage 1) Move north to find a wall, then:
//
// repeat:
//  (stage 2) turn right / try move forwards (repeat until success)
//  (stage 3) turn left / try move forwards (repeat until find wall)
//
// (until target found)

int main()
{
	CPU.init();

	memset( map, int( TILE::UNKNOWN ), sizeof( map ) );
	memset( search, -1, sizeof( search ) );
	DIR dir = DIR::NORTH;
	int x = _START;
	int y = _START;
	int tx = -1;
	int ty = -1;
	// assume start in a space
	map[y][x] = TILE::START;
	STATUS status = STATUS::MOVED_SPACE;
	int stage = 1;
	do 
	{
		print( x, y, dir );
		CPU.push_input( int(dir) );
		cpu::result finished = CPU.process( outputs );
		status = STATUS( outputs.front() );
		outputs.pop_front();
		// (stage 1) Move north to find a wall, then:
		//
		// repeat:
		//  (stage 2) turn right / try move forwards (repeat until success)
		//  (stage 3) turn left / try move forwards (repeat until find wall)
		//
		// (until target found)if
		switch ( status )
		{
			case STATUS::MOVED_TARGET:
			case STATUS::MOVED_SPACE:
				move( dir, x, y );

				if ( status == STATUS::MOVED_TARGET )
				{
					tx = x;
					ty = y;
					map[y][x] = TILE::TARGET;
				}
				else if ( map[y][x] == TILE::UNKNOWN )
				{
					map[y][x] = TILE::SPACE;
				}

				// React appropriately for the stage
				switch ( stage )
				{
					case 1:
						break;
					case 2:
						turn( false, dir );
						stage = 3;
						break;
					case 3:
						turn( false, dir );
						break;
				}
				break;
			case STATUS::STOPPED_WALL:
			{
				int wx = x, wy = y;
				move( dir, wx, wy );
				map[wy][wx] = TILE::WALL;
				// React appropriately for the stage
				switch ( stage )
				{
					case 1:
						stage = 2;
						turn( true, dir );
						break;
					case 2:
						turn( true, dir );
						break;
					case 3:
						turn( true, dir );
						stage = 2;
						break;
				}
				break;
			}
		}
	} while ( (max_y - min_y) < 10 || x != _START || y != _START || dir != DIR::NORTH );
	print( x, y, dir, true );

	// Having built the entire map, floodfill search from start.
	int width = max_x - min_x + 1;
	int height = max_y - min_y + 1;
	printf( "\nWidth: %d, height: %d\n", width, height );
	memset( search, -1, sizeof( search ) );
	x = _START;
	y = _START;
	floodfill( x, y, 0 );

	memset( search, -1, sizeof( search ) );
	print( x, y, dir, true );
	x = tx;
	y = ty;
	floodfill( x, y, 0, 2 );
}
	