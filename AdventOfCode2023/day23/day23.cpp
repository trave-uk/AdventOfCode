// day23.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 23/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::map<coord, char>;
using Coords = std::vector<coord>;

bool FindExits(const coord& pos, const Map& map, Coords& exits)
{
	exits.clear();
	// From pos, which other coordinates can we go to?
	std::string allowed = "><v^";
	int inout = 0;
	for (int d = 0; d < 4; ++d)
	{
		coord dir((d == 0) - (d == 1), (d == 2) - (d == 3));
		dir += pos;
		if (map.count(dir))
		{
			char next = map.at(dir);
			if (next == '.' || next == allowed[d])
				exits.push_back(dir);
			if (next != '#')
				++inout;
		}
	}
	return (inout > 2);
}

struct Edge
{
	Edge(const coord& dest = coord(0, 0), int64 dist = 0)
		: dest(dest)
		, dist(dist)
	{}
	bool operator<(const Edge& other) const
	{
		if (dest < other.dest)
			return true;
		else if (dest > other.dest)
			return false;
		else
			return dist < other.dist;
	}
	bool operator==(const Edge& other) const
	{
		return dest == other.dest && dist == other.dist;
	}
	coord dest;
	int64 dist;
};
using Graph = std::map<coord, std::vector<Edge>>;

int64 LongestDistance(const coord& start, const coord& end, const Map& map, bool part2 = false)
{
	// The map looks big but actually every junction has directed inputs and outputs. There are choices at some junctions.
	// Therefore, given a point (at a junction), find the distance to all possible next junctions, iterate to build a graph.
	Graph graph;
	std::set<coord> junctions;
	junctions.insert(start);
	while (!junctions.empty())
	{
		auto it = junctions.begin();
		coord pos = *it;
		junctions.erase(it);
		if (graph.count(pos))
			continue;
		Coords exits;
		FindExits(pos, map, exits);
		for (coord path : exits)
		{
			Edge edge(path, 1);
			Coords exits2;
			coord prev = pos;
			while (!FindExits(path, map, exits2))
			{
				coord next;
				if (exits2[0] == prev)
					next = exits2[1];
				else
					next = exits2[0];
				++edge.dist;
				edge.dest = next;
				prev = path;
				path = next;
				if (path == end || path == start)
					break;
			}
			graph[pos].push_back(edge);
			if (path != end)
				junctions.insert(path);
		}
	}

	if (part2)
	{
		// In part 2, direction in the graph is two-way, so add paths back along all paths between junctions (except from end or to start)
		for (auto& x : graph)
		{
			if (x.first == start)
				continue;
			for (auto& y : x.second)
			{
				if (y.dest == end)
					continue;
				Edge edge(x.first, y.dist);
				std::vector<Edge>& edges = graph[y.dest];
				bool found = false;
				for (auto it = edges.begin(); it != edges.end(); ++it)
				{
					if (*it == edge)
					{
						found = true;
						break;
					}
				}
				if (!found)
					edges.push_back(edge);
			}
		}
	}

	// Now, traverse the graph, following all paths and adding up the distances to end
	using Search = std::pair<Edge, Coords>;
	std::set<Search> search;
	search.insert(std::make_pair(Edge(start, 0), Coords()));
	int64 longestDistance = 0;
	while (!search.empty())
	{
		auto it = search.begin();
		Coords path = it->second;
		int64 distance = it->first.dist;
		coord pos = it->first.dest;
		path.push_back(pos);
		search.erase(it);

		for (const Edge& e : graph[pos])
		{
			if (std::find(path.begin(), path.end(), e.dest) == path.end())
			{
				Search nextSearch = std::make_pair(Edge(e.dest, distance + e.dist), path);
				if (e.dest == end)
				{
					if (nextSearch.first.dist > longestDistance)
						longestDistance = nextSearch.first.dist;
					continue;
				}
				search.insert(nextSearch);
			}
		}
	}
	return longestDistance;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	coord pos(0, 0);
	Map map;
	coord start(-1, -1);
	coord end(-1, -1);
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				pos.first = 0;
				for (char c : line)
				{
					if (c == '.')
					{
						if (start.first == -1)
							start = pos;
						end = pos;
					}
					map[pos] = c;
					++pos.first;
				}
			}
			++pos.second;
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = LongestDistance(start, end, map);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	part2 = LongestDistance(start, end, map, true);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 94, 154);
	Process("input.txt");

	return 0;
}
