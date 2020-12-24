// day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//  |     |     |
// / \ nw/ \ne / \
//    \ /   \ /
//   w |     | e
//     |     |
//    / \   / \
// \ / sw\ /se \ /
//  |     |     |

struct Coord
{
	int x = 0;
	int y = 0;
	bool operator<(const Coord other) const
	{
		return (x < other.x) || (x == other.x && y < other.y);
	}
};

using Map = std::set<Coord>;
Map map;


Coord GetNeighbour(const Coord& c, int i)
{
	// e, se, sw, w, nw, ne
	Coord t = c;
	t.x -= (i >= 2 && i <= 4) + (i == 3);
	t.x += (i < 2 || i == 5) + (i == 0);
	t.y -= (i >= 4);
	t.y += (i == 1 || i == 2);
	return t;
}

int CountNeighbours(const Coord& c)
{
	int total = 0;

	for (int i = 0; i < 6; ++i)
	{
		if (map.count(GetNeighbour(c, i)))
		{
			++total;
		}
	}
	return total;
}

void ProcessStep()
{
	Map neighbours;
	for (Coord active : map)
	{
		neighbours.insert(active);
		for (int i = 0; i < 6; ++i)
		{
			neighbours.insert(GetNeighbour(active, i));
		}
	}

	Map next;
	for (Coord test : neighbours)
	{
		int count = CountNeighbours(test);
		bool black = map.count(test) > 0;

		if (black)
		{
			if (count == 1 || count == 2)
			{
				next.insert(test);
			}
		}
		else if (!black && count == 2)
		{
			next.insert(test);
		}
	}
	map = next;
}

size_t ProcessFile(const char *filename, int steps)
{
	map.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			// e=x+2 ne=y-1,x+1 se=y+1,x+1 w=x-2 nw=y-1,x-1 sw=y+1,x-1
			Coord xy;
			for (char*c = thisLine; *c; ++c)
			{
				switch (*c)
				{
				case 'e':
					xy.x += 2; break;
				case 'w':
					xy.x -= 2; break;
				case 'n':
					xy.y -= 1;
					++c;
					xy.x += (*c == 'e') - (*c == 'w');
					break;
				case 's':
					xy.y += 1;
					++c;
					xy.x += (*c == 'e') - (*c == 'w');
					break;
				}
			}
			if (map.count(xy))
				map.erase(xy);
			else
				map.insert(xy);
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int s = 0; s < steps; ++s)
	{
		ProcessStep();
	}

	return map.size();
}

int main()
{
	//const char* filename = "example.txt";
	const char* filename = "input.txt";
	printf("Part 1: %zd\n", ProcessFile(filename, 0));
	printf("Part 2: %zd\n", ProcessFile(filename, 100));
	return 0;
}
