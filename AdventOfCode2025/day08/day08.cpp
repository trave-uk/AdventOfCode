// day08.cpp : Advent of Code 2025 https://adventofcode.com/2025
// Solution by trave.uk 08/12/2025 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Point
{
	int64 x;
	int64 y;
	int64 z;
};
using Points = std::vector<Point>;

struct Connection
{
	int64 squareDistance;
	int64 startIndex;
	int64 endIndex;
	bool operator<(const Connection& other) const
	{
		return squareDistance < other.squareDistance || (squareDistance == other.squareDistance && (startIndex < other.startIndex || (startIndex == other.startIndex && endIndex < other.endIndex)));
	}
};
using Connections = std::set<Connection>;

using Circuit = std::set<int64>;
using Circuits = std::vector<Circuit>;

int64 Calculate(const Points& points, int64 part1Connections, int64& part2)
{
	part2 = 0;
	int64 total = points.size();
	Connections connections;
	Connection c;
	for (c.startIndex = 0; c.startIndex < total; ++c.startIndex)
	{
		const Point& p1 = points[c.startIndex];
		for (c.endIndex = c.startIndex + 1; c.endIndex < total; ++c.endIndex)
		{
			const Point& p2 = points[c.endIndex];
			c.squareDistance = ((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)) + ((p1.z - p2.z) * (p1.z - p2.z));
			connections.insert(c);
		}
	}
	int64 part1Count = part1Connections;
	Circuits circuits;
	int64 part1 = 1;
	for (const Connection& connection : connections)
	{
		bool found = false;
		Circuit* foundCircuit = nullptr;
		for (Circuit& circuit : circuits)
		{
			if (circuit.count(connection.startIndex) || circuit.count(connection.endIndex))
			{
				if (foundCircuit)
				{
					// this connection was found in 2 existing circuits: merge them together
					for (int64 node : circuit)
					{
						foundCircuit->insert(node);
					}
					circuit.clear();
				}
				else
				{
					circuit.insert(connection.startIndex);
					circuit.insert(connection.endIndex);
				}
				found = true;
				foundCircuit = &circuit;
				if (part2 == 0 && circuit.size() == total)
				{
					const Point& p1 = points[connection.startIndex];
					const Point& p2 = points[connection.endIndex];
					part2 = p1.x * p2.x;
				}
			}
		}
		if (!found)
		{
			Circuit circuit;
			circuit.insert(connection.startIndex);
			circuit.insert(connection.endIndex);
			circuits.push_back(circuit);
		}
		if (!--part1Count)
		{
			// Find three largest circuits and multiply the sizes together
			std::set<int64> used;
			for (int i = 0; i < 3; ++i)
			{
				int64 maxSize = 0;
				int64 index = 0;
				int64 maxIndex = -1;
				for (Circuit& circuit : circuits)
				{
					if (circuit.size() > maxSize && !used.count(index))
					{
						maxSize = circuit.size();
						maxIndex = index;
					}
					++index;
				}
				used.insert(maxIndex);
				part1 *= maxSize;
			}
		}
		if (part2)
			break;
	}
	return part1;
}

void Process(const char* filename, int64 part1Connections, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Points points;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				Point c;
				int count = sscanf(thisLine, "%lld,%lld,%lld", &c.x, &c.y, &c.z);
				assert(count == 3);
				points.push_back(c);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = Calculate(points, part1Connections, part2);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 10, 40, 25272);
	Process("input.txt", 1000);

	return 0;
}
