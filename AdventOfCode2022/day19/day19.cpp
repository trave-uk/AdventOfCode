// day19.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 19/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

bool verbose = false;

// Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.
enum Resource
{
	NONE = -1,
	ORE,
	CLAY,
	OBSIDIAN,
	GEODE,
	COUNT
};

const char* ResourceNames[] =
{
	"ore",
	"clay",
	"obsidian",
	"geode"
};

const char* ResourceDescriptions[] =
{
	"ore-collecting robot",
	"clay-collecting robot",
	"obsidian-collecting robot",
	"geode-cracking robot"
};

Resource Decode(char* name)
{
	for (int i = 0; i < COUNT; ++i)
	{
		if (strcmp(name, ResourceNames[i]) == 0)
		{
			return (Resource)i;
		}
	}
	return NONE;
}

char Identifier(Resource r)
{
	int index = (r == OBSIDIAN) ? 1 : 0;
	return toupper(ResourceNames[r][index]);
}
using Counters = std::array<int64, COUNT>;

struct Robot
{
	Robot() = default;
	Robot(char* input)
	{
		// "Each geode robot costs 2 ore and 7 obsidian."
		char gen[80], inp1[80], inp2[80];
		int m = sscanf(input, "Each %s robot costs %d %s and %d %s.", gen, &cost1, inp1, &cost2, inp2);
		assert(m == 3 || m == 5);
		generates = Decode(gen);
		input1 = Decode(inp1);
		input2 = Decode(inp2);
		assert(input1 != NONE);
	}
	
	bool CanAfford(const Counters resources) const
	{
		return (resources[input1] >= cost1 && (input2 == NONE || resources[input2] >= cost2));
	}

	void Buy(Counters& resources) const
	{
		// Well, spend the resources
		resources[input1] -= cost1;
		if (input2 != NONE)
		{
			resources[input2] -= cost2;
		}
	}


	Resource generates = NONE;
	Resource input1 = NONE;
	int cost1 = 0;
	Resource input2 = NONE;
	int cost2 = 0;

	// Calculated maximum number of this robot that could ever be needed
	int max = -1;
};

struct State
{
	State()
		: robots()
		, resources()
		, buyNext()
	{
		for (auto& robot : robots)
		{
			robot = 0;
		}
		for (auto& resource : resources)
		{
			resource = 0;
		}
	}

	void Tick()
	{
		for (int i = 0; i < COUNT; ++i)
		{
			resources[i] += robots[i];
		}
		--stepsRemaining;
	}

	int stepsRemaining = 0;
	Counters robots;
	Counters resources;
	std::bitset<COUNT> buyNext;
	std::string path;
};

struct Blueprint
{
	Blueprint() = default;
	Blueprint(char* input)
	{
		char* tok = strtok(input, ":");
		while (tok)
		{
			if (*tok == ' ')
				++tok;
			switch (*tok)
			{
			case 'B':
			{
				assert(index == 0);
				int m = sscanf(tok, "Blueprint %d", &index);
				assert(m == 1);
				break;
			}
			case 'E':
			{
				Robot r(tok);
				robots.push_back(r);
				break;
			}
			default:
			{
				assert(false);
			}
			}
			tok = strtok(nullptr, ".");
		}
		assert(robots.size() == 4);
		for (auto& r : robots)
		{
			Resource res = r.generates;
			if (res == GEODE)
				continue;
			for (auto& s : robots)
			{
				if (s.input1 == res && s.cost1 > r.max)
					r.max = s.cost1;
				if (s.input2 == res && s.cost2 > r.max)
					r.max = s.cost2;
			}
			assert(r.max > 0);
		}
	}

	void OutputDetailedPath(std::string path) const
	{
		State state;
		state.robots[ORE] = 1;
		int minute = 0;
		for (char id : path)
		{
			// == Minute 3 ==
			// Spend 2 ore to start building a clay-collecting robot.
			// 1 ore-collecting robot collects 1 ore; you now have 1 ore.
			// The new clay-collecting robot is ready; you now have 1 of them.

			++minute;
			printf("\n== Minute %d ==\n", minute);
			state.Tick();
			Resource newRobot = NONE;
			switch (id)
			{
			case 'O':
				newRobot = ORE;
				printf("Spend %d ore to start building an ore-collecting robot.\n", robots[ORE].cost1);
				break;
			case 'C':
				newRobot = CLAY;
				printf("Spend %d ore to start building a clay-collecting robot.\n", robots[CLAY].cost1);
				break;
			case 'B':
				newRobot = OBSIDIAN;
				printf("Spend %d ore and %d clay to start building an obsidian-collecting robot.\n", robots[OBSIDIAN].cost1, robots[OBSIDIAN].cost2);
				break;
			case 'G':
				newRobot = GEODE;
				printf("Spend %d ore and %d obsidian to start building a geode-cracking robot.\n", robots[GEODE].cost1, robots[GEODE].cost2);
				break;
			default:
				assert(id == '-');
				break;
			}
			if (newRobot != NONE)
			{
				robots[newRobot].Buy(state.resources);
			}
			for (int ri = 0; ri < COUNT; ++ri)
			{
				if (state.robots[ri] > 0)
				{
					printf("%lld %s%s %s%s %lld %s%s; you now have %lld %s%s%s.\n", state.robots[ri], ResourceDescriptions[ri], (state.robots[ri] == 1) ? "" : "s", (ri == GEODE) ? "crack" : "collect", (state.robots[ri] == 1) ? "s" : "", state.robots[ri], ResourceNames[ri], (state.robots[ri] == 1 || ri != GEODE) ? "" : "s", state.resources[ri], (ri == GEODE) ? "open " : "", ResourceNames[ri], (state.resources[ri] == 1 || ri != GEODE) ? "" : "s");
				}
			}
			if (newRobot != NONE)
			{
				++state.robots[newRobot];
				printf("The new %s is ready; you now have %lld of them.\n", ResourceDescriptions[newRobot], state.robots[newRobot]);
			}
		}
		printf("\n");
	}

	int64 Process(int steps = 24) const
	{
		double startTime = GetMilliseconds();
		// Find the maximum number of geodes that can be created.
		State state;
		state.robots[ORE] = 1;
		state.stepsRemaining = steps;
		std::stack<State> s;
		s.push(state);
		int64 maxGeodes = 0;
		std::string bestPath;
		while (!s.empty())
		{
			State t = s.top();
			s.pop();

			// Spent all steps?
			if (!t.stepsRemaining)
			{
				if (t.resources[GEODE] > maxGeodes)
				{
					maxGeodes = t.resources[GEODE];
					bestPath = t.path;
				}
				continue;
			}

			// Given the number of resources, we might be able to spend them to build robots. Each step, maximum one robot can be built
			// Prefer Geode robots: if a Geode robot can be built, no point building a lesser one or not building anything.
			if (robots[GEODE].CanAfford(t.resources))
			{
				State u = t;
				robots[GEODE].Buy(u.resources);   // spend resources on the geode robot
				u.Tick();                         // generate resources with current robots
				++u.robots[GEODE];                // add a geode robot
				u.buyNext = std::bitset<COUNT>(); // buy anything
				if (verbose)
					u.path.append("G");
				s.push(u);
			}
			else
			{
				std::bitset<COUNT> couldntBuy;
				for (int ri = ORE; ri <= OBSIDIAN; ++ri)
				{
					// If buyNext set, only allow those listed to be bought
					if (t.buyNext.count() && !t.buyNext.test(ri))
					{
						continue;
					}
					const Robot& r = robots[ri];
					// If there are already enough robots of this type to produce enough of this resource to build any robot, don't buy any more
					if (r.max > -1 && t.robots[ri] >= r.max)
					{
						continue;
					}
					if (r.CanAfford(t.resources))
					{
						State u = t;
						r.Buy(u.resources);               // spend resources on this robot
						u.Tick();                         // generate resources with current robots
						++u.robots[r.generates];          // add a robot of this type
						u.buyNext = std::bitset<COUNT>(); // buy anything
						if (verbose)
							u.path.append(1, Identifier(r.generates));
						s.push(u);
					}
					else
					{
						// If we couldn't buy this robot this frame, add it to the list of robots to buy next if we don't buy anything
						couldntBuy.set(ri);
					}
				}
				if (couldntBuy.count())
				{
					// We may also not buy a robot, but only if there was at least one we couldn't buy.
					// In that case, the next robot bought _must_ be one of those we couldn't buy (otherwise we may as well have bought that robot now)
					State u = t;
					u.Tick();
					u.buyNext = couldntBuy;
					if (verbose)
						u.path.append("-");
					s.push(u);
				}
			}
		}
		if (verbose)
		{
			printf("(%0.04fms) Blueprint %d: max geodes: %lld \n", GetMilliseconds() - startTime, index, maxGeodes);
			printf("Path: %s\n", bestPath.c_str());
			OutputDetailedPath(bestPath);
		}

		return maxGeodes;
	}

	int index = 0;
	std::vector<Robot> robots;
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<Blueprint> blueprints;
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				Blueprint b(thisLine);
				blueprints.push_back(b);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (const Blueprint& b : blueprints)
	{
		part1 += b.index * b.Process(24);
	}

	if (expectedPart1 != -1)
	{
		printf("%s: Part 1 expected: %lld\n", filename, expectedPart1);
	}
	printf("%s: Part 1: %lld\n", filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	// For part 2, maximum of 3 blueprints
	if (blueprints.size() > 3)
	{
		blueprints.resize(3);
	}

	part2 = 1;
	for (const Blueprint& b : blueprints)
	{
		part2 *= b.Process(32);
	}


	if (expectedPart2 != -1)
	{
		printf("%s: Part 2 expected: %lld\n", filename, expectedPart2);
	}
	printf("%s: Part 2: %lld\n", filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 33, 56*62);
	Process("input.txt");

	return 0;
}
