// day21.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 01/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// Weapons:    Cost  Damage  Armor
// Dagger        8     4       0
// Shortsword   10     5       0
// Warhammer    25     6       0
// Longsword    40     7       0
// Greataxe     74     8       0
// 
// Armor:      Cost  Damage  Armor
// Leather      13     0       1
// Chainmail    31     0       2
// Splintmail   53     0       3
// Bandedmail   75     0       4
// Platemail   102     0       5
// 
// Rings:      Cost  Damage  Armor
// Damage +1    25     1       0
// Damage +2    50     2       0
// Damage +3   100     3       0
// Defense +1   20     0       1
// Defense +2   40     0       2
// Defense +3   80     0       3

struct Item
{
	Item(std::string n, int64 c, int64 d, int64 a)
		: name(n)
		, cost(c)
		, damage(d)
		, armor(a)
	{}

	std::string name;
	int64 cost;
	int64 damage;
	int64 armor;
};

std::array<Item, 5> weapons =
{
	Item("Dagger", 8, 4, 0),
	Item("Shortsword", 10, 5, 0),
	Item("Warhammer", 25, 6, 0),
	Item("Longsword", 40, 7, 0),
	Item("Greataxe", 74, 8, 0)
};

std::array<Item, 5> armors =
{
	Item("Leather", 13, 0, 1),
	Item("Chainmail", 31, 0, 2),
	Item("Splintmail", 53, 0, 3),
	Item("Bandedmail", 75, 0, 4),
	Item("Platemail", 102, 0, 5)
};

std::array<Item, 6> rings =
{
	Item("Damage +1", 25, 1, 0),
	Item("Damage +2", 50, 2, 0),
	Item("Damage +3", 100, 3, 0),
	Item("Defense +1", 20, 0, 1),
	Item("Defense +2", 40, 0, 2),
	Item("Defense +3", 80, 0, 3)
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

	bool Attack(Actor& attacker)
	{
		int64 attack = attacker.damage - armor;
		hitpoints -= attack;
		return (hitpoints <= 0);
	}

	int64 hitpoints = 0;
	int64 damage = 0;
	int64 armor = 0;
};

struct Player : public Actor
{
	Player() = default;
	Player(int64 hp, int weaponIndex, int armorIndex = -1, int ring1Index = -1, int ring2Index = -1)
		: Actor(hp)
	{
		items.push_back(&weapons[weaponIndex]);
		if (armorIndex != -1)
			items.push_back(&armors[armorIndex]);
		if (ring1Index != -1)
			items.push_back(&rings[ring1Index]);
		if (ring2Index != -1)
			items.push_back(&rings[ring2Index]);
		for (Item* i : items)
		{
			cost += i->cost;
			damage += i->damage;
			armor += i->armor;
		}
	}
	std::vector<Item*> items;
	int64 cost = 0;
};

bool Fight(Actor& player, Actor& enemy)
{
	Actor p1(player), p2(enemy);
	for (;;)
	{
		if (p2.Attack(p1))
			return true;
		if (p1.Attack(p2))
			return false;
	}
}

void Process(const char* filename, int64 playerHp, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	double start = GetMilliseconds();
	int64 part1 = INT64_MAX;
	Player winningPlayer;
	int64 part2 = 0;
	Player losingPlayer;
	Actor enemy(filename);
	const int numWeapons = weapons.size();
	const int numArmors = armors.size();
	const int numRings = rings.size();
	for (int w = 0; w < numWeapons; ++w)
	{
		for (int a = -1; a < numArmors; ++a)
		{
			for (int r1 = -1; r1 < numRings; ++r1)
			{
				for (int r2 = -1; r2 < numRings; ++r2)
				{
					if (r1 <= r2 && r2 != -1)
						continue;
					Player player(playerHp, w, a, r1, r2);
					bool win = Fight(player, enemy);
					if (win)
					{
						if (player.cost < part1)
						{
							part1 = player.cost;
							winningPlayer = player;
						}
					}
					else
					{
						if (player.cost > part2)
						{
							part2 = player.cost;
							losingPlayer = player;
						}
					}
				}
			}
		}
	}

	printf("Best player:\n");
	for (Item* i : winningPlayer.items)
	{
		printf("  %s (%lld, %lld, %lld)\n", i->name.c_str(), i->cost, i->damage, i->armor);
	}
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	printf("Worst player:\n");
	for (Item* i : losingPlayer.items)
	{
		printf("  %s (%lld, %lld, %lld)\n", i->name.c_str(), i->cost, i->damage, i->armor);
	}
	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 8);
	Process("input.txt", 100);

	return 0;
}
