// day10.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct startype
{
	int posx = 0;
	int posy = 0;
	int velx = 0;
	int vely = 0;
	bool operator<( const startype& star1 ) const
	{
		return ( posy < star1.posy ||
				(posy == star1.posy && (posx < star1.posx ||
									    (posx == star1.posx && (velx < star1.velx ||
																(velx == star1.velx && vely < star1.vely))))) );
	}
};
std::set<startype> stars;

bool showstars()
{
	int minx = INT_MAX;
	int maxx = 0;
	int miny = INT_MAX;
	int maxy = 0;
	for ( startype star : stars )
	{
		if ( star.posx > maxx )
			maxx = star.posx;
		if ( star.posy > maxy )
			maxy = star.posy;
		if ( star.posx < minx )
			minx = star.posx;
		if ( star.posy < miny )
			miny = star.posy;
	}
	bool stop = true;

	size_t width = maxx - minx + 1;
	size_t height = maxy - miny + 1;

	printf( "size %zd x %zd\n", width, height );
	if ( width > 1000 || height > 1000 )
	{
		return false;
	}
	const int SHOWX = 160;
	const int SHOWY = 40;
	bool* starfield = nullptr;
	size_t size = sizeof(bool) * SHOWX * SHOWY;
	starfield = static_cast<bool*>(malloc( size));
	memset( starfield, 0, size );
	for ( startype star : stars )
	{
		int x = star.posx - minx;
		int y = star.posy - miny;
		if ( x < SHOWX && y < SHOWY )
			starfield[SHOWX*y + x] = true;
	}
	// show top-left of star field
	for ( size_t y = 0; y < SHOWY; ++y )
	{
		for ( size_t x = 0; x < SHOWX; ++x )
		{
			char c = (starfield[y * SHOWX + x]) ? '#' : '.';
			putchar( c );
		}
		putchar( '\n' );
	}
	return true;
}

int main()
{
	char* buffer = new char[65536];
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';

			// position=< 21518, -21209> velocity=<-2,  2>
			startype star;
			sscanf( buffer, "position=< %d, %d> velocity=<%d, %d>", &star.posx, &star.posy, &star.velx, &star.vely );
			assert( star.posx && star.posy && star.velx && star.vely );
			stars.insert( star );
		}
	}
	fclose( fp );
	delete[] buffer;

	for ( int i = 0; i < 100000; ++i )
	{
		printf( "Step %d:\n", i );
		if ( showstars() )
			getchar();
		std::set<startype> newstars;
		// update each star's position
		for ( startype star : stars )
		{
			star.posx += star.velx;
			star.posy += star.vely;
			newstars.insert( star );
		}
		stars = newstars;
	}
}

