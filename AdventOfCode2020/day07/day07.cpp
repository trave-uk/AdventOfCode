// day07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct node
{
	std::string bagType;
	std::vector<int> childCounts;
	std::vector<std::string> childBagTypes;
};

using bagT = std::map<std::string, node>;
bagT bags;

bool findBag(std::string searchBag, node bag)
{
	for (std::string childBagName : bag.childBagTypes)
	{
		bagT::iterator xIter = bags.find(childBagName);
		assert(xIter != bags.end());
		if (xIter->first == searchBag )
			return true;
		if (findBag(searchBag, xIter->second))
			return true;
	}
	return false;
}

int searchBags(std::string searchBag)
{
	int total = 0;
	for (std::pair<std::string, node> top : bags)
	{
		if (findBag(searchBag, top.second))
		{
			++total;
		}
	}
	return total;
}

int countBags(std::string searchBag)
{
	bagT::iterator xIter = bags.find(searchBag);
	assert(xIter != bags.end());

	node bag = xIter->second;
	int numChildren = bag.childCounts.size();

	int total = 0;

	for (int i = 0; i < numChildren; ++i)
	{
		int count = bag.childCounts[i];
		std::string name = bag.childBagTypes[i];

		total += count * (1 + countBags(name));
	}
	return total;
}

int main()
{
	char* buffer = new char[65536];
//	FILE *fp = fopen("example.txt", "rt");
//	FILE *fp = fopen("example2.txt", "rt");
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				node bag;
				char* contains = strstr(thisLine, " bags contain ");
				*contains = '\0';
				bag.bagType = std::string(thisLine);
				char* bagsString = contains + 14;
				char* bagString = strtok(bagsString, ",.");
				while (bagString && *bagString)
				{
					if (*bagString == ' ')
					{
						++bagString;
					}
					if (strcmp(bagString, "no other bags") == 0)
					{
						break;
					}
					int count = atoi(bagString);
					bagString += strcspn(bagString, " ");
					++bagString;
					if (count != 1)
					{
						assert(bagString[strlen(bagString) - 1] == 's');
						bagString[strlen(bagString) - 1] = '\0';
					}
					assert(strcmp(&(bagString[strlen(bagString) - 4]), " bag") == 0);
					bagString[strlen(bagString) - 4] = '\0';
					bag.childCounts.push_back(count);
					bag.childBagTypes.push_back(std::string(bagString));

					// continue
					bagString = strtok(nullptr, ",.");
				}
				bags.insert( std::make_pair(bag.bagType, bag) );
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	std::string searchBag("shiny gold");
	int part1 = searchBags(searchBag);

	printf( "Part 1: %d\n", part1 );

	int part2 = countBags(searchBag);

	printf("Part 2: %d\n", part2);

	return 0;
}
