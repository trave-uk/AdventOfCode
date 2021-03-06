// day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct node_t
{
	std::string name;
	int weight = 0;
	int total_weight = 0;
	std::string child;
	std::string next;
};

std::map<std::string, node_t> nodes;
std::set<std::string> children;

node_t& find_or_insert( std::string name )
{
	auto found = nodes.find( name );
	if ( found == nodes.end() )
	{
		node_t node;
		node.name = name;
		nodes.insert( std::make_pair( name, node ) );
		found = nodes.find( name );
	}
	return found->second;
}

bool load( const char *filename )
{
	nodes.clear();
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
				// ktlj (57)
				// fwft (72) -> ktlj, cntj, xhth
				char* name_ = strtok( thisLine, " ()" );
				std::string name( name_ );
				node_t& node = find_or_insert( name );
				char* value = strtok( nullptr, " (" );
				node.weight = atoi( value );
				char* remainder = strtok( nullptr, ") " );
				if ( remainder )
				{
					char *childname = strtok( nullptr, "-> ," );
					node_t* pnode = nullptr;
					while ( childname )
					{
						std::string cname( childname );
						children.insert( cname );
						node_t& childnode = find_or_insert( cname );
						if ( pnode )
						{
							pnode->next = cname;
						}
						else
						{
							node.child = cname;
						}
						pnode = &childnode;
						childname = strtok( nullptr, ", " );
					}
				}
			}
		}
	}
	return true;
}

void process()
{
	std::string root;
	for ( auto node : nodes )
	{
		if ( children.count( node.first ) == 0 )
		{
			root = node.first;
			printf( "%s has no parent\n", node.first.c_str() );
		}
	}

	int remaining = 0;
	do
	{
		remaining = 0;
		for ( auto& n : nodes )
		{
			node_t& node = n.second;
			if ( node.total_weight > 0 )
				continue;
			bool complete = true;
			std::string child = node.child;
			int total = node.weight;
			bool same = true;
			int childweight = 0;
			while ( child.length() > 0 )
			{
				node_t& cnode = nodes[child];
				if ( cnode.total_weight == 0 )
				{
					complete = false;
					break;
				}
				if ( childweight != 0 && childweight != cnode.total_weight )
				{
					same = false;
				}
				childweight = cnode.total_weight;
				total += cnode.total_weight;
				child = cnode.next;
			}
			if ( complete )
			{
				node.total_weight = total;
				if ( !same )
				{
					// found the root of the problem child
					printf( "node %s is unbalanced\n", node.name.c_str() );
					child = node.child;
					while ( child.length() > 0 )
					{
						node_t& cnode = nodes[child];
						printf( "%s -> %d (%d)\n", cnode.name.c_str(), cnode.total_weight, cnode.weight );
						child = cnode.next;
					}
				}
			}
			else
			{
				++remaining;
			}
		}
	} while ( remaining > 0 );
	
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

