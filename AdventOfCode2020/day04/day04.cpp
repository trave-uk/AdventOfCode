// day4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct pp
{
	std::string byr;	// ( Birth Year )
	std::string iyr;	// ( Issue Year )
	std::string eyr;	// ( Expiration Year )
	std::string hgt;	// ( Height )
	std::string hcl;	// ( Hair Color )
	std::string ecl;	// ( Eye Color )
	std::string pid;	// ( Passport ID )
	std::string cid;	// ( Country ID )
	bool valid = true;
};

int main()
{
	char line[80];
	int validCount1 = 0;
	int validCount2 = 0;
	struct pp p;
	FILE *fp = fopen( "input.txt", "rt" );
//	FILE *fp = fopen( "example.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( line, 80, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine == '\0' )
			{
				// next entry
				if (
					!p.byr.empty()
					&& !p.iyr.empty()
					&& !p.eyr.empty()
					&& !p.hgt.empty()
					&& !p.hcl.empty()
					&& !p.ecl.empty()
					&& !p.pid.empty()
//					&& !p.cid.empty()
					)
				{
					validCount1++;
					if ( p.valid )
					{
						validCount2++;
					}
				}
				p = pp();
			}
			else
			{
				std::set<std::string> pairs;
				char *pair = strtok( thisLine, " " );
				do
				{
					pairs.insert( pair );
				} while ( pair = strtok( nullptr, " " ) );
				for ( std::string pr : pairs )
				{
					char* x = const_cast<char*>( pr.c_str() );
					char* field = strtok( x, ":" );
					char* value = strtok( nullptr, ":" );
					if ( strcmp( field, "byr" ) == 0 )
					{
						p.byr = std::string( value );
						int year = atoi( value );
						if ( year < 1920 || year > 2002 )
							p.valid = false;
					}
					else if ( strcmp( field, "iyr" ) == 0 )
					{
						p.iyr = std::string( value );
						int year = atoi( value );
						if ( year < 2010 || year > 2020 )
							p.valid = false;
					}
					else if ( strcmp( field, "eyr" ) == 0 )
					{
						p.eyr = std::string( value );
						int year = atoi( value );
						if ( year < 2020 || year > 2030 )
							p.valid = false;
					}
					else if ( strcmp( field, "hgt" ) == 0 )
					{
						p.hgt = std::string( value );
						int val;
						char units[3];
						sscanf( value, "%d%s", &val, units );
						if ( strcmp( units, "cm" ) == 0 )
						{
							if ( val < 150 || val > 193 )
								p.valid = false;
						}
						else if ( strcmp( units, "in" ) == 0 )
						{
							if ( val < 59 || val > 76 )
								p.valid = false;
						}
					}
					else if ( strcmp( field, "hcl" ) == 0 )
					{
						p.hcl = std::string( value );
						if ( strlen( value ) != 7 || value[0] != '#' )
							p.valid = false;
						else
						{
							for ( int i = 1; i <= 6; ++i )
							{
								char c = value[i];
								if ( !isdigit( c ) && c < 'a' && c > 'f' )
								{
									p.valid = false;
								}
							}
						}
					}
					else if ( strcmp( field, "ecl" ) == 0 )
					{
						p.ecl = std::string( value );
						// amb blu brn gry grn hzl oth
						if ( strcmp( value, "amb" ) != 0
							&& strcmp( value, "blu" ) != 0
							&& strcmp( value, "brn" ) != 0
							&& strcmp( value, "gry" ) != 0
							&& strcmp( value, "grn" ) != 0
							&& strcmp( value, "hzl" ) != 0
							&& strcmp( value, "oth" ) != 0
							)
						{
							p.valid = false;
						}
					}
					else if ( strcmp( field, "pid" ) == 0 )
					{
						p.pid = std::string( value );
						if ( strlen( value ) != 9 )
							p.valid = false;
						else
						{
							for ( int i = 0; i <= 8; ++i )
							{
								char c = value[i];
								if ( !isdigit( c ) )
								{
									p.valid = false;
								}
							}
						}
					}
					else if ( strcmp( field, "cid" ) == 0 )
					{
						p.cid = std::string( value );
					}
					else
					{
						printf( "Unknown field: %s\n", field );
					}
				}
			}
		}
	}
	// last entry
	if ( !p.byr.empty()
		&& !p.iyr.empty()
		&& !p.eyr.empty()
		&& !p.hgt.empty()
		&& !p.hcl.empty()
		&& !p.ecl.empty()
		&& !p.pid.empty()
//		&& !p.cid.empty()
		)
	{
		if ( p.valid )
			validCount2++;
		validCount1++;
	}
	fclose( fp );

	printf( "Part 1: There are %d valid passports\n", validCount1 );
	printf( "Part 2: There are %d valid passports\n", validCount2 );
	return 0;
}

