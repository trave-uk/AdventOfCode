// day16.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::array<int, 4> registers;

// opcodes
// addr (add register) stores into register C the result of adding register A and register B.
void addr( int A, int B, int C )
{
	registers[C] = (registers[A] + registers[B]);
}

// addi (add immediate) stores into register C the result of adding register A and value B.
void addi( int A, int B, int C )
{
	registers[C] = (registers[A] + B);
}

// mulr (multiply register) stores into register C the result of multiplying register A and register B.
void mulr( int A, int B, int C )
{
	registers[C] = (registers[A] * registers[B]);
}

// muli (multiply immediate) stores into register C the result of multiplying register A and value B.
void muli( int A, int B, int C )
{
	registers[C] = (registers[A] * B);
}

// banr (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
void banr( int A, int B, int C )
{
	registers[C] = (registers[A] & registers[B]);
}

// bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
void bani( int A, int B, int C )
{
	registers[C] = (registers[A] & B);
}

// borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
void borr( int A, int B, int C )
{
	registers[C] = (registers[A] | registers[B]);
}

// bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
void bori( int A, int B, int C )
{
	registers[C] = (registers[A] | B);
}

// setr (set register) copies the contents of register A into register C. (Input B is ignored.)
void setr( int A, int B, int C )
{
	registers[C] = registers[A];
}

// seti (set immediate) stores value A into register C. (Input B is ignored.)
void seti( int A, int B, int C )
{
	registers[C] = A;
}

// gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
void gtir( int A, int B, int C )
{
	registers[C] = (A > registers[B]);
}

// gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
void gtri( int A, int B, int C )
{
	registers[C] = (registers[A] > B);
}

// gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
void gtrr( int A, int B, int C )
{
	registers[C] = (registers[A] > registers[B]);
}

// eqir (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
void eqir( int A, int B, int C )
{
	registers[C] = (A == registers[B]);
}

// eqri (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
void eqri( int A, int B, int C )
{
	registers[C] = (registers[A] == B);
}

// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
void eqrr( int A, int B, int C )
{
	registers[C] = (registers[A] == registers[B]);
}

typedef void( *OpCode )(int, int, int);
std::array<OpCode, 16> operations = {
	bani,
	gtri,
	seti,
	eqir,
	eqrr,
	borr,
	bori,
	banr,
	muli,
	eqri,
	mulr,
	gtrr,
	setr,
	addr,
	gtir,
	addi
};

std::map<int, int> opmap;
bool useMap = false;

void operate( int opcode, int A, int B, int C )
{
	if ( useMap )
		opcode = opmap[opcode];
	operations[opcode]( A, B, C );
}

std::vector<int> CountOpcodes( std::array<int, 4> before, std::array<int, 4> command, std::array<int, 4> after )
{
	std::vector<int> ops;
	for ( int op = 0; op < 16; ++op )
	{
		registers = before;
		operate( op, command[1], command[2], command[3] );
		if ( registers == after )
		{
			ops.push_back( op );
		}
	}
	return ops;
}

bool process( const char *filename )
{
	std::array<int, 4> before;
	std::array<int, 4> command;
	std::array<int, 4> after;
	bool started = false;
	int count = 0;
	int total = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				if ( strstr( thisLine, "Before" ) != nullptr )
				{
					sscanf( thisLine, "Before: [%d, %d, %d, %d]", &before[0], &before[1], &before[2], &before[3] );
					started = true;
				}
				else if ( strstr( thisLine, "After" ) != nullptr )
				{
					sscanf( thisLine, "After: [%d, %d, %d, %d]", &after[0], &after[1], &after[2], &after[3] );
					started = false;
					// Run a test
					++total;
					std::vector<int> ops = CountOpcodes( before, command, after );
					registers.fill( 0 );
					if ( ops.size() >= 3 )
					{
						++count;
					}
 					if ( opmap.count( command[0] ) > 0 )
 					{
						// This opcode already mapped. Check the mapping matches this instance!
 						assert( std::find( ops.begin(), ops.end(), opmap[command[0]] ) != ops.end() );
 						continue;
 					}
					std::vector<int> prunedOps;
					for ( auto op : ops )
					{
						bool found = false;
						for ( auto opmapped : opmap )
						{
							if ( opmapped.second == op )
							{
								found = true;
							}
						}
						if ( !found )
							prunedOps.push_back( op );
					}
					if ( prunedOps.size() == 1 )
					{
						opmap.insert( std::pair<int, int>(command[0], prunedOps[0]) );
						printf( " %d => %d\n", command[0], prunedOps[0] );
					}
				}
				else if ( started )	// between Before and After
				{
					sscanf( thisLine, "%d %d %d %d", &command[0], &command[1], &command[2], &command[3] );
				}
				else
				{
					// Finished test data, so execute command
					useMap = true;
					sscanf( thisLine, "%d %d %d %d", &command[0], &command[1], &command[2], &command[3] );
					operate( command[0], command[1], command[2], command[3] );
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	printf( "%d / %d samples matched 3 or more opcodes\n", count, total );
	printf( "After running commands, register 0 contains %d\n", registers[0] );

	return true;
}

int main()
{
//	process( "example.txt" );
	process( "input.txt" );
	getchar();
    return 0;
}

// 560 / 811 samples matched 3 or more opcodes
// After running commands, register 0 contains 622
