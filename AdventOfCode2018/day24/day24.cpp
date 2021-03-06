// day24.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// 47 units each with 4241 hit points (weak to slashing, cold; immune to radiation) with an attack that does 889 cold damage at initiative 10
class group
{
public:
	int units;	// 47
	int unitHP;	// 4241
	std::set<std::string> weaknesses; // [ "slashing","cold" ]
	std::set<std::string> immunities; // [ "radiation" ]
	int attackDamage; // 889
	std::string attackType; // "cold"
	int initiative;	// 10
	int target = -1;
	int effectivePower() const { return units * attackDamage; }
	int calculatePower( const group& defender )
	{
		int power = effectivePower();
		if ( defender.immunities.count( attackType ) > 0 )
		{
			// make this attack 0
			power = 0;
		}
		if ( defender.weaknesses.count( attackType ) > 0 )
		{
			power *= 2;
		}
		return power;
	}
};

std::vector<group> ImmuneSystem;
std::vector<group> Infection;

bool verbose = false;

void removeGroups( std::vector<int>& deaths, std::vector<group>& army )
{
	// sort death vector so that we start removing the later groups first; otherwise the indices will go out of sync
	std::sort( deaths.begin(), deaths.end() );
	while ( deaths.size() > 0 )
	{
		int index = deaths.back();
		deaths.pop_back();
		army.erase( army.begin() + index );
	}
}

void attackPhase()
{
	std::vector<int> immuneOrder;
	for ( int i = 0; i < ImmuneSystem.size(); ++i )
		immuneOrder.push_back( i );
	// sort using a lambda expression 
	std::sort( immuneOrder.begin(), immuneOrder.end(), []( int a, int b ) {
		return ImmuneSystem[a].initiative > ImmuneSystem[b].initiative;
	} );

	std::vector<int> infectionOrder;
	for ( int i = 0; i < Infection.size(); ++i )
		infectionOrder.push_back( i );
	// sort using a lambda expression 
	std::sort( infectionOrder.begin(), infectionOrder.end(), []( int a, int b ) {
		return Infection[a].initiative > Infection[b].initiative;
	} );

	std::vector<int> immuneDeaths;
	std::vector<int> infectionDeaths;

	while ( immuneOrder.size() > 0 || infectionOrder.size() > 0 )
	{
		int immuneIndex = immuneOrder.size() == 0 ? -1 : immuneOrder[0];
		group* immuneGroup = immuneOrder.size() == 0 ? nullptr : &ImmuneSystem[immuneIndex];
		int infectionIndex = infectionOrder.size() == 0 ? -1 : infectionOrder[0];
		group* infectionGroup = infectionOrder.size() == 0 ? nullptr : &Infection[infectionIndex];
		group* attackingGroup = nullptr;
		group* targetGroup = nullptr;
		int attackingIndex = -1;
		int targetIndex = -1;
		const char* attackingArmy;
		if ( !infectionGroup || (immuneGroup && immuneGroup->initiative > infectionGroup->initiative) )
		{
			// immuneGroup's turn
			infectionGroup = nullptr;
			immuneOrder.erase( immuneOrder.begin() );
			if ( immuneGroup->target == -1 )
				continue;
			attackingGroup = immuneGroup;
			targetIndex = immuneGroup->target;
			targetGroup = &Infection[targetIndex];
			attackingArmy = "Immune System";
			attackingIndex = immuneIndex;
		}
		else
		{
			assert( !immuneGroup || (infectionGroup && infectionGroup->initiative > immuneGroup->initiative) );
			immuneGroup = nullptr;
			// infectionGroup's turn
			infectionOrder.erase( infectionOrder.begin() );
			if ( infectionGroup->target == -1 )
				continue;
			attackingGroup = infectionGroup;
			targetIndex = infectionGroup->target;
			targetGroup = &ImmuneSystem[targetIndex];
			attackingArmy = "Infection";
			attackingIndex = infectionIndex;
		}
		assert( attackingGroup && targetGroup );
		int power = attackingGroup->calculatePower( *targetGroup );
		int victims = power / targetGroup->unitHP;
		victims = std::min( victims, targetGroup->units );
		if ( verbose )
		{
			printf( "%s group %d attacks defending group %d, killing %d units\n", attackingArmy, attackingIndex + 1, targetIndex + 1, victims );
		}
		targetGroup->units -= victims;
		if ( targetGroup->units <= 0 )
		{
			// remove the target from the list of indices, and add to the list of deaths 
			if ( immuneGroup )
			{
				auto unit = std::find( infectionOrder.begin(), infectionOrder.end(), targetIndex );
				if ( unit != infectionOrder.end() )
					infectionOrder.erase( unit );
				infectionDeaths.push_back( immuneGroup->target );
			}
			else
			{
				auto unit = std::find( immuneOrder.begin(), immuneOrder.end(), targetIndex );
				if ( unit != immuneOrder.end() )
					immuneOrder.erase( unit );
				immuneDeaths.push_back( infectionGroup->target );
			}
		}
	}

	// TODO Now finished processing, remove the dead groups
	removeGroups( immuneDeaths, ImmuneSystem );
	removeGroups( infectionDeaths, Infection );
}

void selectTargets( std::vector<group>& attackers, const std::vector<group>& defenders, const char* armyName )
{
	std::vector<int> attackerOrder;
	std::set<int> defenderAvailable;
	// First, sort the attackers by effective power (units * attackDamage), and then (if equal) initiative
	for ( int i = 0; i<attackers.size(); ++i )
		attackerOrder.push_back( i );
	for ( int i = 0; i < defenders.size(); ++i )
		defenderAvailable.insert( i );
	// sort using a lambda expression 
	std::sort( attackerOrder.begin(), attackerOrder.end(), [attackers]( int a, int b ) {
		int a1 = attackers[a].effectivePower();
		int b1 = attackers[b].effectivePower();
		if ( a1 == b1 )
			return attackers[a].initiative > attackers[b].initiative;
		return a1 > b1;
	} );
	for ( int index : attackerOrder )
	{
		group& attacker = attackers[index];
		int chosenDefender = -1;
		int attackPower = -1;
		for ( int defenderIndex : defenderAvailable )
		{
			const group& defender = defenders[defenderIndex];
			int thisAttack = attacker.calculatePower( defender );
			if ( thisAttack == 0 )
			{
				// Ignore defenders immune to our attack (not clear if should do this)
				continue;
			}
			if ( verbose )
			{
				printf( "%s group %d would deal defending group %d %d damage\n", armyName, index + 1, defenderIndex + 1, thisAttack );
			}
			bool useThis = (thisAttack > attackPower);
			if ( thisAttack == attackPower )
			{
				// In the event of a tie, target the defending group with the largest effective power; if there is still a tie, choose the defending group with the highest initiative
				const group& currentDefender = defenders[chosenDefender];
				useThis = (defender.effectivePower() > currentDefender.effectivePower());
				if ( defender.effectivePower() == currentDefender.effectivePower() )
					useThis = defender.initiative > currentDefender.initiative;
			}
			if ( useThis )
			{
				attackPower = thisAttack;
				chosenDefender = defenderIndex;
			}
		}
		attacker.target = chosenDefender;
		if ( chosenDefender != -1 )
		{
			defenderAvailable.erase( chosenDefender );
		}
	}
}

int countUnits( const std::vector<group>& army )
{
	int result = 0;
	for ( group g : army )
	{
		assert( g.units > 0 );
		result += g.units;
	}
	return result;
}

int process()
{
	int totalUnits = countUnits( ImmuneSystem ) + countUnits( Infection );
	while ( ImmuneSystem.size() > 0 && Infection.size() > 0 )
	{
		if ( verbose )
		{
			printf( "Immune System:\n" );
			int index = 1;
			for ( auto g : ImmuneSystem )
			{
				printf( "Group %d contains %d units\n", index++, g.units );
			}
			printf( "Infection:\n" );
			index = 1;
			for ( auto g : Infection )
			{
				printf( "Group %d contains %d units\n", index++, g.units );
			}
			printf( "\n" );
		}
		// Target selection
		selectTargets( Infection, ImmuneSystem, "Infection" );
		selectTargets( ImmuneSystem, Infection, "Immune System" );
		if ( verbose )
		{
			printf( "\n" );
		}

		// Attack phase
		attackPhase();
		if ( verbose )
		{
			printf( "\n" );
			getchar();
		}
		int newTotal = countUnits( ImmuneSystem ) + countUnits( Infection );
		if ( newTotal == totalUnits )
		{
			return -1;
		}
		totalUnits = newTotal;
	}
	printf( "Finished with %d immune system units and %d infection units\n", countUnits( ImmuneSystem ), countUnits( Infection ) );
	return countUnits( ImmuneSystem );
}

bool load( const char *filename, int boost = 0 )
{
	char* buffer = new char[65536];
	FILE *fp = fopen( filename, "rt" );
	if ( !fp )
		return false;
	std::vector<group>* army = nullptr;
	int armyBoost = 0;
	ImmuneSystem.clear();
	Infection.clear();
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			if ( *thisLine )
			{
				// Process each line...
				if ( strcmp( thisLine, "Immune System:" ) == 0 )
				{
					army = &ImmuneSystem;
					armyBoost = boost;
					continue;
				}
				if ( strcmp( thisLine, "Infection:" ) == 0 )
				{
					army = &Infection;
					armyBoost = 0;
					continue;
				}
				assert( army );
				// 47 units each with 4241 hit points (weak to slashing, cold; immune to radiation) with an attack that does 889 cold damage at initiative 10
				group g;
				char type[20];
				sscanf( thisLine, "%d units each with %d hit points", &g.units, &g.unitHP );
				char* part2 = strstr( thisLine, "with an" );
				sscanf( part2, "with an attack that does %d %s damage at initiative %d", &g.attackDamage, type, &g.initiative );
				g.attackType = type;
				g.attackDamage += armyBoost;
				// parse the (optional) flags
				if ( strcspn( thisLine, "(" ) < strlen( thisLine ) )
				{
					strtok( thisLine, "(" );
					char *flags = strtok( nullptr, ")" );
					// Now flags contains the text within brackets (e.g. "weak to slashing, cold; immune to radiation")
					char *parts[2];
					parts[0] = strtok( flags, ";" );
					parts[1] = strtok( nullptr, ";" );
					for ( int i = 0; i < 2; ++i )
					{
						char *part = parts[i];
						if ( !part )
							continue;
						if ( *part == ' ' )
							part++;
						if ( strncmp( part, "weak to ", 8 ) == 0 )
						{
							part += 8;
							char* type = strtok( part, ", " );
							while ( type )
							{
								std::string typestring = type;
								g.weaknesses.insert( typestring );
								type = strtok( nullptr, ", " );
							}
						}
						else if ( strncmp( part, "immune to ", 10 ) == 0 )
						{
							part += 10;
							char* type = strtok( part, ", " );
							while ( type )
							{
								std::string typestring = type;
								g.immunities.insert( typestring );
								type = strtok( nullptr, ", " );
							}
						}
						else
						{
							assert( false );
						}
					}

				}
				army->push_back( g );
			}
		}
	}
	fclose( fp );
	delete[] buffer;
	return true;
}

int run_boosted( const char* filename, int boost )
{
	if ( load( filename, boost ) )
	{
		return process();
	}
	return 0;
}

int main()
{
//	const char* filename = "example.txt";
	const char* filename = "input.txt";
	int boost = 0;
	// Binary search for correct boost.
	// First, go up in steps of 1024 until a successful boost found.
	int remaining = 0;
	int low = 0;
	int high = 0;
	while ( remaining == 0 )
	{
		remaining = run_boosted( filename, boost );
		printf( "%d remaining immune system units with boost of %d\n", remaining, boost );
		if ( remaining > 0 )
		{
			high = boost;
		}
		else
		{
			low = boost;
			boost += 1024;
		}
	}
	
	// Now, try halfway between high and low, replacing high or low each time
	while ( high - low > 1 )
	{
		boost = (high + low) / 2;
		remaining = run_boosted( filename, boost );
		printf( "%d remaining immune system units with boost of %d\n", remaining, boost );
		if ( remaining == -1 )
		{
			printf( "Deadlock occurred with boost of %d\n", boost );
			for ( boost = low; boost <= high; ++boost )
			{
				remaining = run_boosted( filename, boost );
				printf( "%d remaining immune system units with boost of %d\n", remaining, boost );
				if ( remaining > 0 )
				{
					high = boost;
					break;
				}
			}
			break;
		}
		else
		{
			if ( remaining > 0 )
				high = boost;
			else
				low = boost;
		}
	}
	remaining = run_boosted( filename, high );
	printf( "%d remaining immune system units with boost of %d\n", remaining, high );
	getchar();
	return 0;
}
// Part 1:
// 23376 too low (but example hadn't worked the same so not surprised)
// 23385 correct (recalculate power as casualties effect it)