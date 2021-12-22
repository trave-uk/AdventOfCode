// day22.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 22/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct coord3
{
	coord3(int64 _x = 0, int64 _y = 0, int64 _z = 0)
		: x(_x)
		, y(_y)
		, z(_z)
	{}
	int64 x;
	int64 y;
	int64 z;

	void operator=(const coord3& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
	}
	bool operator==(const coord3& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const coord3& rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}
	bool operator<(const coord3& rhs) const
	{
		return (x < rhs.x || (x == rhs.x && y < rhs.y) || (x == rhs.x && y == rhs.y && z < rhs.z));
	}
	void operator-=(const coord3& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}
	void operator+=(const coord3& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
	}
	int64 length() const
	{
		return abs(x) + abs(y) + abs(z);
	}
	bool isContainedExclusive(const coord3& min, const coord3& max) const
	{
		return x >= min.x && x < max.x && y >= min.y && y < max.y && z >= min.z && z < max.z;
	}
	void clamp(const coord3& min, const coord3& max)
	{
		if (x < min.x) x = min.x;
		if (x > max.x) x = max.x;
		if (y < min.y) y = min.y;
		if (y > max.y) y = max.y;
		if (z < min.z) z = min.z;
		if (z > max.z) z = max.z;
	}
};

// For part 2, consider everything as cuboids.
// Maintain a list of cuboids, which do not overlap.
// When 2 cuboids overlap, split one of them up into smaller cuboids, and remove those that exactly overlap with the other.
// So when adding (or indeed removing) a new cuboid, C, split up all existing cuboids (D) that overlap with it
//   While any cuboids overlap:
//     If D entirely contained within C, remove D
//     For first face of C intersecting with D, split D into 2 with that plane.
struct cuboid
{
	// NB: max exclusive
	cuboid(coord3 _min, coord3 _max)
	{
		min = _min;
		max = _max;
	}

	coord3 min;
	coord3 max;

	bool intersectsWith(const cuboid& other) const
	{
		return (min.x < other.max.x && max.x > other.min.x) &&
			(min.y < other.max.y && max.y > other.min.y) &&
			(min.z < other.max.z && max.z > other.min.z);
	}

	bool isContained(const cuboid& other) const
	{
		coord3 unit(1, 1, 1);
		coord3 max1 = max;
		max1 -= unit;
		return (min.isContainedExclusive(other.min, other.max) && max1.isContainedExclusive(other.min, other.max));
	}

	bool operator<(const cuboid& rhs) const
	{
		return min < rhs.min || (min == rhs.min && max < rhs.max);
	}
};

struct reactor
{
	std::set<cuboid> cuboids;

	// Split all cuboids that intersect with c, removing sections that are entirely within c
	void SplitCuboids(const cuboid& c)
	{
		// First, find intersecting cuboids (if any)
		std::set<cuboid> intersecting;
		for (const cuboid& d : cuboids)
		{
			if (c.intersectsWith(d))
			{
				intersecting.insert(d);
			}
		}
		// Erase all intersecting cuboids, as these will be split up and the areas not intersecting added back.
		for (const cuboid& d : intersecting)
		{
			cuboids.erase(d);
		}
		// Now, for all intersecting cuboids, split them up until no more intersect. Split each intersecting cuboid once per iteration
		while (!intersecting.empty())
		{
			// as intersecting cuboids are split, add the resulting cuboids to splitCuboids
			std::vector<cuboid> splitCuboids;
			for (const cuboid& d : intersecting)
			{
				// If D entirely contained within C, just remove D, so ignore it
				if (d.isContained(c))
				{
					continue;
				}

				// For each face of C intersecting with D, split D into 2.
				if (c.min.x > d.min.x && c.min.x < d.max.x)
				{
					coord3 midmin(c.min.x, d.min.y, d.min.z);
					coord3 midmax(c.min.x, d.max.y, d.max.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else if (c.max.x > d.min.x && c.max.x < d.max.x)
				{
					coord3 midmin(c.max.x, d.min.y, d.min.z);
					coord3 midmax(c.max.x, d.max.y, d.max.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else if (c.min.y > d.min.y && c.min.y < d.max.y)
				{
					coord3 midmin(d.min.x, c.min.y, d.min.z);
					coord3 midmax(d.max.x, c.min.y, d.max.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else if (c.max.y > d.min.y && c.max.y < d.max.y)
				{
					coord3 midmin(d.min.x, c.max.y, d.min.z);
					coord3 midmax(d.max.x, c.max.y, d.max.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else if(c.min.z > d.min.z && c.min.z < d.max.z)
				{
					coord3 midmin(d.min.x, d.min.y, c.min.z);
					coord3 midmax(d.max.x, d.max.y, c.min.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else if (c.max.z > d.min.z && c.max.z < d.max.z)
				{
					coord3 midmin(d.min.x, d.min.y, c.max.z);
					coord3 midmax(d.max.x, d.max.y, c.max.z);
					splitCuboids.push_back(cuboid(d.min, midmax));
					splitCuboids.push_back(cuboid(midmin, d.max));
				}
				else
				{
					assert(false);
				}
			}
			intersecting.clear();
			for (const cuboid& d : splitCuboids)
			{
				if (c.intersectsWith(d))
				{
					// If the split cuboid still intersects, add it back in to be split again by a different face
					intersecting.insert(d);
				}
				else
				{
					// If it doesn't, then it can be added back into the master list
					cuboids.insert(d);
				}
			}
		}
	}

	void Insert(const cuboid& c)
	{
		SplitCuboids(c);
		cuboids.insert(c);
	}

	void Remove(const cuboid& c)
	{
		SplitCuboids(c);
	}

	size_t size()
	{
		size_t sum = 0;
		for (const cuboid& c : cuboids)
		{
			sum += (c.max.x - c.min.x) * (c.max.y - c.min.y) * (c.max.z - c.min.z);
		}
		return sum;
	}
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::set<coord3> initRegion; // part 1
	reactor cuboids; // part 2

	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	coord3 topLeft(-50, -50, -50);
	coord3 bottomRight(50, 50, 50);
	coord3 unit(1, 1, 1);
	bottomRight += unit;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// on x=-20..26,y=-36..17,z=-47..7
				char action[4];
				coord3 min;
				coord3 max;
				int matches = sscanf(thisLine, "%s x=%lld..%lld,y=%lld..%lld,z=%lld..%lld", action, &min.x, &max.x, &min.y, &max.y, &min.z, &max.z);
				assert(matches == 7);
				bool bOn = strcmp(action, "on") == 0;
				assert(bOn || strcmp(action, "off") == 0);

				// Using "exclusive max" volumes, so they can have zero volume
				max += unit;
				cuboid c(min, max);

				// part 1
				max.clamp(topLeft, bottomRight);
				min.clamp(topLeft, bottomRight);

				coord3 pos;
				for (pos.x = min.x; pos.x < max.x; ++pos.x)
				{
					for (pos.y = min.y; pos.y < max.y; ++pos.y)
					{
						for (pos.z = min.z; pos.z < max.z; ++pos.z)
						{
							if (bOn)
							{
								initRegion.insert(pos);
							}
							else
							{
								initRegion.erase(pos);
							}
						}
					}
				}

				// part 2
				if (bOn)
				{
					cuboids.Insert(c);
				}
				else
				{
					cuboids.Remove(c);
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = initRegion.size();
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	if (expectedPart1 != -1 && expectedPart1 != part1)
		printf("WRONG! ");
	printf("%s: Part 1: %lld\n", filename, part1);

	int64 part2 = cuboids.size();
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	if (expectedPart2 != -1 && expectedPart2 != part2)
		printf("WRONG! ");
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example1.txt", 39, 39);
	Process("example2.txt", 590784);
	Process("example3.txt", 474140, 2758514936282235);
	Process("input.txt");

	return 0;
}
