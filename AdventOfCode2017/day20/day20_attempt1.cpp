// day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::string input;

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
				input = thisLine;
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

struct mappednode;

struct node
{
	int index = 0;
	node *north = nullptr;
	node *west = nullptr;
	node *east = nullptr;
	node *south = nullptr;
	mappednode* mapped;
};

struct mappednode
{
	int index;
	int x;
	int y;
	bool operator<( const mappednode& other ) const
	{
		return (y < other.y || (y == other.y && x < other.x));
	}
};

std::vector<node> nodes;
std::stack<node*> nodestack;
std::set<mappednode> nodemap;

node* allocnode()
{
	node n;
	n.index = nodes.size();
	nodes.push_back( n );
	return &nodes.back();
}

void mapnode( node* n, int x, int y )
{
	mappednode m;
	m.index = n->index;
	m.x = x;
	m.y = y;
	nodemap.insert( m );
}
void process()
{
	// Process the regex. First, remove initial ^ and trailing $
	assert( input.front() == '^' && input.back() == '$' );
	input.erase( input.begin() );
	input.pop_back();

	// The remaining regex consists of options in (possibly nested) brackets.
	int depth = 0;
	node* parent = nullptr;
	node* current = allocnode();
	mapnode( current, 0, 0 );
	mappednode m;
	m.index = current->index;
	int x = 0;
	int y = 0;
	for ( char c : input )
	{
		switch ( c )
		{
			case 'N':
			{
				mappednode m;
				//m.
				if ( !current->north )
				{
					current->north = allocnode();
					current->north->south = current;
					current->north->x = current->x;
					current->north->y = current->y - 1;
				}
				current = current->north;
				break;
			}
			case 'E':
			{
				if ( !current->east )
				{
					current->east = allocnode();
					current->east->west = current;
					current->east->x = current->x + 1;
					current->east->y = current->y;
				}
				current = current->east;
				break;
			}
			case 'W':
			{
				if ( !current->west )
				{
					current->west = allocnode();
					current->west->east = current;
					current->west->x = current->x - 1;
					current->west->y = current->y;
				}
				current = current->west;
				break;
			}
			case 'S':
			{
				if ( !current->south )
				{
					current->south = allocnode();
					current->south->north = current;
					current->south->x = current->x;
					current->south->y = current->y + 1;
				}
				current = current->south;
				current->distance++;
				break;
			}
			case '(':
			{
				nodestack.push( current );
				depth++;
				break;
			}
			case '|':
			{
				current = nodestack.top();
				break;
			}
			case ')':
			{
				depth--;
				break;
			}
		}
	}
}

int main()
{
//	if ( load( "example.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		printf( "Press return to continue...\n" );
		getchar();
	}
	return 0;
}

