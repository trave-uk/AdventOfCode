// day24.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 24/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using coord_axis = double;
using coord_double = coord_type<coord_axis>;

struct HailStone
{
	HailStone(const coord_double p, const coord_double v)
		: pos2D(p)
		, vel2D(v)
	{
		pos = coord3(pos2D.first, pos2D.second, 0);
		vel = coord3(vel2D.first, vel2D.second, 0);
	}
	HailStone(const coord3& p, const coord3& v)
		: pos(p)
		, vel(v)
	{
		pos2D = coord_double(pos.get<0>(), pos.get<1>());
		vel2D = coord_double(vel.get<0>(), vel.get<1>());
	}
	HailStone(char* input)
	{
		// 19, 13, 30 @ -2,  1, -2
		char* p = strtok(input, "@");
		char* v = strtok(nullptr, "@");
		pos = coord3(p);
		vel = coord3(v);
		pos2D = coord_double(pos.get<0>(), pos.get<1>());
		vel2D = coord_double(vel.get<0>(), vel.get<1>());
	}
	coord3 pos;
	coord3 vel;
	coord_double pos2D;
	coord_double vel2D;
};

using HailStones = std::vector<HailStone>;

// Counter Clockwise function based on this: https://algs4.cs.princeton.edu/91primitives/
int ccw(const coord_double& a, const coord_double& b, const coord_double& c)
{
	coord_axis result = (b.first - a.first) * (c.second - a.second) - (c.first - a.first) * (b.second - a.second);
	if (result < 0.0)
		return -1;
	else if (result > 0.0)
		return 1;
	else
		return 0;
}

// This Stack Overflow answer gives a method of determining whether two line segments intersect:
// https://stackoverflow.com/a/23303498/532735
bool intersect(coord_double a, coord_double b, coord_double c, coord_double d)
{
	if (ccw(a, c, d) == ccw(b, c, d))
		return false;
	else if (ccw(a, b, c) == ccw(a, b, d))
		return false;
	else
		return true;
}

int64 CountIntersectionsPart1(const HailStones& hailStones, int64 minXY, int64 maxXY)
{
	HailStones hailStones2;
	// So, first extend each line to the extents of the box to get the start and end points.
	// If any hailstone doesn't pass through the box, it can be ignored.
	// If the starting point is within the box, don't move the start.
	coord_double min2D(minXY, minXY);
	coord_double max2D(maxXY, maxXY);
	for (HailStone h : hailStones)
	{
		coord_double start = h.pos2D;
		coord_double end = h.pos2D;
		end += h.vel2D;
		std::array<coord_axis, 4> timeToEdge;
		timeToEdge[0] = (minXY - start.first) / (end.first - start.first);
		timeToEdge[1] = (maxXY - start.first) / (end.first - start.first);
		timeToEdge[2] = (minXY - start.second) / (end.second - start.second);
		timeToEdge[3] = (maxXY - start.second) / (end.second - start.second);
		coord_double newStart = start;
		coord_double newEnd = end;
		int startEdge = -1;
		if (!h.pos2D.isContained(min2D, max2D))
		{
			// Extrude start to the edge it first meets. Try all 4 edges and choose the first one.
			coord_axis tStart = coord_axis(INT64_MAX);

			for (int edge = 0; edge < 4; ++edge)
			{
				coord_axis t = timeToEdge[edge];
				if (t >= 0 && t < tStart)
				{
					// Is this actually on the edge?
					coord_double dist = h.vel2D;
					dist *= t;
					newStart = start;
					newStart += dist;
					if (newStart.isContained(min2D, max2D))
					{
						tStart = t;
						startEdge = edge;
					}
				}
			}
			// If no edge found within the bounds, this hailstone is entirely external.
			if (startEdge == -1)
				continue;
			coord_double dist = h.vel2D;
			dist *= tStart;
			newStart = start;
			newStart += dist;
		}

		// Extrude further to find the end
		coord_axis tEnd = coord_axis(INT64_MAX);

		for (int edge = 0; edge < 4; ++edge)
		{
			if (edge == startEdge)
				continue;
			coord_axis t = timeToEdge[edge];
			if (t >= 0 && t < tEnd)
			{
				// Is this actually on the edge?
				coord_double dist = h.vel2D;
				dist *= t;
				newEnd = start;
				newEnd += dist;
				if (newEnd.isContained(min2D, max2D))
				{
					tEnd = t;
				}
			}
		}
		coord_double dist = h.vel2D;
		dist *= tEnd;
		newEnd = start;
		newEnd += dist;
		hailStones2.push_back(HailStone(newStart, newEnd));
	}
	int64 part1 = 0;
	int64 size = hailStones2.size();
	for (int64 i = 0; i < size - 1; ++i)
	{
		const HailStone& hi = hailStones2[i];
		for (int64 j = i + 1; j < size; ++j)
		{
			const HailStone& hj = hailStones2[j];

			if (intersect(hi.pos2D, hi.vel2D, hj.pos2D, hj.vel2D))
			{
				++part1;
			}
		}
	}
	return part1;
}

void Process(const char* filename, int64 minXY, int64 maxXY, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	HailStones hailStones;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				HailStone h(thisLine);
				hailStones.push_back(h);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = CountIntersectionsPart1(hailStones, minXY, maxXY);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 7, 27, 2);
	Process("input.txt", 200000000000000ll, 400000000000000ll);

	return 0;
}

// 4894 too low