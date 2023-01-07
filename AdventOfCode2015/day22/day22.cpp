// day22.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 07/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// Magic Missile costs 53 mana. It instantly does 4 damage.
// Drain costs 73 mana. It instantly does 2 damage and heals you for 2 hit points.
// Shield costs 113 mana. It starts an effect that lasts for 6 turns. While it is active, your armor is increased by 7.
// Poison costs 173 mana. It starts an effect that lasts for 6 turns. At the start of each turn while it is active, it deals the boss 3 damage.
// Recharge costs 229 mana. It starts an effect that lasts for 5 turns. At the start of each turn while it is active, it gives you 101 new mana.

struct Spell
{
	Spell(std::string n, int64 c, int64 id = 0, int64 ih = 0, int64 t = 1, int64 aa = 0, int64 ad = 0, int64 am = 0)
		: name(n)
		, cost(c)
		, instantDamage(id)
		, instantHeal(ih)
		, turns(t)
		, activeArmor(aa)
		, activeDamage(ad)
		, activeMana(am)
	{}

	std::string name;
	int64 cost = 0;
	int64 instantDamage = 0;
	int64 instantHeal = 0;
	int64 turns = 1;
	int64 activeArmor = 0;
	int64 activeDamage = 0;
	int64 activeMana = 0;
};

enum SPELL
{
	NONE = -1,
	MAGIC_MISSILE,
	DRAIN,
	SHIELD,
	POISON,
	RECHARGE,
	FIRST_SPELL = MAGIC_MISSILE,
	LAST_SPELL = RECHARGE
};

std::array<Spell, 5> spellDefs =
{
	Spell("Magic Missile", 53, 4),
	Spell("Drain", 73, 2, 2),
	Spell("Shield", 113, 0, 0, 6, 7),
	Spell("Poison", 173, 0, 0, 6, 0, 3),
	Spell("Recharge", 229, 0, 0, 5, 0, 0, 101)
};

struct Actor
{
	Actor() = default;
	Actor(int64 hp)
		: hitpoints(hp)
	{}

	Actor(const char* filename)
	{
		char* buffer = new char[65536];
		FILE* fp = fopen(filename, "rt");
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (*thisLine)
				{
					char* tok = strtok(thisLine, ":");
					std::string stat(tok);
					tok = strtok(nullptr, " ");
					assert(isdigit(*tok));
					int64 value = atoll(tok);
					if (stat == "Hit Points")
					{
						// Hit Points: 12
						hitpoints = value;
					}
					else if (stat == "Damage")
					{
						// Damage: 7
						damage = value;
					}
					else
					{
						// Armor: 2
						assert(stat == "Armor");
						armor = value;
					}
				}
			}
		}
		fclose(fp);
		delete[] buffer;
	}

	bool Attack(Actor& attacker, bool verbose = false)
	{
		int64 attack = attacker.damage - armor;
		hitpoints -= attack;
		if (verbose)
		{
			printf("Boss attacks for %lld damage.\n", attack);
		}
		return (hitpoints <= 0);
	}

	int64 hitpoints = 0;
	int64 damage = 0;
	int64 armor = 0;
};

struct ActiveSpell
{
	ActiveSpell() = default;
	ActiveSpell(SPELL s, int t)
		: spell(s)
		, turns(t)
	{}

	SPELL spell = NONE;
	int turns = 0;
};

struct Player : public Actor
{
	Player() = default;
	Player(int64 hp, int64 m)
		: Actor(hp)
		, mana(m)
	{
		activeSpells.assign(0);
	}
	int64 mana = 0;
	std::array<int64, LAST_SPELL+1> activeSpells;
};

enum RESULT
{
	UNFINISHED = -1,
	BOSS_WIN = 0,
	PLAYER_WIN = 1
};

void ProcessActiveSpells(Player& player, Actor& boss, bool verbose)
{
	player.armor = 0;
	for (int i = FIRST_SPELL; i <= LAST_SPELL; ++i)
	{
		SPELL spell = SPELL(i);
		Spell& activeSpellDef = spellDefs[spell];
		int64& turns = player.activeSpells[i];
		if (turns <= 0)
			continue;
		if (activeSpellDef.activeArmor > 0)
		{
			if (verbose)
			{
				printf("%s sets %lld armor", activeSpellDef.name.c_str(), activeSpellDef.activeArmor);
			}
			player.armor += activeSpellDef.activeArmor;
		}
		if (activeSpellDef.activeDamage > 0)
		{
			if (verbose)
			{
				printf("%s deals %lld damage", activeSpellDef.name.c_str(), activeSpellDef.activeDamage);
			}
			player.damage = activeSpellDef.activeDamage;
			boss.Attack(player);
			player.damage = 0;
		}
		if (activeSpellDef.activeMana > 0)
		{
			if (verbose)
			{
				printf("%s provides %lld mana", activeSpellDef.name.c_str(), activeSpellDef.activeMana);
			}
			player.mana += activeSpellDef.activeMana;
		}
		--turns;
		if (verbose)
		{
			printf("; its timer is now %lld\n", turns);
		}
		if (turns == 0)
		{
			if (verbose)
				printf("%s wears off.\n", activeSpellDef.name.c_str());
			if (spell == SHIELD)
			{
				player.armor = 0;
			}
		}
	}
}

void OutputStats(Player& player, Actor& boss)
{
	// - Player has 10 hit points, 0 armor, 250 mana
	// - Boss has 13 hit points
	printf("- Player has %lld hit points, %lld armor, %lld mana\n", player.hitpoints, player.armor, player.mana);
	printf("- Boss has %lld hit points\n", boss.hitpoints);
}

int64 Fight(Player& player, Actor& boss, std::vector<SPELL> spells, int part, bool verbose = false)
{
	int64 cost = 0;
	for (SPELL spell : spells)
	{
		// Players turn.
		if (part == 2)
		{
			--player.hitpoints;
		}
		if (verbose)
		{
			printf("\n-- Player turn --\n");
			OutputStats(player, boss);
		}
		// active spells
		ProcessActiveSpells(player, boss, verbose);
		if (boss.hitpoints <= 0)
			break;

		// player casts spell
		Spell& spellDef = spellDefs[spell];
		if (verbose)
		{
			printf("Player casts %s", spellDef.name.c_str());
		}
		player.mana -= spellDef.cost;
		cost += spellDef.cost;
		if (spellDef.turns == 1)
		{
			if (spellDef.instantDamage > 0)
			{
				if (verbose)
				{
					printf(", dealing %lld damage", spellDef.instantDamage);
				}
				player.damage = spellDef.instantDamage;
				boss.Attack(player);
				player.damage = 0;
			}
			if (spellDef.instantHeal > 0)
			{
				if (verbose)
				{
					printf(", healing %lld health", spellDef.instantHeal); 
				}
				player.hitpoints += spellDef.instantHeal;
			}
		}
		else
		{
			player.activeSpells[spell] = spellDef.turns;
		}
		if (verbose)
		{
			printf(".\n");
		}

		if (boss.hitpoints <= 0)
		{
			if (verbose)
			{
				printf("This kills the boss, and the player wins.\n");
			}
			break;
		}

		// Boss's turn
		if (verbose)
		{
			printf("\n-- Boss turn --\n");
			OutputStats(player, boss);
		}
		ProcessActiveSpells(player, boss, verbose);
		if (boss.hitpoints <= 0)
		{
			if (verbose)
			{
				printf("This kills the boss, and the player wins.\n\n");
			}
			break;
		}

		// boss attacks
		player.Attack(boss, verbose);
		if (player.hitpoints <= 0)
		{
			if (verbose)
			{
				printf("This kills the player, who loses.\n\n");
			}
			break;
		}
	}
	return cost;
}

void Process(const char* filename, int part, int64 initialHp, int64 initialMana, int64 expectedResult = -1)
{
	double start = GetMilliseconds();
	int64 result = INT64_MAX;

	printf("\nProcessing '%s' for part %d...\n", filename, part);

	using Search = std::vector<SPELL>;
	Search best;
	std::stack<Search> stack;
	stack.push(Search());
	Actor boss1(filename);
	while (!stack.empty())
	{
		Search s = stack.top();
		stack.pop();
		Actor boss = boss1;
		Player player(initialHp, initialMana);
		int64 cost = Fight(player, boss, s, part);

		for (int i = FIRST_SPELL; i <= LAST_SPELL; ++i)
		{
			if (player.activeSpells[i] > 1)
				continue;
			Spell spellDef = spellDefs[i];
			if (spellDef.cost > player.mana)
				continue;
			SPELL spell = SPELL(i);
			Search t;
			t.push_back(spell);
			Actor boss2 = boss;
			Player player2 = player;
			int64 cost2 = cost + Fight(player2, boss2, t, part);
			if (player2.hitpoints > 0)
			{
				Search t2 = s;
				t2.push_back(spell);
				if (boss2.hitpoints > 0)
				{
					stack.push(t2);
				}
				else if (cost2 < result)
				{
					result = cost2;
					best = t2;
				}
			}
		}
	}
	assert(best.size() > 0);
	{
		Actor boss(filename);
		Player player(initialHp, initialMana);
		Fight(player, boss, best, part, true);
	}

	if (expectedResult != -1)
	{
		printf("[%.2f] %s: Part %d expected: %lld\n", GetMilliseconds() - start, filename, part, expectedResult);
	}
	printf("[%.2f] %s: Part %d: %lld\n", GetMilliseconds() - start, filename, part, result);
	assert(expectedResult == -1 || expectedResult == result);
}

int main()
{
	Process("example1.txt", 1, 10, 250, 173+53);
	Process("example2.txt", 1, 10, 250, 229+113+73+173+53);
	Process("input.txt", 1, 50, 500);
	Process("input.txt", 2, 50, 500);

	return 0;
}
