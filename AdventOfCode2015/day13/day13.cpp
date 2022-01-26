// day13.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 26/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Order = std::vector<std::string>;
using Costs = std::map<std::pair<std::string, std::string>, int64>;

int64 Evaluate(Order& order, Costs& costs, bool part2 = false)
{
	int64 total = 0;
	int size = order.size();
	for (int i = 0; i < (part2?(size-1):size); ++i)
	{
		total += costs[std::make_pair(order[i], order[(i + 1) % size])];
		total += costs[std::make_pair(order[(i + 1) % size], order[i])];
	}
	return total;
}

void GenerateOrders(Order& order, std::vector<Order>& orders)
{
	if (order.size() == 1)
	{
		orders.push_back(order);
		return;
	}
	for (std::string first : order)
	{
		Order remaining = order;
		for (auto it = remaining.begin(); it != remaining.end(); ++it)
		{
			if (*it == first)
			{
				remaining.erase(it);
				break;
			}
		}
		std::vector<Order> orders2;
		GenerateOrders(remaining, orders2);
		for (Order o : orders2)
		{
			Order p;
			p.push_back(first);
			p.insert(p.end(), o.begin(), o.end());
			orders.push_back(p);
		}
	}
}

void Process(const char* filename, int64 expectedPart1 = -1)
{
	std::set<std::string> people;
	Costs costs;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// Alice would gain 54 happiness units by sitting next to Bob.
				char primary[10];
				char secondary[10];
				char gainLose[5];
				int64 happiness;
				int matches = sscanf(thisLine, "%s would %s %lld happiness units by sitting next to %[^.]s", primary, gainLose, &happiness, secondary);
				assert(matches == 4);
				if (strcmp(gainLose, "lose") == 0)
				{
					happiness = -happiness;
				}
				else
				{
					assert(strcmp(gainLose, "gain") == 0);
				}
				people.insert(primary);
				people.insert(secondary);
				std::pair<std::string, std::string> pairing = std::make_pair(primary, secondary);
				assert(costs.count(pairing) == 0);
				costs.insert(std::make_pair(pairing, happiness));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// as table is circular, all Orders start with the first person in the list, but each subsequent person is then chosen from the pool of those remaining.
	// the number of permutations could be reduced further as the reverse is equivalent, but life is short and computers are fast enough ;)
	Order order;
	for (std::string name : people)
	{
		order.push_back(name);
	}

	std::vector<Order> orders;
	GenerateOrders(order, orders);

	int64 part1 = 0;
	int64 part2 = 0;
	for (Order o : orders)
	{
		int64 happiness = Evaluate(o, costs);
		if (happiness > part1)
			part1 = happiness;
		int64 happiness2 = Evaluate(o, costs, true);
		if (happiness2 > part2)
			part2 = happiness2;
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 330);
	Process("input.txt");

	return 0;
}
