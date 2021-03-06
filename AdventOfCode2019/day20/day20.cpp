// day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

std::vector<std::string> map;

bool load( const char *filename )
{
	map.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				std::string line = thisLine;
				map.push_back( line );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

struct portal
{
	// Each x,y position is in the spot just outside (or inside) the donut.
	union
	{
		int id = 0;
		char name[4];
	};
	int outerx = -1;
	int outery = -1;
	int innerx = -1;
	int innery = -1;
	bool operator<( const portal& other ) const
	{
		return id < other.id;
	}
};
std::map<int, portal> portals;

struct link
{
	int value() const { return fromx + fromy * 256; }
	bool operator<( const link& other ) const { return value() < other.value(); }
	int fromx = -1, fromy = -1;
	int tox = -1, toy = -1;
	bool outer = true;
};
std::map<int, link> links;

portal start, end;

std::vector<std::vector<std::vector<int>>> search;

void convert()
{
	// Parse the map.
	//
	// First, find the size
	int outer_width = 0, outer_height = map.size() - 4;
	int thick_top = 0, thick_bottom = 0, thick_left = 0, thick_right = 0;
	bool found_mid = false;
	for ( int y = 2; y < map.size() - 2; ++y )
	{
		const std::string line = map[y];
		int width = line.size() - 4;
		assert( !outer_width || outer_width == width );
		outer_width = width;
		char mid = line[line.size() / 2];
		if ( mid == '.' || mid == '#' )
		{
			// This line is in the top or bottom edge
			if ( !found_mid )
			{
				++thick_top;
			}
			else
			{
				++thick_bottom;
			}
		}
		else
		{
			found_mid = true;
			int left = 0, right = 0;
			bool mid = false;
			for ( int x = 2; x < line.size() - 2; ++x )
			{
				char tile = line[x];
				if ( tile == '.' || tile == '#' )
				{
					if ( !mid )
					{
						++left;
					}
					else
					{
						++right;
					}
				}
				else
				{
					mid = true;
				}
			}
			assert( !thick_left || thick_left == left );
			assert( !thick_right || thick_right == right );
			thick_left = left;
			thick_right = right;
		}
	}
	assert( thick_top == thick_bottom && thick_left == thick_right && thick_left == thick_top );
	int thickness = thick_top;
	int inner_width = outer_width - thickness * 2;
	int inner_height = outer_height - thickness * 2;

	// Now find the portals
	portals.clear();

	// First look around the outer edges
	for ( int w = 0; w < outer_width; ++w )
	{
		// top
		if ( map[1][w + 2] != ' ' )
		{
			portal p;
			p.name[0] = map[0][w + 2];
			p.name[1] = map[1][w + 2];
			p.outerx = w + 2;
			p.outery = 2;
			portals.insert( std::make_pair( p.id, p ) );
		}
		// bottom
		if ( map[outer_height + 2][w + 2] != ' ' )
		{
			portal p;
			p.name[0] = map[outer_height + 2][w + 2];
			p.name[1] = map[outer_height + 3][w + 2];
			p.outerx = w + 2;
			p.outery = outer_height + 1;
			portals.insert( std::make_pair( p.id, p ) );
		}
	}
	for ( int h = 0; h < outer_height; ++h )
	{
		// left
		if ( map[h + 2][1] != ' ' )
		{
			portal p;
			p.name[0] = map[h + 2][0];
			p.name[1] = map[h + 2][1];
			p.outerx = 2;
			p.outery = h + 2;
			portals.insert( std::make_pair( p.id, p ) );
		}
		// right
		if ( map[h + 2][outer_width + 2] != ' ' )
		{
			portal p;
			p.name[0] = map[h + 2][outer_width + 2];
			p.name[1] = map[h + 2][outer_width + 3];
			p.outerx = outer_width + 1;
			p.outery = h + 2;
			portals.insert( std::make_pair( p.id, p ) );
		}
	}

	// Now look within the inner edges
	for ( int w = 0; w < inner_width; ++w )
	{
		// top
		if ( map[2 + thickness][w + 2 + thickness] != ' ' )
		{
			portal p;
			p.name[0] = map[2 + thickness][w + 2 + thickness];
			p.name[1] = map[3 + thickness][w + 2 + thickness];
			portal &p2 = portals[p.id];
			p2.innerx = w + 2 + thickness;
			p2.innery = 1 + thickness;
		}
		// bottom
		if ( map[outer_height + 1 - thickness][w + 2 + thickness] != ' ' )
		{
			portal p;
			p.name[0] = map[outer_height - thickness][w + 2 + thickness];
			p.name[1] = map[outer_height + 1 - thickness][w + 2 + thickness];
			portal &p2 = portals[p.id];
			p2.innerx = w + 2 + thickness;
			p2.innery = outer_height + 2 - thickness;
		}
	}
	for ( int h = 0; h < inner_height; ++h )
	{
		// left
		if ( map[h + 2 + thickness][thickness + 2] != ' ' )
		{
			portal p;
			p.name[0] = map[h + 2 + thickness][thickness + 2];
			p.name[1] = map[h + 2 + thickness][thickness + 3];
			portal &p2 = portals[p.id];
			p2.innerx = thickness + 1;
			p2.innery = h + 2 + thickness;
		}
		// right
		if ( map[h + 2 + thickness][thickness + inner_width + 1] != ' ' )
		{
			portal p;
			p.name[0] = map[h + 2 + thickness][thickness + inner_width];
			p.name[1] = map[h + 2 + thickness][thickness + inner_width + 1];
			portal &p2 = portals[p.id];
			p2.innerx = thickness + inner_width + 2;
			p2.innery = h + 2 + thickness;
		}
	}

	strcpy( start.name, "AA" );
	strcpy( end.name, "ZZ" );
	start = portals[start.id];
	end = portals[end.id];

	// Now use the portal list to create the links between nodes
	links.clear();
	for ( auto pit : portals )
	{
		portal p = pit.second;
		if ( p.id == start.id || p.id == end.id )
			continue;
		assert( p.innerx > 0 && p.innery > 0 && p.outerx > 0 && p.outery > 0 );
		link l;
		l.fromx = p.outerx;
		l.fromy = p.outery;
		l.tox = p.innerx;
		l.toy = p.innery;
		l.outer = true;
		map[l.fromy][l.fromx] = '*';
		links.insert( std::make_pair( l.value(), l ) );
		l.fromx = p.innerx;
		l.fromy = p.innery;
		l.tox = p.outerx;
		l.toy = p.outery;
		l.outer = false;
		links.insert( std::make_pair( l.value(), l ) );
		map[l.fromy][l.fromx] = '*';
	}
}

void add_search()
{
	// Initialise the search vector
	std::vector<std::vector<int>> s;
	for ( std::string line : map )
	{
		std::vector<int> searchline;
		for ( char tile : line )
		{
			searchline.push_back( -1 );
		}
		s.push_back( searchline );
	}
	search.push_back( s );
}

int nearest = INT_MAX;

struct tilesearch
{
	tilesearch( int ll, int xx, int yy, int dd ) : level( ll ), x( xx ), y( yy ), distance( dd ) {}
	bool operator<( const tilesearch& other ) const { return level > other.level; }
	int level;
	int x;
	int y;
	int distance;
};
std::priority_queue<tilesearch> tiles;

void floodfill( int ll, int xx, int yy, int part = 1 )
{
	tilesearch t( ll, xx, yy, 0 );
	tiles.push( t );

	while ( tiles.size() > 0 )
	{
		t = tiles.top();
		tiles.pop();

		if ( t.level < 0 || map[t.y][t.x] == '#' || isalpha( map[t.y][t.x] ) )
			continue;
		if ( t.distance > nearest )
			continue;
		while ( t.level >= search.size() )
			add_search();
		if ( search[t.level][t.y][t.x] == -1 || search[t.level][t.y][t.x] > t.distance )
		{
			search[t.level][t.y][t.x] = t.distance;
			if ( t.level == 0 && t.x == end.outerx && t.y == end.outery )
			{
				printf( "Closest path: %d steps    \n", t.distance );
				nearest = t.distance;
			}
			const int d = t.distance + 1;

			tiles.push( tilesearch( t.level, t.x - 1, t.y, d ) );
			tiles.push( tilesearch( t.level, t.x + 1, t.y, d ) );
			tiles.push( tilesearch( t.level, t.x, t.y - 1, d ) );
			tiles.push( tilesearch( t.level, t.x, t.y + 1, d ) );
			if ( map[t.y][t.x] == '*' )
			{
				static link la;
				la.fromx = t.x;
				la.fromy = t.y;
				const link& l = links[la.value()];
				int ll = t.level;
				if ( part == 2 )
				{
					if ( l.outer )
						--ll;
					else
					{
						++ll;
					}
				}
				tiles.push( tilesearch( ll, l.tox, l.toy, d ) );
			}
		}
	}
}

void part1()
{
	nearest = INT_MAX;
	search.clear();
	floodfill( 0, start.outerx, start.outery, 1 );
}

void part2()
{
	nearest = INT_MAX;
	search.clear();
	floodfill( 0, start.outerx, start.outery, 2 );
}

int main()
{
//	if ( load( "example1.txt" ) )
// 	if ( load( "example2.txt" ) )
 	if ( load( "input.txt" ) )
	{
		printf( "Part 1\n" );
		// Parse the map to find links
		convert();
		// Now process the map
		part1();
	}
//	if ( load( "example1.txt" ) )
// 	if ( load( "example3.txt" ) )
 	if ( load( "input.txt" ) )
	{
		printf( "Part 2\n" );
		convert();
		part2();
	}
	return 0;
}

