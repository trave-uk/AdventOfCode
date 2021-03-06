// day21.cpp : Defines the entry point for the console application.
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

std::set<INT64> exits;
INT64 last = 0;

// eqrr (equal register/register) sets register C to 1 if register A is equal to register B. Otherwise, register C is set to 0.
void eqrr( INT64 A, INT64 B, INT64 C )
{
	if ( exits.count( registers[A] ) > 0 )
		printf( "%lld\n", last );
	else
	{
		last = registers[A];
		exits.insert( last );
	}
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

void print_registers( reg_t registers )
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
		print_registers( regcopy );
		com.print();
		print_registers( registers );
		putchar( '\n' );
#endif
		++registers[ip];
	}
	printf( "Register 0 contains %lld\n", registers[0] );
}

int main()
{
//	if ( load( "example.txt" ) )
	if ( load( "input.txt" ) )
	{
		registers.fill( 0 );
		process();
		printf( "Press return to continue...\n" );
		getchar();
		registers.fill( 0 );
		registers[0] = 1;
		process();
		printf( "Press return to exit...\n" );
		getchar();
	}
	return 0;
}

/*

#ip 2
0	seti 123 0 4		while ( (456 & 123) != 72 )		//int r4 = 123;
1	bani 4 456 4										//r4 &= 456;
2	eqri 4 72 4											//r4 = r4 == 72;
3	addr 4 2 2											//*r2 += r4
4	seti 0 0 2											//goto 1
5	seti 0 7 4			int r4 = 0;
6	bori 4 65536 3		int r3 = r4 | 65536
7	seti 10283511 1 4	r4 = 10283511

8	bani 3 255 1		r1 = r3 & 255
9	addr 4 1 4			r4 += r1
10	bani 4 16777215 4	r4 &= 16777215
11	muli 4 65899 4		r4 *= 65899
12	bani 4 16777215 4	r4 &= 16777215
13	gtir 256 3 1		if (256 > r3)								//r1 = (256 > r3)
14	addr 1 2 2											//*r2 += r1
15	addi 2 1 2											//goto 17
16	seti 27 8 2				goto 28
17	seti 0 1 1			for ( r1 = 0; ; ++r1 )
18	addi 1 1 5			{								r5 = r1 + 1
19	muli 5 256 5										r5 *= 256
20	gtrr 5 3 5				if ( (r1+1)*256 > r3 )		r5 = (r5 > r3)
21	addr 5 2 2					break;					*r2 += r5
22	addi 2 1 2											goto 24
23	seti 25 3 2											goto 26
24	addi 1 1 1											++r1
25	seti 17 0 2			}								goto 18
26	setr 1 4 3			r3 = r1
27	seti 7 6 2			goto 8

28	eqrr 4 0 1			if ( r4 == r0 )
29	addr 1 2 2				goto 31
30	seti 5 2 2			goto 6

*/