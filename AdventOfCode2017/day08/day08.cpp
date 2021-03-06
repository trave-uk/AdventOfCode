// day08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

enum OP
{
	INC,
	DEC,
	OP_NOOF
};
const char* _op[] =
{
	"inc",
	"dec"
};

enum COMP
{
	NE, // !=
	LT, // <
	LE, // <=
	EQ, // ==
	GE, // >=
	GT, // >
	COMP_NOOF
};
const char* _comp[] =
{
	"!=",
	"<",
	"<=",
	"==",
	">=",
	">"
};

struct operation
{
	// <var1> <op> <value1> if <var2> <comparison> <value2>
	std::string var1;
	OP op = OP_NOOF;
	int value1 = 0;
	std::string var2;
	COMP comp = COMP_NOOF;
	int value2 = 0;
};

std::vector<operation> operations;

std::map<std::string, int> vars;

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
				// a inc 1 if b < 5
				// <var1> <op> <value1> if <var2> <comparison> <value2>
				operation entry;
				char var1[10];
				char op[10];
				char var2[10];
				char comp[10];
				sscanf( thisLine, "%s %s %d if %s %s %d", var1, op, &entry.value1, var2, comp, &entry.value2 );
				entry.var1 = std::string( var1 );
				entry.var2 = std::string( var2 );
				if ( vars.find( entry.var1 ) == vars.end() )
					vars.insert( std::make_pair( entry.var1, 0 ) );
				if ( vars.find( entry.var2 ) == vars.end() )
					vars.insert( std::make_pair( entry.var2, 0 ) );
				for ( int o = 0; o < _ARRAYSIZE_( _op ); ++o )
				{
					if ( strcmp( _op[o], op ) == 0 )
					{
						entry.op = (OP)o;
					}
				}
				assert( entry.op != OP_NOOF );
				for ( int c = 0; c < _ARRAYSIZE_( _comp ); ++c )
				{
					if ( strcmp( _comp[c], comp ) == 0 )
					{
						entry.comp = (COMP)c;
					}
				}
				assert( entry.comp != COMP_NOOF );
				operations.push_back( entry );
			}
		}
	}
	return true;
}

void process()
{
	std::string highest_ever_var;
	int highest_ever = INT_MIN;
	for ( operation op : operations )
	{
		int& var1 = vars[op.var1];
		int var2 = vars[op.var2];
		bool go = false;
		switch ( op.comp )
		{
			case NE:
				go = var2 != op.value2;
				break;
				
			case LT:
				go = var2 < op.value2;
				break;

			case LE:
				go = var2 <= op.value2;
				break;

			case EQ:
				go = var2 == op.value2;
				break;

			case GE:
				go = var2 >= op.value2;
				break;

			case GT:
				go = var2 > op.value2;
				break;

			default:
				assert( false );
		}
		if ( go )
		{
			switch ( op.op )
			{
				case INC:
					var1 += op.value1;
					break;
				case DEC:
					var1 -= op.value1;
					break;
				default:
					assert( false );
			}
			if ( var1 > highest_ever )
			{
				highest_ever = var1;
				highest_ever_var = op.var1;
			}
		}
	}
	std::string highest_var;
	int highest = INT_MIN;
	for ( auto var : vars )
	{
		if ( var.second > highest )
		{
			highest_var = var.first;
			highest = var.second;
		}
	}
	printf( "Highest variable is '%s', with value %d\n", highest_var.c_str(), highest );
	printf( "Highest ever variable was '%s', with value %d\n", highest_ever_var.c_str(), highest_ever );
}

int main()
{
	if ( load( "input.txt" ) )
//	if ( load( "example.txt" ) )
	{
		process();
	}
    return 0;
}

// a inc 1 if b < 5

