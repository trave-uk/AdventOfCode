// day13.cpp : Defines the entry point for the console application.
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

cpu CPU;
std::deque<int> balldir;
struct _score
{
	int amount;
	int x;
	int y;
	bool operator<( const _score other ) const
	{
		return y < other.y || ( y == other.y && x < other.x );
//		return x < other.x || ( x == other.x && y < other.y );
	}
};
std::set<_score> scores;
std::deque<int> outputs;

int main()
{
	CPU.init();
	cpu::result finished = CPU.process( outputs );

	int width = 0;
	int height = 0;
	while ( outputs.size() > 3)
	{
		int x = outputs.front();
		outputs.pop_front();
		int y = outputs.front();
		outputs.pop_front();
		int tile = outputs.front();
		outputs.pop_front();
		if ( x >= width )
			width = x + 1;
		if ( y >= height )
			height = y + 1;
		assert( x >= 0 && y >= 0 );
	}
	int* map = (int*)calloc( height + 1, width * sizeof( int ) );

// 	while ( !_kbhit() )
// 	{
// 	}
// 	_getch();

	do
	{
		std::deque<int> paddledir = balldir;
		// First two moves get in line with ball
		if ( paddledir.size() >= 2 )
		{
			paddledir.pop_front();
			paddledir.pop_front();
		}
		paddledir.push_front( 0 ); // 2: stay still
		paddledir.push_front( -1 ); // 1: move left
		balldir.clear();

		// reinit
		CPU.init();
		// Infinite quarters
		CPU.poke( 0, 2 );

		scores.clear();
		finished = cpu::MAXOUTPUT;
		int score = 0;
		int paddle = 0;
		int py = 0;
		int ball = 0;
		int by = 0;
		int aim = 0;
		int sx = 0, sy = 0;
		while ( finished != cpu::FINISHED)
		{
			finished = CPU.process( outputs, 3 );
			while ( outputs.size() >= 3 )
			{
				int x = outputs.front();
				outputs.pop_front();
				int y = outputs.front();
				outputs.pop_front();
				int tile = outputs.front();
				outputs.pop_front();
				int index = y * width + x;
				if ( x == -1 )
				{
					if ( tile > 0 )
					{
						assert( abs( ball - sx ) <= 1 && abs( by - sy ) <= 1 && ( ball != sx || by != sy ) );
						_score s;
						s.x = sx;
						s.y = sy;
						s.amount = tile - score;
						scores.insert( s );
					}
					score = tile;
					gotoxy( 0, height );
					printf( "%d", score );
				}
				else
				{
					gotoxy( x, y );
					switch ( tile )
					{
						case 0:
							printf( " " );
							sx = x; sy = y;
							break;
						case 1:
							printf( "#" );
							break;
						case 2:
							printf( "@" );
							break;
						case 3:
							paddle = x;
							py = y;
							printf( "-" );
							break;
						case 4:
						{
							int dir = ball ? ( x - ball ) : 0;
							balldir.push_back( dir );
							ball = x;
							by = y;
							aim = ball + dir;
							printf( "o" );
							break;
						}
					}
				}
				if ( index < _ARRAYSIZE_( map ) )
				{
					map[index] = tile;
				}
			}
			if ( by != 0 && py != 0 && by >= py )
			{
				finished = cpu::FINISHED;
			}
			fflush( stdout );;
			if ( finished == cpu::NEEDINPUT)
			{
				if ( paddledir.size() > 0 )
				{
					int dir = paddledir.front();
					paddledir.pop_front();
					CPU.push_input( dir );
				}
				else
				{
					// 			while ( !_kbhit() )
					// 			{
					// 			}
					// 			_getch();
					int height = py - by;
					int input = 0;
					if ( height > 2 && paddle >= width - 3 )
						input = ( -1 );
					else if ( height > 2 && paddle <= 2 )
						input = ( 1 );
					else
					{
						int target = ( height < 4 ) ? aim : ball;
						if ( paddle < target )
							input = ( 1 );
						else if ( paddle > target )
							input = ( -1 );
						else
							input = ( 0 );
					}
					CPU.push_input( input );
				}
			}
		}
	} while ( true );
	printf( "\n\n\n\n\n\n" );
	free( map );
	return 0;
}

