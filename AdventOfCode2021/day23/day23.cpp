// day23.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 23/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// Oh part 2. My solution below is tailored a bit too closely to work for part 1, but can be tweaked to have bigger rooms...

// Part 1:
// #############
// #01 2 3 4 56#
// ###e#f#g#h###
//   #a#b#c#d#
//   #########

// Part 2:
// #############
// #01 2 3 4 56#
// ###m#n#o#p###
//   #i#j#k#l#
//   #e#f#g#h#
//   #a#b#c#d#
//   #########

// 2/4 amphipods of each type, with different movement costs
//  A = 1 point, target ae
//  B = 10 points, target bf
//  C = 100 points, target cg
//  D = 1000 points, target dh

// Use A*, but not the 2D implementation in astar.h

// State:
//  Store the contents of each tile, rooms then corridor, as a string
//  Using the positions "eafbgchd0123456", or 
//  So the solution is: "AABBCCDD......."

// Valid moves:
//  room (a-h) to corridor (0-6)
//  corridor to _correct_ room
//  not passing through another amphipod

bool bPart2 = false;
std::string key1 = "eafbgchd0123456";
std::string key2 = "mieanjfbokgcplhd0123456";
std::string key = key1;

struct validMove
{
	std::string path = "";
	int64 cost = 0;
	char amphipod = 0;

	bool operator<(const validMove& rhs) const
	{
		return path < rhs.path || (path == rhs.path && cost < rhs.cost) || (path == rhs.path && cost == rhs.cost && amphipod < rhs.amphipod);
	}
};

std::map<char, std::set<validMove>> validMoves;
std::map<char, coord> posMap;

char roomToAmphipod(char room)
{
	int i = room - 'a';
	i %= 4;
	return toupper(i + 'a');
}

void PopulateValidMoves()
{
	// valid moves are from digits to letters and vice versa
	// the path includes all digits + letters passed through (which must be clear), e.g. "01ea" from 0 to a, which would have cost 4 (the Manhattan distance between 0 and a).
	validMoves.clear();

	char map[5][12] =
	{
		"01 2 3 4 56",
		"  m n o p  ",
		"  i j k l  ",
		"  e f g h  ",
		"  a b c d  ",
	};
	for (int64 y = 0; y < 5; ++y)
	{
		coord pos;
		pos.second = y;
		if (!bPart2)
		{
			if (y == 1 || y == 2)
				continue;
			else if (y > 2)
				pos.second -= 2;
		}
		for (pos.first = 0; pos.first < 11; ++pos.first)
		{
			char tile = map[y][pos.first];
			if (isalnum(tile))
			{
				posMap[tile] = pos;
			}
		}
	}

	for (char corridor = '0'; corridor <= '6'; ++corridor)
	{
		coord cpos = posMap[corridor];
		for (char room = 'a'; room <= (bPart2 ? 'p' : 'h'); ++room)
		{
			coord rpos = posMap[room];
			validMove v;
			coord distance = rpos;
			distance -= cpos;
			v.cost = abs(distance.first) + abs(distance.second);
			coord pos = cpos;
			if (!bPart2 && rpos.second > 0)
			{
				rpos.second += 2;
			}
			for (; pos.first != rpos.first; pos.first += sgn(rpos.first - cpos.first))
			{
				char tile = map[pos.second][pos.first];
				if (isdigit(tile))
				{
					v.path.append(1, tile);
				}
			}
			++pos.second;
			for (; pos.second <= rpos.second; ++pos.second)
			{
				if (!bPart2 && pos.second < 3)
					pos.second += 2;
				char tile = map[pos.second][rpos.first];
				if (isalpha(tile))
				{
					v.path.append(1, tile);
				}
			}
			v.amphipod = roomToAmphipod(room);
			validMoves[v.amphipod].insert(v);
			std::reverse(v.path.begin(), v.path.end());
			v.amphipod = 0;
			validMoves[v.amphipod].insert(v);
		}
	}
	key = bPart2 ? key2 : key1;
}

struct StateMap
{
	StateMap(std::string state)
	{
		posToAmphipod.clear();
		amphipodToPos.clear();
		assert(state.length() == key.length());
		for (int i = 0; i < key.length(); ++i)
		{
			if (state[i] == '.')
				continue;
			posToAmphipod[key[i]] = state[i];
			amphipodToPos[state[i]].push_back(key[i]);
		}
	}

	char GetAmphipodKey(char amphipod, int index)
	{
		auto keys = amphipodToPos[amphipod];
		return keys[index];
	}

	coord GetAmphipodPos(char amphipod, int index)
	{
		auto keys = amphipodToPos[amphipod];
		assert(index < keys.size());
		return posMap[keys[index]];
	}

	bool IsPathValid(const validMove& v)
	{
		for (int i = 1; i < v.path.length(); ++i)
		{
			char tile = posToAmphipod[v.path[i]];
			if (isalpha(tile))
			{
				return false;
			}
		}
		char amphipod = posToAmphipod[v.path[0]];
		if (amphipod != 0)
		{
			// Also check that amphipod moves don't go next to a different amphipod in the room, and go as deep into the room as possible
			char roomKey = v.path.back();
			while (roomKey >= 'e')
			{
				roomKey -= 4;
				char otherAmphipod = posToAmphipod[roomKey];
				if (otherAmphipod != amphipod)
				{
					return false;
				}
			}
		}
		if (v.amphipod == 0)
		{
			// Don't let amphipods that are in their final position move out unless there is a different amphipod below them.
			char roomKey = v.path.front();
			if (roomKey == tolower(amphipod))
			{
				return false;
			}
			while (roomKey >= 'e')
			{
				roomKey -= 4;
				char otherAmphipod = posToAmphipod[roomKey];
				if (otherAmphipod != amphipod)
				{
					return true;
				}
			}
			return false;

		}
		return true;
	}

	void ApplyPath(std::string path, std::string& state)
	{
		int startIndex = key.find(path[0]);
		int endIndex = key.find(path.back());
		assert(state[endIndex] == '.');
		assert(isalpha(state[startIndex]));
		state[endIndex] = state[startIndex];
		state[startIndex] = '.';
	}

	std::map<char, char> posToAmphipod;
	std::map<char, std::vector<char>> amphipodToPos;
};

// Heuristic:
//  Basically, use the sum of the shortest distance of each amphipod from its closest destination, each scaled by movement cost for that amphipod
int64 heuristic(std::string& start, std::string& goal)
{
	StateMap startMap(start);
	StateMap goalMap(goal);
	int64 cost = 0;
	int multiplier = 1;
	for (char amphipod = 'A'; amphipod <= 'D'; ++amphipod)
	{
		coord target = goalMap.GetAmphipodPos(amphipod, 1);
		for (int i = 0; i < (bPart2 ? 4 : 2); ++i)
		{
			coord a = startMap.GetAmphipodPos(amphipod, i);
			if (a == target || a == goalMap.GetAmphipodPos(amphipod, 0))
				continue;
			cost += multiplier * (a.second + target.second + abs(a.first - target.first));
		}
		multiplier *= 10;
	}
	return cost;
}

int64 aStarAmphipod(std::string& start, std::string& goal, std::vector<std::pair<std::string, int64>>* pPath = nullptr)
{
	std::set<std::string> openSet;
	openSet.insert(start);
	std::map<std::string, int64> gScore;
	gScore[start] = 0;
	std::map<std::string, int64> fScore;
	fScore[start] = heuristic(start, goal);

	// For node n, cameFrom[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	std::map<std::string, std::string> cameFrom;

	while (!openSet.empty())
	{
		int64 minF = INT_MAX;
		std::string current;
		for (const std::string& c : openSet)
		{
			if (fScore.count(c) && fScore[c] < minF)
			{
				minF = fScore[c];
				current = c;
			}
		}

		printf("%s\n", current.c_str());
		if (current == goal)
		{
			if (pPath)
			{
				pPath->clear();
				pPath->push_back(std::make_pair(current, gScore[current]));
				while (current != start)
				{
					current = cameFrom[current];
					pPath->push_back(std::make_pair(current, gScore[current]));
				}
				std::reverse(pPath->begin(), pPath->end());
			}
			return fScore[goal];
		}

		openSet.erase(current);

		StateMap currentState(current);
		int multiplier = 1;
		std::set<validMove> alwaysValid = validMoves[0];
		for (char amphipod = 'A'; amphipod <= 'D'; ++amphipod)
		{
			std::set<validMove> amphipodValid = validMoves[amphipod];
			std::set<validMove> valid = alwaysValid;
			valid.insert(amphipodValid.begin(), amphipodValid.end());
			for (int index = 0; index < (bPart2 ? 4 : 2); ++index)
			{
				char key = currentState.GetAmphipodKey(amphipod, index);
				for (validMove v : valid)
				{
					if (v.path[0] != key)
						continue;
					if (v.amphipod != 0 && v.amphipod != amphipod)
						continue;
					if (!currentState.IsPathValid(v))
						continue;

					std::string neighbour = current;
					currentState.ApplyPath(v.path, neighbour);

					// tentative_gScore is the distance from start to the neighbour through current
					int64 tentative_gScore = gScore[current] + v.cost * multiplier;
					int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
					if (tentative_gScore < neighbour_gScore)
					{
						// This path to neighbour is better than any previous one. Record it!
						cameFrom[neighbour] = current;
						gScore[neighbour] = tentative_gScore;
						fScore[neighbour] = tentative_gScore + heuristic(neighbour, goal);
						openSet.insert(neighbour);
					}
				}
			}

			multiplier *= 10;
		}
	}
	return -1; // failed to find a path
}

void Print(std::string state)
{
	assert(state.length() == 15);
	printf("#############\n");
	printf("#%c%c.%c.%c.%c.%c%c#\n", state[8],  state[9], state[10], state[11], state[12], state[13], state[14]);
	printf("###%c#%c#%c#%c###\n", state[0], state[2], state[4], state[6]);
	printf("  #%c#%c#%c#%c#  \n", state[1], state[3], state[5], state[7]);
	printf("  #########  \n");
}
//	std::vector<std::pair<std::string, int64>> path;
//	for (auto state : path)
//	{
//		Print(state.first);
//		printf("Energy used: %lld\n\n", state.second);
//	}

void Process(const char* name, std::string startState, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	bPart2 = false;
	PopulateValidMoves();
	std::string goal = bPart2 ? "AAAABBBBCCCCDDDD......." : "AABBCCDD.......";

	int64 part1 = aStarAmphipod(startState, goal);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	if (expectedPart1 != -1 && expectedPart1 != part1)
		printf("WRONG! ");
	printf("%s: Part 1: %lld\n", name, part1);


	bPart2 = true;
	PopulateValidMoves();
	goal = "AAAABBBBCCCCDDDD.......";

	std::string part2StartState = startState.substr(0, 1) + "DD" + startState.substr(1, 2) + "CB" + startState.substr(3, 2) + "BA" + startState.substr(5, 2) + "AC" + startState.substr(7);

	int64 part2 = aStarAmphipod(part2StartState, goal);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
	if (expectedPart2 != -1 && expectedPart2 != part2)
		printf("WRONG! ");
	printf("%s: Part 2: %lld\n", name, part2);

}

int main()
{
	Process("example", "BACDBCDA.......", 12521, 44169);
	Process("input", "BDACABDC.......");

	return 0;
}
