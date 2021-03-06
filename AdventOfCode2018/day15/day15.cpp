// day15.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

static const int SIZE = 32;
// map is a 32x32 array of bools (input.txt is 32x32)
std::array<std::array<bool, SIZE>, SIZE> map;
int width = 0;
int height = 0;

struct entity
{
	int id = -1;
	char type = '.';
	int health = 200;
	int attack = 3;
};
std::map<int, entity> entities;

struct entityPos
{
	int id = -1;
	int x = -1;
	int y = -1;
	bool operator<( const entityPos& other ) const
	{
		if ( y < other.y ) return true;
		if ( y == other.y && x < other.x ) return true;
		return false;
	}
};
std::set<entityPos> entityPositions;


int findEnemies( std::set<entityPos>& results, char myType )
{
	results.clear();
	for ( auto p : entityPositions )
	{
		entity& e = entities[p.id];
		if ( e.type != myType && e.health > 0 )
		{
			results.insert( p );
		}
	}
	return results.size();
}

bool isPassable( std::array<std::array<int, SIZE>, SIZE>& distances, int x, int y, int distance )
{
	if ( x < 0 || x >= width || y < 0 || y >= height )
		return false;
	if ( !map[y][x] )
		return false;
	if ( distances[y][x] <= distance )
		return false;
	entityPos e;
	e.x = x;
	e.y = y;
	if ( entityPositions.count( e ) > 0 )
	{
		return false;
	}
	return true;
}

void fillfrom( std::array<std::array<int, SIZE>, SIZE>& distances, int x, int y, int distance )
{
	distances[y][x] = distance;
	++distance;
	// Check up
	if ( isPassable( distances, x, y - 1, distance ) )
		fillfrom( distances, x, y - 1, distance );
	// left
	if ( isPassable( distances, x - 1, y, distance ) )
		fillfrom( distances, x - 1, y, distance );
	// right
	if ( isPassable( distances, x + 1, y, distance ) )
		fillfrom( distances, x + 1, y, distance );
	// down
	if ( isPassable( distances, x, y + 1, distance ) )
		fillfrom( distances, x, y + 1, distance );
}

void print_map( int round )
{
	if ( round == 0 )
		printf( "Initially\n" );
	else
		printf( "\nAfter %d round%s:\n", round, round > 1 ? "s" : "" );
	for ( int y = 0; y < height; ++y )
	{
		std::string health( "   " );
		for ( int x = 0; x < width; ++x )
		{
			if ( !map[y][x] )
			{
				putchar( '#' );
			}
			else
			{
				entityPos test;
				test.x = x;
				test.y = y;
				auto found = entityPositions.find( test );
				if ( found != entityPositions.end() )
				{
					entityPos p = *found;
					entity& e = entities[p.id];
					putchar( e.type );
					health += e.type;
					health += " (" + std::to_string( e.health ) + ") ";
				}
				else
				{
					putchar( '.' );
				}
			}
		}
		printf( "%s\n", health.c_str() );
	}
}

void insert_target( std::set<entityPos>& targets, const entityPos& newPos )
{
	if ( targets.count( newPos ) > 0 )
	{
		entityPos p = *targets.find( newPos );
		entity& e1 = entities[newPos.id];
		entity& e2 = entities[p.id];
		bool swap = false;
		if ( e1.health < e2.health )
			swap = true;
		else if ( e1.health == e2.health )
		{
			if ( newPos < p )
				swap = true;
		}
		if ( swap )
		{
			targets.erase( p );
			targets.insert( newPos );
		}
	}
	else
	{
		targets.insert( newPos );
	}
}

bool process(const char *filename, int elfAttack = 3)
{
	bool elfDied = false;
	width = height = 0;
	for ( auto& line : map )
	{
		line.fill( false ); // impassable
	}
	entities.clear();
	entityPositions.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	int y = 0;
	int id = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				int len = strlen( thisLine );
				width = std::max( width, len );
				for ( int x = 0; x < len; ++x )
				{
					char tile = thisLine[x];
					int attack = 3;
					switch ( tile )
					{
						case 'E':
							attack = elfAttack;
						// fall through:
						case 'G':
						{
							entity e;
							e.id = id++;
							e.type = tile;
							e.attack = attack;
							entities.insert( std::pair<int, entity>( e.id, e ) );
							entityPos p;
							p.id = e.id;
							p.x = x;
							p.y = y;
							entityPositions.insert( p );
						}
						// fall through:
						case '.':
							map[y][x] = true;	// passable
							break;
					}
				}
				++y;
				height = std::max( height, y );
			}
		}
	}
	fclose( fp );
	delete[] buffer;

	bool finished = false;
	int round = 0;
	//print_map( round );
	// Each round, each entity processes in turn.
	// Each entity finds all other opposing entities, and attacks or moves towards the closest one
	while ( !finished )
	{
		std::set<entityPos> oldPositions = entityPositions;
		for ( auto p : oldPositions )
		{
			entity e = entities[p.id];
			if ( e.health <= 0 )
				continue;
			std::set<entityPos> enemyPositionss;
			int count = findEnemies( enemyPositionss, e.type );
			if ( !count )
			{
				finished = true;
				break;
			}
			// Floodfill array of distances from e
			std::array<std::array<int, SIZE>, SIZE> distances;
			for ( auto& line : distances )
			{
				line.fill( INT_MAX );
			}
			fillfrom( distances, p.x, p.y, 0 );
			// Find all positions adjacent to enemies
			std::set<entityPos> targets;
			for ( auto& pos : enemyPositionss )
			{
				const int x = pos.x;
				const int y = pos.y;
				if ( distances[y - 1][x] < INT_MAX )
				{
					entityPos newPos = pos;
					--newPos.y;
					insert_target( targets, newPos );
				}
				if ( distances[y][x - 1] < INT_MAX )
				{
					entityPos newPos = pos;
					--newPos.x;
					insert_target( targets, newPos );
				}
				if ( distances[y][x + 1] < INT_MAX )
				{
					entityPos newPos = pos;
					++newPos.x;
					insert_target( targets, newPos );
				}
				if ( distances[y + 1][x] < INT_MAX )
				{
					entityPos newPos = pos;
					++newPos.y;
					insert_target( targets, newPos );
				}
			}
			// Find closest enemy
			int nearestEnemyId = -1;
			int nearestDistance = INT_MAX;

			for ( auto& pos : targets )
			{
				const int x = pos.x;
				const int y = pos.y;
				if ( distances[y][x] < nearestDistance )
				{
					nearestDistance = distances[y][x];
					nearestEnemyId = pos.id;
				}
			}

			// if no nearest enemy found, do nothing
			if ( nearestDistance == INT_MAX )
			{
				continue;
			}
			// Resolve actual enemy position
			const entityPos* nearestEnemy = nullptr;
			for ( auto& testPos : entityPositions )
			{
				if ( testPos.id == nearestEnemyId )
				{
					nearestEnemy = &testPos;
				}
			}
			// move if no neighbouring enemy
			if ( nearestDistance > 0 )
			{
				entityPos newPos = p;
				// Work out distances from target
				std::array<std::array<int, SIZE>, SIZE> distancesBack;
				for ( auto& line : distancesBack )
				{
					line.fill( INT_MAX );
				}
				fillfrom( distancesBack, nearestEnemy->x, nearestEnemy->y, 0 );
				if ( distancesBack[p.y - 1][p.x] == nearestDistance )
				{
					--newPos.y;
				}
				else if ( distancesBack[p.y][p.x - 1] == nearestDistance )
				{
					--newPos.x;
				}
				else if ( distancesBack[p.y][p.x + 1] == nearestDistance )
				{
					++newPos.x;
				}
				else
				{
					assert( distancesBack[p.y + 1][p.x] == nearestDistance );
					++newPos.y;
				}
				entityPositions.erase( p );
				entityPositions.insert( newPos );
				--nearestDistance;
			}

			// attack if next to us
			if ( nearestDistance == 0 )
			{
				assert( nearestDistance == 0 );
				entity& enemy = entities[nearestEnemy->id];
				enemy.health -= e.attack;
				if ( enemy.health <= 0 )
				{
					if ( enemy.type == 'E' )
						elfDied = true;
					// this enemy is dead.
					entityPositions.erase( *nearestEnemy );
				}
			}
		}
		if ( !finished )
			++round;
	}
//	print_map( round );
	int totalHealth = 0;
	for ( auto e : entities )
	{
		if ( e.second.health > 0 )
		{
			totalHealth += e.second.health;
		}
	}
	printf( "For file '%s' (elf attack %d), %s, completed rounds = %d, total health = %d, result = %d\n", filename, elfAttack, elfDied ? "elves died" : "all elves survived!", round, totalHealth, round * totalHealth );
	if ( !elfDied )
		getchar();
	return elfDied;
}

void process2( const char* filename )
{
	char type = '.';
	int elfAttack = 3;
	while ( process( filename, elfAttack ) )
	{
		++elfAttack;
	}
}

int main()
{
	process( "input.txt" );

	std::array<const char*,7> files = {
		"example.txt",
		"example1.txt",
		"example2.txt",
		"example3.txt",
		"example4.txt",
		"example5.txt",
		"input.txt"
	};

	for ( auto file : files )
	{
		process2( file );
	}
}
