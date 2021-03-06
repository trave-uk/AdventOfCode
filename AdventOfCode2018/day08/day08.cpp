// day8.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	std::vector<int> input;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			while ( *thisLine )
			{
				int number;
				if ( sscanf( thisLine, "%d", &number ) )
				{
					thisLine += strspn( thisLine, "0123456789" );
					thisLine += strspn( thisLine, " " );
					input.push_back( number );
				}
				else
				{
					break;
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	const int MAX_CHILDREN = 7;
	struct nodeHeader
	{
		int nChildren = -1;
		int nMetaData = -1;
		int parsedChildren = 0;
		int childValues[MAX_CHILDREN];
		int value = 0;
	};
	std::array<nodeHeader, 16384> headers;
	int nextHeader = 0;
	std::stack<nodeHeader*> nodes;
	nodeHeader *currentNode = nullptr;
	nodeHeader *parentNode = nullptr;
	bool readingMetaData = false;
	int totalMetaData = 0;
	int maxChildren = 0;
	// Now parse the input
	for ( int number : input )
	{
		while ( parentNode && parentNode->nChildren == 0 )
		{
			readingMetaData = true;
			if ( parentNode->nMetaData == 0 )
			{
				readingMetaData = false;
				int value = parentNode->value;
				nodes.pop();
				parentNode = nodes.top();
				currentNode = nullptr;
				assert( parentNode->parsedChildren <= MAX_CHILDREN );
				parentNode->childValues[parentNode->parsedChildren - 1] = value;
			}
			else
			{
				break;
			}
		}
		if ( !readingMetaData )
		{
			if ( !currentNode )
			{
				currentNode = &headers[nextHeader++];
			}
			if ( currentNode->nChildren == -1 )
			{
				currentNode->nChildren = number;
				if ( number > maxChildren )
					maxChildren = number;
				continue;
			}
			if ( currentNode->nMetaData == -1 )
			{
				currentNode->nMetaData = number;
				nodes.push( currentNode );
				if ( parentNode )
				{
					parentNode->nChildren--;
					parentNode->parsedChildren++;
				}
				parentNode = currentNode;
				currentNode = nullptr;
				continue;
			}
		}
		else
		{
			totalMetaData += number;
			parentNode->nMetaData--;
			if ( parentNode->parsedChildren == 0 )
			{
				parentNode->value += number;
			}
			else
			{
				if ( number > 0 && number <= parentNode->parsedChildren )
				{
					parentNode->value += parentNode->childValues[number - 1];
				}
			}
		}
	}
	printf( "Total metadata = %d\n", totalMetaData );
	printf( "Root node value = %d\n", parentNode->value );
	return 0;
}

// part 2 1st try: 115032 (too high)