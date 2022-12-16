// day16.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 16/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Valve
{
	Valve() = default;
	Valve(std::string input)
	{
		// "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB"
		char n[80]={0}, t[80], tunnel[80], lead[80], valve[80];
		int m = sscanf(input.c_str(), "Valve %s has flow rate=%lld; %s %s to %s %[^;]s", n, &flowRate, tunnel, lead, valve, t);
		assert(m == 6);
		name = n;
		char* tok = strtok(t, ", ");
		while (tok)
		{
			tunnels.push_back(tok);
			tok = strtok(nullptr, ", ");
		}
	}

	void Reset()
	{
		open = false;
		targeted = false;
		pressure = 0;
	}

	std::string name;
	int64 flowRate = 0;
	std::vector<std::string> tunnels;
	bool open = false;
	bool targeted = false;
	int64 pressure = 0;
};

struct Valves : public std::map<const std::string, Valve>
{
	void Reset()
	{
		for (auto& v : *this)
		{
			v.second.Reset();
		}
	}

	void Step(int steps = 1)
	{
		for (auto& pv : *this)
		{
			Valve& v = pv.second;
			if (v.open)
			{
				v.pressure += v.flowRate * steps;
			}
		}
	}

	int64 GetPressure()
	{
		int64 total = 0;
		for (auto& pv : *this)
		{
			Valve& v = pv.second;
			total += v.pressure;
		}
		return total;
	}

	std::vector<std::string> GetClosedValves(std::string exclude)
	{
		std::vector<std::string> closed;
		for (auto& pv : *this)
		{
			if (pv.first == exclude)
				continue;
			Valve& v = pv.second;
			if (!v.targeted && !v.open && v.flowRate)
			{
				closed.push_back(v.name);
			}
		}
		return closed;
	}
};

// Distance to each other valve
struct ValveConnectivity : public std::map<std::string, int>
{
	int operator[](const std::string& key) const
	{
		const auto& x = find(key);
		return x->second;
	}
};

// For each node, distances to all other valves
struct Connectivity : public std::map<std::string, ValveConnectivity>
{
	Connectivity(const Valves& valves)
	{
		for (auto& v : valves)
		{
			ValveConnectivity vc;
			std::queue<std::pair<std::string, int>> search;
			search.push(std::make_pair(v.first, 0));
			while (!search.empty())
			{
				auto top = search.front();
				search.pop();
				std::string th = top.first;
				int dist = top.second;
				auto f = valves.find(th);
				const Valve& valve = f->second;
				for (auto tun : valve.tunnels)
				{
					if (vc.count(tun) == 0 && tun != v.first)
					{
						auto np = std::make_pair(tun, dist + 1);
						vc.insert(np);
						search.push(np);
					}
				}
			}
			insert(std::make_pair(v.first, vc));
		}
	}

	const ValveConnectivity& operator[](const std::string& key) const 
	{
		const auto& x = find(key);
		return x->second;
	}
};

struct SearchState
{
	SearchState(const Valves& vs, int s, std::string v)
		: valves(vs)
		, steps(s)
		, valve(v)
	{}

	Valves valves;
	int steps;
	std::string valve;
};

// Depth-first-search, but not moving randomly. At any point it is only worth moving towards a closed valve.
// If there are no more closed valves then there are no moves required and we can skip to the end.
int64 MaximizePressure(Valves& valves)
{
	valves.Reset();
	Connectivity vc(valves);

	int steps = 30;
	std::queue<SearchState> q;
	SearchState start(valves, steps, "AA");
	q.push(start);
	int64 result = 0;
	while (!q.empty())
	{
		SearchState s = q.front();
		q.pop();

		std::vector<std::string> targets = s.valves.GetClosedValves(s.valve);

		bool did = false;
		for (const std::string& target : targets)
		{
			int distance = vc[s.valve][target];
			if (distance > s.steps)
				continue;
			did = true;
			SearchState newState = s;
			newState.valve = target;
			newState.valves.Step(distance + 1);
			newState.valves.find(target)->second.open = true;
			newState.steps -= distance + 1;
			q.push(newState);
		}

		if (!did)
		{
			s.valves.Step(s.steps);
			int64 thisResult = s.valves.GetPressure();
			result = max(result, thisResult);
			continue;
		}
	}
	return result;
}

struct Bot
{
	Bot() = default;
	Bot(std::string n)
		: node(n)
		, distanceLeft(0)
	{}
	std::string node;
	int distanceLeft;
};

struct ValveState
{
	ValveState() = default;
	ValveState(const Valve& valve)
		: open(valve.open)
		, targeted(valve.targeted)
		, pressure(valve.pressure)
	{

	}
	bool open = false;
	bool targeted = false;
	int64 pressure = 0;
};

struct ValveStates : public std::map<const std::string, ValveState>
{
	ValveStates(const Valves& vs)
		: valves(vs)
	{
		for (const auto& v : vs)
		{
			if (v.first == "AA" || v.second.flowRate > 0)
			{
				ValveState vst(v.second);
				insert(std::make_pair(v.first, vst));
			}
		}
	}

	void Step(int steps = 1)
	{
		for (auto& pv : *this)
		{
			ValveState& v = pv.second;
			const Valve& vv = valves.find(pv.first)->second;
			if (v.open)
			{
				v.pressure += vv.flowRate * steps;
			}
		}
	}

	int64 GetPressure()
	{
		int64 total = 0;
		for (auto& pv : *this)
		{
			ValveState& v = pv.second;
			total += v.pressure;
		}
		return total;
	}

	std::vector<std::string> GetClosedValves(std::string exclude)
	{
		std::vector<std::string> closed;
		for (auto& pv : *this)
		{
			if (pv.first == exclude)
				continue;
			ValveState& v = pv.second;
			const Valve& vv = valves.find(pv.first)->second;
			if (!v.targeted && !v.open && vv.flowRate)
			{
				closed.push_back(pv.first);
			}
		}
		return closed;
	}

	const Valves& valves;
};

struct SearchState2
{
	SearchState2(const Valves& vs, int s, std::string v)
		: valves(vs)
		, steps(s)
	{
		bots[0] = Bot(v);
		bots[1] = Bot(v);
	}

	ValveStates valves;
	int steps;
	std::array<Bot, 2> bots;
};

// Copy-pasted part 1 solution so that I can heavily modify it.
// Two bots moving, so it's asynchronous.
// Each bot has a target node and a target distance. Still don't care about intermediate nodes.
// When each one gets to a node, branch-choose a new one and store its distance. (Exclude targeted nodes from options)
// Each step will have at least one bot at its target.
// If both bots at target (e.g. at start), then get one to branch-choose a new target and push all these without processing
int64 MaximizePressurePart2(Valves& valves)
{
	valves.Reset();
	Connectivity vc(valves);

	std::queue<SearchState2> q;
	SearchState2 start(valves, 26, "AA");
	q.push(start);
	int64 result = 0;
	while (!q.empty())
	{
		SearchState2 s = q.front();
		q.pop();

		// get first bot with 0 distance to go and store distance of other one
		int thisBot = s.bots[0].distanceLeft == 0 ? 0 : 1;
		int otherBot = 1 - thisBot;
		Bot& b = s.bots[thisBot];
		assert(b.distanceLeft == 0);
		Bot& ob = s.bots[otherBot];
		int otherBotDist = ob.distanceLeft;

		std::vector<std::string> targets = s.valves.GetClosedValves(b.node);

		bool did = false;
		for (const std::string& target : targets)
		{
			int distance = vc[b.node][target] + 1;
			if (distance > s.steps)
				continue;
			SearchState2 newState = s;
			newState.bots[thisBot].node = target;
			newState.valves[target].targeted = true;
			newState.bots[thisBot].distanceLeft = distance;
			int thisStep = min(newState.bots[thisBot].distanceLeft, newState.bots[otherBot].distanceLeft);
			newState.bots[thisBot].distanceLeft -= thisStep;
			newState.bots[otherBot].distanceLeft -= thisStep;
			newState.valves.Step(thisStep);
			newState.steps -= thisStep;
			for (auto& nb : newState.bots)
			{
				if (nb.distanceLeft == 0)
				{
					if (valves[nb.node].flowRate > 0)
					{
						newState.valves[nb.node].open = true;
					}
				}
			}
			q.push(newState);
			did = true;
		}

		if (!did)
		{
			if (otherBotDist)
			{
				assert(otherBotDist < s.steps);
				s.valves.Step(otherBotDist);
				s.steps -= otherBotDist;
				s.valves[ob.node].open = true;
			}
			s.valves.Step(s.steps);
			int64 thisResult = s.valves.GetPressure();
			if (thisResult > result)
			{
				result = thisResult;
			}
			continue;
		}
	}
	return result;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Valves valves;
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
				// "Valve AA has flow rate=0; tunnels lead to valves DD, II, BB"
				Valve v(thisLine);
				valves.insert(std::make_pair(v.name, v));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	part1 = MaximizePressure(valves);

	if (expectedPart1 != -1)
	{
		printf("%s: Part 1 expected: %lld\n", filename, expectedPart1);
	}
	printf("%s: Part 1: %lld\n", filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	part2 = MaximizePressurePart2(valves);

	if (expectedPart2 != -1)
	{
		printf("%s: Part 2 expected: %lld\n", filename, expectedPart2);
	}
	printf("%s: Part 2: %lld\n", filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 1651, 1707);
	Process("input.txt"); // 1604 too low

	return 0;
}
