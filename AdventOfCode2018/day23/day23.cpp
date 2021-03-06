// day23.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct nanobot
{
	int x;
	int y;
	int z;
	int r;
};

std::vector<nanobot> bots;
int biggest_bot = -1;
int minx = INT_MAX;
int miny = INT_MAX;
int minz = INT_MAX;
int maxx = 0;
int maxy = 0;
int maxz = 0;

bool load( const char *filename )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	int biggest_radius = 0;
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				// Process each line...
				nanobot n;
				sscanf( thisLine, "pos=<%d,%d,%d>, r=%d", &n.x, &n.y, &n.z, &n.r );
				if ( n.r > biggest_radius )
				{
					biggest_radius = n.r;
					biggest_bot = bots.size();
				}
				if ( n.x < minx )
					minx = n.x;
				if ( n.x > maxx )
					maxx = n.x;
				if ( n.y < miny )
					miny = n.y;
				if ( n.y > maxy )
					maxy = n.y;
				if ( n.z < minz )
					minz = n.z;
				if ( n.z > maxz )
					maxz = n.z;

				bots.push_back( n );
			}
		}
	}
	return true;
}

struct pos_t
{
	pos_t() = default;
	pos_t( int _x, int _y, int _z ) : x( _x ), y( _y ), z( _z ) {}
	int x;
	int y;
	int z;
	bool operator<( const pos_t& other ) const
	{
		if ( x < other.x )
			return true;
		if ( x == other.x && y < other.y )
			return true;
		if ( x == other.x && y == other.y && z < other.z )
			return true;
		return false;
	}
};

int distance( const pos_t& pos )
{
	return abs( pos.x ) + abs( pos.y ) + abs( pos.z );
}

bool compare( int x, int y, int z, const nanobot& b )
{
	int distance = abs( b.x - x ) + abs( b.y - y ) + abs( b.z - z );
	return (distance <= b.r);
}

int count_bots( const pos_t& pos )
{
	int count = 0;
	for ( const auto& bot : bots )
	{
		if ( compare( pos.x, pos.y, pos.z, bot ) )
			++count;
	}
	return count;
}

int find_bots( std::vector<nanobot>& output, const pos_t& pos )
{
	int count = 0;
	for ( const auto& bot : bots )
	{
		if ( compare( pos.x, pos.y, pos.z, bot ) )
		{
			output.push_back( bot );
			++count;
		}
	}
	return count;
}

void insert_points( std::set<pos_t>& points, const pos_t& centre, int dx, int dy, int dz )
{
	pos_t pos1( centre.x - dx, centre.y - dy, centre.z - dz );
	points.insert( pos1 );
	pos_t pos2( centre.x - dx, centre.y + dy, centre.z - dz );
	points.insert( pos2 );
	pos_t pos3( centre.x - dx, centre.y - dy, centre.z + dz );
	points.insert( pos3 );
	pos_t pos4( centre.x - dx, centre.y + dy, centre.z + dz );
	points.insert( pos4 );
	pos_t pos5( centre.x + dx, centre.y - dy, centre.z - dz );
	points.insert( pos5 );
	pos_t pos6( centre.x + dx, centre.y + dy, centre.z - dz );
	points.insert( pos6 );
	pos_t pos7( centre.x + dx, centre.y - dy, centre.z + dz );
	points.insert( pos7 );
	pos_t pos8( centre.x + dx, centre.y + dy, centre.z + dz );
	points.insert( pos8 );
}

void get_points( std::set<pos_t>& points, const pos_t& centre, int radius )
{
	// TODO: iterate all positions with Manhattan distance radius from cx,cy,cz
	for ( int dx = 0; dx <= radius; ++dx )
	{
		for ( int dy = 0; dy <= radius - dx; ++dy )
		{
			int dz = radius - dx - dy;
			insert_points( points, centre, dx, dy, dz );
			insert_points( points, centre, dx, dz, dy );
			insert_points( points, centre, dy, dx, dz );
			insert_points( points, centre, dy, dz, dx );
			insert_points( points, centre, dz, dx, dy );
			insert_points( points, centre, dz, dy, dx );
		}
	}
}

void find_closest2()
{
	// start at the average position of all the bots, and circle out
	double px = 0.0, py = 0.0, pz = 0.0;
	for ( auto bot : bots )
	{
		px += bot.x;
		py += bot.y;
		pz += bot.z;
	}
	int count = bots.size();
	px /= count;
	py /= count;
	pz /= count;
	int cx = px;
	int cy = py;
	int cz = pz;
	printf( "Centre at (%d,%d,%d)\n", cx, cy, cz );
	int max_count = 0;
	int min_distance = 0;
	pos_t nearest;

//	pos_t centre( cx, cy, cz );
//	pos_t centre( 0, 0, 0 );
	pos_t centre( 59209531, 49468046, 11128627 );	// result of first attempt
	printf( "Using centre (%d,%d,%d)\n", centre.x, centre.y, centre.z );
	int step = 8192;
	int start = 0;
	int end = 16384;
	for ( int radius = start; radius < 16384; radius += step )
	{
		std::set<pos_t> points;
		get_points( points, centre, radius );
		for ( auto pos : points )
		{
			int count = count_bots( pos );
			if ( count >= max_count )
			{
				int dist = distance(pos);
				if ( count > max_count )
				{
					max_count = count;
					min_distance = dist;
					nearest = pos;
				}
				else if ( count == max_count && dist < min_distance )
				{
					min_distance = dist;
					nearest = pos;
				}
			}
		}
		printf( "Closest point at radius %d from centre (seeing %d bots) is distance %d away\n", radius, max_count, min_distance );
	}
}

void find_closest(pos_t& point)
{
	int max_count = 0;
	int min_distance = 0;
	int step = 1000000;
	while ( step > 0 )
	{
		for ( int x = minx; x <= maxx; x += step )
		{
			for ( int y = miny; y <= maxy; y += step )
			{
				for ( int z = minz; z <= maxz; z += step )
				{
					pos_t pos( x, y, z );
					int count = count_bots( pos );
					if ( count >= max_count )
					{
						int dist = distance(pos); //abs( x ) + abs( y ) + abs( z );
						if ( count > max_count )
						{
							max_count = count;
							min_distance = dist;
							point = pos;
						}
						else if ( count == max_count && dist < min_distance )
						{
							min_distance = dist;
							point = pos;
						}
					}
				}
			}
		}
		// Narrow search around found spot
		printf( "Closest point at step %d (to see %d bots) is distance %d away\n", step, max_count, min_distance );
		minx = std::max( minx, point.x - step );
		maxx = std::min( maxx, point.x + step );
		miny = std::max( miny, point.y - step );
		maxy = std::min( maxy, point.y + step );
		minz = std::max( minz, point.z - step );
		maxz = std::min( maxz, point.z + step );
		step /= 10;
	}
	printf( "Closest point (to see %d bots) is distance %d away\n", max_count, min_distance );
}

template <typename T> int sgn( T val ) {
	return (T( 0 ) < val) - (val < T( 0 ));
}

int check_neighbours( const pos_t& pos )
{
	std::queue<pos_t> neighbours;
	neighbours.push( pos );
	std::set<pos_t> seen;

	int best = 0;
	int dist = INT_MAX;
	while ( !neighbours.empty() )
	{
		pos_t pos = neighbours.front();
		neighbours.pop();

		int score = count_bots( pos );
		if ( score < best )
			continue;
		int thisdist = distance( pos );
		if ( score > best )
		{
			best = score;
			dist = thisdist;
		}
		else if ( thisdist < dist )
		{
			dist = thisdist;
		}
		pos_t newpos = pos;
		newpos.x = pos.x - sgn( pos.x );
		if ( seen.count( newpos ) == 0 )
		{
			neighbours.push( newpos );
			seen.insert( newpos );
		}
		newpos = pos;
		newpos.y = pos.y - sgn( pos.y );
		if ( seen.count( newpos ) == 0 )
		{
			neighbours.push( newpos );
			seen.insert( newpos );
		}
		newpos = pos;
		newpos.z = pos.z - sgn( pos.z );
		if ( seen.count( newpos ) == 0 )
		{
			neighbours.push( newpos );
			seen.insert( newpos );
		}
	}
	printf( "Most bots = %d; min dist = %d\n", best, dist );
	return dist;
}

void find_closest3()
{
	pos_t start( 59209531, 49468046, 11128627 );
	check_neighbours( start );
}

void find_closest4()
{
	// OK, I have a case of 923 overlapping bots. For each of these bots, get the Manhattan distance from the origin ( |x| + |y| + |z| - r ). The largest of these values is the answer.
//	pos_t pos( 59209531, 49468046, 11128627 );
	pos_t pos;
	find_closest( pos );
	std::vector<nanobot> bots;
	int count = find_bots( bots, pos );
	int maxdist = 0;
	for ( auto bot : bots )
	{
		int dist = abs( bot.x ) + abs( bot.y ) + abs( bot.z ) - bot.r;
		if ( dist > maxdist )
		{
			maxdist = dist;
		}
	}
	printf( "Furthest point within influence of %d bots is at distance %d\n", count, maxdist );
}

void process()
{
	auto c = bots[biggest_bot];
	int bot_count = 0;
	for ( auto b : bots )
	{
		if ( compare( b.x, b.y, b.z, c ) )
		{
			++bot_count;
		}
	}
	printf( "%d bots are within the radius of bot %d\n", bot_count, biggest_bot );
	int dx = maxx - minx + 1;
	int dy = maxy - miny + 1;
	int dz = maxz - minz + 1;
	printf( "bot cloud is within volume (%d,%d,%d) to (%d,%d,%d).\nThis has the size (%d,%d,%d)\n", minx, miny, minz, maxx, maxy, maxz, dx, dy, dz );
//	pos_t pos;
//	find_closest( pos );
//	find_closest2();
//	find_closest3();
	find_closest4();
}

int main()
{
//	if ( load( "example2.txt" ) )
	if ( load( "input.txt" ) )
	{
		process();
		printf( "Press return to continue...\n" );
		getchar();
	}
	return 0;
}

// 119806204 too high (923 bots at 59209531, 49468046, 11128627, using find_closest().)
// 119188816 was correct (using previous point in find_closest4().)