// day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using reg_t = std::array<INT64, 6>;
reg_t registers;

int ip = 0;

typedef void( *OpCode )(INT64, INT64, INT64);

struct command
{
	char op[5];
	OpCode fn = nullptr;
	INT64 A;
	INT64 B;
	INT64 C;
	void print()
	{
		printf( " %s %lld %lld %lld ", op, A, B, C );
	}
};

std::vector<command> program;

// opcodes
// addr (add register) stores into register C the result of adding register A and register B.
void addr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] + registers[B]);
}

// addi (add immediate) stores into register C the result of adding register A and value B.
void addi( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] + B);
}

// mulr (multiply register) stores into register C the result of multiplying register A and register B.
void mulr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] * registers[B]);
}

// muli (multiply immediate) stores into register C the result of multiplying register A and value B.
void muli( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] * B);
}

// banr (bitwise AND register) stores into register C the result of the bitwise AND of register A and register B.
void banr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] & registers[B]);
}

// bani (bitwise AND immediate) stores into register C the result of the bitwise AND of register A and value B.
void bani( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] & B);
}

// borr (bitwise OR register) stores into register C the result of the bitwise OR of register A and register B.
void borr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] | registers[B]);
}

// bori (bitwise OR immediate) stores into register C the result of the bitwise OR of register A and value B.
void bori( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] | B);
}

// setr (set register) copies the contents of register A into register C. (Input B is ignored.)
void setr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = registers[A];
}

// seti (set immediate) stores value A into register C. (Input B is ignored.)
void seti( INT64 A, INT64 B, INT64 C )
{
	registers[C] = A;
}

// gtir (greater-than immediate/register) sets register C to 1 if value A is greater than register B. Otherwise, register C is set to 0.
void gtir( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (A > registers[B]);
}

// gtri (greater-than register/immediate) sets register C to 1 if register A is greater than value B. Otherwise, register C is set to 0.
void gtri( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] > B);
}

// gtrr (greater-than register/register) sets register C to 1 if register A is greater than register B. Otherwise, register C is set to 0.
void gtrr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] > registers[B]);
}

// eqir (equal immediate/register) sets register C to 1 if value A is equal to register B. Otherwise, register C is set to 0.
void eqir( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (A == registers[B]);
}

// eqri (equal register/immediate) sets register C to 1 if register A is equal to value B. Otherwise, register C is set to 0.
void eqri( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] == B);
}

// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
void eqrr( INT64 A, INT64 B, INT64 C )
{
	registers[C] = (registers[A] == registers[B]);
}

struct lookup
{
	lookup( const char* _op, OpCode _fn ) :
		op( _op ),
		fn( _fn )
	{
	}
	const char *op;
	OpCode fn;
};

std::array<lookup, 16> operations =
{
	lookup( "bani", bani ),
	lookup( "gtri", gtri ),
	lookup( "seti", seti ),
	lookup( "eqir", eqir ),
	lookup( "eqrr", eqrr ),
	lookup( "borr", borr ),
	lookup( "bori", bori ),
	lookup( "banr", banr ),
	lookup( "muli", muli ),
	lookup( "eqri", eqri ),
	lookup( "mulr", mulr ),
	lookup( "gtrr", gtrr ),
	lookup( "setr", setr ),
	lookup( "addr", addr ),
	lookup( "gtir", gtir ),
	lookup( "addi", addi )
};

bool load( const char *filename )
{
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
				if ( strncmp( thisLine, "#ip", 3 ) == 0 )
				{
					sscanf( thisLine, "#ip %d", &ip );
				}
				else
				{
					command c;
					sscanf( thisLine, "%s %lld %lld %lld", c.op, &c.A, &c.B, &c.C );
					for ( lookup l : operations )
					{
						if ( strcmp( c.op, l.op ) == 0 )
						{
							c.fn = l.fn;
							break;
						}
					}
					assert( c.fn != nullptr );
					program.push_back( c );
				}
			}
		}
	}
	return true;
}


void execute( const command& c )
{
	c.fn( c.A, c.B, c.C );
}

void print_registers(reg_t registers)
{
	putchar( '[' );
	for ( int i = 0; i < registers.size(); ++i )
	{
		if ( i == registers.size() - 1 )
		{
			printf( "%lld", registers[i] );
		}
		else
			printf( "%lld, ", registers[i] );
	}
	putchar( ']' );
}

void process()
{
	while ( registers[ip] >= 0 && registers[ip] < static_cast<INT64>(program.size()) )
	{
#ifdef VERBOSE
		reg_t regcopy = registers;
#endif
		const command& com = program[registers[ip]];
		execute( com );
#ifdef VERBOSE
		print_registers(regcopy);
		com.print();
		print_registers(registers);
		putchar( '\n' );
#endif
		++registers[ip];
	}
	printf( "Register 0 contains %lld\n", registers[0] );
}

//int main()
//{
// //	if ( load( "example.txt" ) )
// 	if ( load( "input.txt" ) )
// 	{
// 		registers.fill( 0 );
// 		process();
// 		printf( "Press return to continue...\n" );
// 		getchar();
// 		registers.fill( 0 );
// 		registers[0] = 1;
// 		process();
// 		printf( "Press return to exit...\n" );
// 		getchar();
// 	}
//	return 0;
//}

int calculate(int r3)
{
	int r0 = 0;
	int root = (int)(sqrtf( float( r3 ) ));
	for ( int r1 = 1; r1 <= root; ++r1 ) 
	{
		if ( (r3 % r1) == 0 )
		{
			r0 += r1;
			r0 += r3 / r1;
		}
	}
	printf( "%d -> %d\n", r3, r0 );
	return r3;
}

int main()
{
	calculate( 1025 );
	calculate( 10551425 );
	getchar();
}
			// OK, fiendish puzzle this!
// To find the 2nd answer (with r0 = 1), I had to work out what the program was doing (factorizing a large number and adding the factors together), and then work out the answer offline.
// The code was extremely inefficient, as it needed to loop through 10551425 * 10551425 possibilities to find 6 actual factors, especially as it was interpreted.

// Factors of 1025 are: 1, 5, 25, 41, 205, 1025
// Summed, this results in 1302

// Factors of 10551425 are: 1, 5, 25, 422057, 2110285 and 10551425.
// Summed, this results in 13083798

// #ip 5
//					int main()
//					{
//					int r0 = 1, r1, r2, r3;
// addi 5 16 5		init();	// r3 = 10551425; r0 = 0
// seti 1 3 1		for ( r1 = 1; r1 <= r3; ++r1) {
// seti 1 1 2			for ( r2 = 1; r2 <= r3; ++r2 ) {
// mulr 1 2 4
// eqrr 4 3 4				if ( r3 == r1*r2 ) r0 += r1;
// addr 4 5 5
// addi 5 1 5
// addr 1 0 0
// addi 2 1 2
// gtrr 2 3 4			}
// addr 5 4 5
// seti 2 4 5
// addi 1 1 1
// gtrr 1 3 4		}
// addr 4 5 5		
// seti 1 5 5
// mulr 5 5 5		return r0; }
// addi 3 2 3		void init()
// mulr 3 3 3		{
// mulr 5 3 3
// muli 3 11 3
// addi 4 8 4
// mulr 4 5 4
// addi 4 13 4
// addr 3 4 3			r3 = 1025;
// addr 5 0 5			if ( r0 == 1 )
// seti 0 8 5			{
// setr 5 3 4
// mulr 4 5 4
// addr 5 4 4
// mulr 5 4 4
// muli 4 14 4
// mulr 4 5 4
// addr 3 4 3				r3 = 10551425;
// seti 0 8 0				r0 = 0;
// seti 0 4 5			}
//					}
