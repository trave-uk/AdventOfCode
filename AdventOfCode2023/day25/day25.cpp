// day25.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 25/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Component
{
	std::string name;
	std::set<std::string> connections;
	bool operator<(const Component& other) const
	{
		return name < other.name;
	}
	bool operator==(const Component& other) const
	{
		return name == other.name;
	}
};

using Components = std::map<std::string, Component>;

using Connection = std::pair<std::string, std::string>;
using Connections = std::vector<Connection>;

int64 CountGroups(Components& components)
{
	int64 found = 1;
	// start at any component, "flood-fill" to find all connections.
	std::set<std::string> componentNames;
	while (components.size() != componentNames.size())
	{
		int64 thisGroup = 0;
		for (auto& start : components)
		{
			if (componentNames.count(start.first) == 0)
			{
				std::set<std::string> search;
				search.insert(start.first);
				while (!search.empty())
				{
					const auto it = search.begin();
					std::string c = *it;
					search.erase(it);
					if (componentNames.count(c) == 0)
					{
						++thisGroup;
						componentNames.insert(c);
					}
					Component comp = components[c];
					for (std::string con : comp.connections)
					{
						if (componentNames.count(con) == 0)
							search.insert(con);
					}
				}
				break;
			}
		}
		found *= thisGroup;
	}
	return found;
}

void EraseConnection(Components& test, const Connection& c)
{
	test[c.first].connections.erase(c.second);
	test[c.second].connections.erase(c.first);
}

void RestoreConnection(Components& test, const Connection& c)
{
	test[c.first].connections.insert(c.second);
	test[c.second].connections.insert(c.first);
}

// annoyingly, my A* algorithm is hard-coded to work on a grid, so copy-pasta it 
int64 aStarSearch(const Component& start, const Component& goal, const Components& space, std::vector<Component>* pPath = nullptr)
{
	std::set<Component> openSet;
	openSet.insert(start);
	std::map<Component, int64> gScore;
	gScore[start] = 0;
	std::map<Component, int64> fScore;
	fScore[start] = 0;

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<Component, Component> cameFrom;

	while (!openSet.empty())
	{
		int64 minF = INT_MAX;
		Component current;
		for (const Component& c : openSet)
		{
			if (fScore.count(c) && fScore[c] < minF)
			{
				minF = fScore[c];
				current = c;
			}
		}

		if (current.name == goal.name)
		{
			if (pPath)
			{
				pPath->clear();
				pPath->push_back(current);
				while (current.name != start.name)
				{
					current = cameFrom[current];
					pPath->push_back(current);
				}
				std::reverse(pPath->begin(), pPath->end());
			}
			return fScore[current];
		}

		openSet.erase(current);
		for (const std::string& next : current.connections)
		{
			Component neighbour = space.at(next);

			// tentative_gScore is the distance from start to the neighbour through current
			int64 tentative_gScore = gScore[current] + 1;
			int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
			if (tentative_gScore < neighbour_gScore)
			{
				// This path to neighbour is better than any previous one. Record it!
				cameFrom[neighbour] = current;
				gScore[neighbour] = tentative_gScore;
				fScore[neighbour] = tentative_gScore;
				openSet.insert(neighbour);
			}
		}
	}
	return -1; // failed to find a path
}


void Process(const char* filename, int64 expectedPart1 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	Components components;
	Connections connections;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// jqt: rhn xhk nvd
				char* c1 = strtok(thisLine, ":");
				char* connectedTo = strtok(nullptr, ":");
				char* c2 = strtok(connectedTo, " ");
				while (c2)
				{
					components[c1].name = c1;
					components[c1].connections.insert(c2);
					components[c2].name = c2;
					components[c2].connections.insert(c1);
					connections.push_back(make_pair(std::string(c1), std::string(c2)));
					c2 = strtok(nullptr, " ");
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(CountGroups(components) == components.size());

	std::set<Connection> removedConnections;
	for (int i = 0; i < 3; ++i)
	{
		int64 maxHits = 0;
		Connection maxHitConnection;
		std::map<Connection, int64> hits;
		// find shortest path to all other nodes.
		// from all of those paths, find the 3 connections used most often.
		std::vector<Component> path;
		for (auto& it : components)
		{
			for (auto& it2 : components)
			{
				if (it <= it2)
					continue;
				path.clear();
				aStarSearch(it.second, it2.second, components, &path);
				std::string prev;
				for (const Component& c : path)
				{
					if (prev.length())
					{
						Connection c1 = std::make_pair(c.name, prev);
						if (!hits.count(c1))
							c1 = std::make_pair(prev, c.name);
						++hits[c1];
						if (hits[c1] > maxHits)
						{
							maxHitConnection = c1;
							maxHits = hits[c1];
						}
					}
					prev = c.name;
				}
			}
		}
		printf("Removing connection %s/%s\n", maxHitConnection.first.c_str(), maxHitConnection.second.c_str());
		removedConnections.insert(maxHitConnection);
		EraseConnection(components, maxHitConnection);
	}

	int64 part1 = CountGroups(components);
	assert(part1 > components.size());

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	int64 answer = part1;
	printf("%s: Part 1: %lld\n", filename, answer);
}

int main()
{
	Process("example.txt", 54);
	Process("input.txt");

	return 0;
}
