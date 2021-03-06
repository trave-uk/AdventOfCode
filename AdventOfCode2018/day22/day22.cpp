// day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

const int width = 100;
const int height = 1000;

using map_t = std::array<std::array<int, width>, height>;

struct search_t
{
	int costs[3] = { INT_MAX, INT_MAX, INT_MAX };
};

using costmap_t = std::array<std::array<search_t, width>, height>;

// input.txt:
const int depth = 8103;
const int target_x = 9;
const int target_y = 758;

enum TYPE
{
	ROCKY,
	WET,
	NARROW
};

enum TOOL
{
	NEITHER,
	TORCH,
	CLIMBING_GEAR
};

void populate( map_t& map, int depth, int target_x, int target_y )
{
	map_t levels;

	int total = 0;
	for ( int y = 0; y < height; ++y )
	{
		for ( int x = 0; x < width; ++x )
		{
			int index = 0;
			if ( x == target_x && y == target_y )
				index = 0;
			else if ( x == 0 )
				index = y * 48271;
			else if ( y == 0 )
				index = x * 16807;
			else
				index = levels[y][x - 1] * levels[y - 1][x];
			int erosion = (index + depth) % 20183;
			levels[y][x] = erosion;
			int type = erosion % 3;
			map[y][x] = type;
			if ( y <= target_y && x <= target_x )
				total += type;
// 			if ( x == 0 && y == 0 )
// 				putchar( 'M' );
// 			else if ( x == target_x && y == target_y )
// 				putchar( 'T' );
// 			else if ( type == 0 )
// 				putchar( '.' );
// 			else if ( type == 1 )
// 				putchar( '=' );
// 			else
// 			{
// 				assert( type == 2 );
// 				putchar( '|' );
// 			}
		}
//		putchar( '\n' );
	}
	printf( "depth: %d\ntarget: %d,%d\n", depth, target_x, target_y );
	printf( "Total risk level = %d\n", total );
}

int switch_tool( int tool, int tile )
{
	assert( tool != tile );
	assert( tool >= 0 && tool < 3 );
	assert( tile >= 0 && tile < 3 );
	return 3 - tile - tool;
}

map_t map;
costmap_t costmap;
int max_cost;

void search_tile( int tool, int x, int y, int cost )
{
	if ( cost > max_cost )
		return;
	const int& tile = map[y][x];
	if ( tool == tile )
		return;
	search_t& cost_tile = costmap[y][x];
	int other_tool = switch_tool( tool, tile );
	bool bSearchThis = false;
	bool bSearchOther = false;
	if ( cost_tile.costs[tool] > cost )
	{
		cost_tile.costs[tool] = cost;
		bSearchThis = true;
	}
	if ( cost_tile.costs[other_tool] > cost + 7 )
	{
		cost_tile.costs[other_tool] = cost + 7;
		bSearchOther = true;
	}
	if ( !bSearchThis && !bSearchOther )
		return;
	// Don't search past the target
	if ( x == target_x && y == target_y )
	{
		max_cost = cost_tile.costs[TORCH];
//		printf( "Target found. Current lowest cost = %d\n", max_cost );
		return;
	}
	for ( int dir = 0; dir < 4; ++dir )
	{
		static const int dxs[4] = { -1, 0, +1, 0 };
		static const int dys[4] = { 0, -1, 0, +1 };
		int dx = dxs[dir];
		int dy = dys[dir];
		if ( x + dx >= 0 && x + dx < width && y + dy >= 0 && y + dy < height )
		{
			if ( bSearchThis )
				search_tile( tool, x + dx, y + dy, cost + 1 );
			if ( bSearchOther )
				search_tile( other_tool, x + dx, y + dy, cost + 7 + 1 );
		}
	}
}

int process( int depth, int target_x, int target_y )
{
	populate( map, depth, target_x, target_y );
	for ( auto& line : costmap )
	{
		line.fill( search_t() );
	}
	int cost = 0;
	int tool = TORCH;
	int x = 0;
	int y = 0;
	max_cost = (target_x + target_y) * 8;
	search_tile( tool, x, y, cost );
	printf( "Cost to get to target (with torch out) = %d\n", costmap[target_y][target_x].costs[TORCH] );
	getchar();
	return 0;
}

int main()
{
	process( 510, 10, 10 );
	process( depth, target_x, target_y );
	return 0;
}

// 5091 too high (interrupted calculation mid-processing, that was the current lowest value at target)
// 1029 correct answer (after realizing max_cost could be reduced whenever the target was reached)
