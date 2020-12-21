// day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Edge
{
public:
	Edge() = default;

	Edge(std::string tileEdge)
	{
		assert(tileEdge.length() == 10);
		value = 0;
		flipped = 0;
		for (char c : tileEdge)
		{
			value = value << 1;
			flipped = flipped >> 1;
			if (c == '#')
			{
				value += 1;
				flipped += (1 << 9);
			}
		}
	}

	void flip()
	{
		int t = value;
		value = flipped;
		flipped = t;
	}

	bool match(const Edge& edge) const
	{
		return (value == edge.value || value == edge.flipped);
	}

	int value = 0;
	int flipped = 0;
};

enum EDGE
{
	TOP, RIGHT, BOTTOM, LEFT
};

EDGE operator++(EDGE& e)
{
	e = (EDGE)((int)e + 1);
	return e;
}

EDGE operator~(EDGE e)
{
	++e;
	++e;
	if (e > LEFT)
	{
		e = (EDGE)((int)e - 4);
	}
	return e;
}

class Tile
{
public:

	Tile() = default;

	Tile(int64 i, std::vector<std::string> c)
		: tileSize(10)
		, id(i)
		, contents(c)
	{
		assert(contents.size() == tileSize);
		edges[TOP] = contents[0];
		edges[BOTTOM] = contents[tileSize-1];
		std::string left;
		std::string right;
		for (std::string line : contents)
		{
			assert(line.length() == tileSize);
			left.append(1, line[0]);
			right.append(1, line[tileSize-1]);
		}
		edges[LEFT] = left;
		edges[RIGHT] = right;

		// Check for duplicates
		std::set<int> values;
		for (const Edge& e : edges)
		{
			values.insert(e.value);
			values.insert(e.flipped);
		}
		assert(values.size() == 8);
	}

	Tile(std::map<int64, Tile>& tiles, std::vector<std::vector<int64>> array, int left, int top, int size)
		: tileSize(size * 8)
	{
		contents.clear();
		contents.assign(tileSize, std::string());
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				for (int dy = 0; dy < 8; ++dy)
				{
					Tile& t = tiles[array[y+top][x+left]];
					contents[y * 8 + dy] += t.contents[dy + 1].substr(1, 8);
				}
			}
		}
	}

	void flip(EDGE edge)
	{
		assert(!locked);

		if (edge == TOP)
		{
			Edge t = edges[TOP];
			edges[TOP] = edges[BOTTOM];
			edges[BOTTOM] = t;
			edges[LEFT].flip();
			edges[RIGHT].flip();

			// flip contents 
			std::vector<std::string> newContents;
			for (auto it = contents.rbegin(); it != contents.rend(); ++it)
			{
				newContents.push_back(*it);
			}
			contents = newContents;
		}
		else
		{
			assert(edge == RIGHT);
			Edge t = edges[RIGHT];
			edges[RIGHT] = edges[LEFT];
			edges[LEFT] = t;
			edges[TOP].flip();
			edges[BOTTOM].flip();
	
			// flip contents 
			for (std::string& s : contents)
			{
				std::string newString;
				for (auto it = s.rbegin(); it != s.rend(); ++it)
				{
					newString += std::string(1, *it);
				}
				s = newString;
			}
		}
	}

	// Rotate clockwise 90 degrees "by" times
	void rotate(int by)
	{
		assert(!locked);

		// Handle negative rotation (anticlockwise)
		if (by < 0)
		{
			by += 4;
		}

		for (int i = 0; i < by; ++i)
		{
			Edge t = edges[TOP];
			edges[TOP] = edges[LEFT];
			edges[LEFT] = edges[BOTTOM];
			edges[BOTTOM] = edges[RIGHT];
			edges[RIGHT] = t;
			edges[TOP].flip();
			edges[BOTTOM].flip();

			// rotate contents
			std::vector<std::string> newContents;
			for (int i = 0; i < tileSize; ++i)
			{
				std::string newString;
				for (auto it = contents.rbegin(); it != contents.rend(); ++it)
				{
					newString += std::string(1, (*it)[i]);
				}
				newContents.push_back(newString);
			}
			contents = newContents;
		}
	}

	bool match(const Edge& edge) const
	{
		for (const Edge& e : edges)
		{
			if (e.match(edge))
			{
				return true;
			}
		}
		return false;
	}

	void lock()
	{
		locked = true;
	}

	bool isLocked()
	{
		return locked;
	}

	int tileSize;
	int64 id;
	bool locked = false;
	// 10 strings, each 10 characters long
	std::vector<std::string> contents;
	// Store each edge's value 0-1023, plus the flipped value
	std::array<Edge, 4> edges;
};

std::map<int64, Tile> tiles;

void sanityCheckTiles(int size)
{
	int adjacent2 = 0;
	int adjacent3 = 0;
	
	for (auto& it : tiles)
	{
		int adjacent = 0;
		Tile& t = it.second;
		for (Edge& e : t.edges)
		{
			int count = 0;
			for (auto& it2 : tiles)
			{
				Tile& t2 = it2.second;
				if (t.id != t2.id)
				{
					if (t2.match(e))
					{
						++count;
					}
				}
			}
			assert(count <= 1);
			adjacent += (count == 1);
		}
		assert(adjacent >= 2);
		adjacent2 += adjacent == 2;
		adjacent3 += adjacent == 3;
	}
	assert(adjacent2 == 4);
	assert(adjacent3 == (size - 2)*4);
}

std::vector<std::vector<int64>> array;
int arraySize = 0;

void sanityCheckArray(int left, int top, int size)
{
	int y = 0;
	for (std::vector<int64> line : array)
	{
		int x = 0;
		for (int64 id : line)
		{
			bool inside = x >= left && x < left + size && y >= top && y < top + size;
			bool idSet = id != 0;
			assert(inside == idSet);
			++x;
		}
		++y;
	}
}

void floodFill(int x, int y)
{
	int64 tileId = array[y][x];
	Tile& tile = tiles[tileId];
	tile.lock();
	for (EDGE e = TOP; e <= LEFT; ++e)
	{
		int x2 = x + (e == RIGHT) - (e == LEFT);
		int y2 = y + (e == BOTTOM) - (e == TOP);
		if (x2 < 0 || y2 < 0 || x2 >= arraySize || y2 >= arraySize)
		{
			continue;
		}
		int64& id2 = array[y2][x2];
		if (id2 == 0)
		{
			const Edge& edge = tile.edges[e];
			// Find a tile that fits, if one is found, fill it in and floodfill again
			for (auto& it2 : tiles)
			{
				Tile& t2 = it2.second;
				if (!t2.isLocked())
				{
					if (t2.match(edge))
					{
						EDGE e2 = ~e;
						Edge& edge2 = t2.edges[e2];
						// Now rotate/flip this tile to align with this one
						while (!edge2.match(edge))
						{
							t2.rotate(1);
						}
						if (edge2.value != edge.value)
						{
							int flipEdge = (int(e) + int(e2)) / 2;
							if (flipEdge > RIGHT)
								flipEdge -= 2;
							t2.flip( EDGE(flipEdge) );
						}
						assert(edge2.value == edge.value);

						// fill it in and floodfill again
						id2 = t2.id;
						floodFill(x2, y2);
					}
				}
			}
		}
	}
}

int main()
{
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	int64 firstTileId = -1;
	int64 tileId = -1;
	std::vector<std::string> tileContents;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if (*thisLine == 'T')
			{
				// "Tile 0000:"
				tileId = atoi(thisLine + 5);
				if (firstTileId == -1)
				{
					firstTileId = tileId;
				}
				tileContents.clear();
			}
			else if (*thisLine)
			{
				tileContents.push_back(thisLine);
				if (tileContents.size() == 10)
				{
					Tile t(tileId, tileContents);
					tiles[tileId] = t;
				}
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	// Double-check that there are no more than 2 of each edge
	int size = (int)(floor(sqrt(double(tiles.size())) + 0.5));
	sanityCheckTiles(size);

	// Now try to fit the tiles (in "tiles") together.
	// Create a 2D array that's twice as big as needed in both dimensions, put the first tile in the middle, and then "flood fill".
	arraySize = size * 2;
	array.assign(arraySize, std::vector<int64>(arraySize));
	for (std::vector<int64>& line : array)
	{
		line.assign(arraySize, 0);
	}

	int x = arraySize / 2;
	int y = arraySize / 2;
	array[x][y] = tiles[firstTileId].id;

	floodFill(x, y);

	// Find top-right corner
	int left = 0;
	int top = 0;
	for (std::vector<int64>& line : array)
	{
		if (line[size])
		{
			left = 0;
			for (int64 id : line)
			{
				if (id)
				{
					break;
				}
				++left;
			}
			break;
		}
		++top;
	}

	sanityCheckArray(left, top, size);

	int64 result = array[top][left] * array[top + size - 1][left] * array[top][left + size - 1] * array[top + size - 1][left + size - 1];
	printf( "Part 1: %lld\n", result );

	Tile sea(tiles, array, left, top, size);

	// Now search sea for monsters
	std::array<std::string, 3> monster = {
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "
	};

	int monsters = 0;
	for (int r = 0; r < 4 && !monsters; ++r)
	{
		for (int f = 0; f < 2 && !monsters; ++f)
		{
			for (y = 0; y < sea.tileSize - monster.size(); ++y)
			{
				for (x = 0; x < sea.tileSize - monster[0].length(); ++x)
				{
					bool notFound = false;
					std::vector<std::pair<int, int>> coords;
					for (int my = 0; my < monster.size() && !notFound; ++my)
					{
						for (int mx = 0; mx < monster[0].length(); ++mx)
						{
							if (monster[my][mx] == '#')
							{
								if (sea.contents[y + my][x + mx] == '#')
								{
									coords.push_back(std::make_pair(x + mx, y + my));
								}
								else
								{
									notFound = true;
									break;
								}
							}
						}
					}
					if (!notFound)
					{
						for (auto c : coords)
						{
							sea.contents[c.second][c.first] = 'O';
						}
						++monsters;
					}
				}
			}
			if (!monsters)
			{
				sea.flip(TOP);
			}
		}
		if (!monsters)
		{
			sea.rotate(1);
		}
	}

	printf("%d monsters\n", monsters);
	int part2 = 0;
	for (std::string line : sea.contents)
	{
		for (char c : line)
		{
			if (c == '#')
			{
				++part2;
			}
		}
	}
	printf("Part 2: %d\n", part2);
	return 0;
}
