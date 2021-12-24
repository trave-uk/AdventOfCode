// day23.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 23/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// #############
// #01 2 3 4 56#
// ###m#n#o#p###
//   #i#j#k#l#
//   #e#f#g#h#
//   #a#b#c#d#
//   #########

// 2 amphipods of each type, with different movement costs
//  A = 1 point, target ae
//  B = 10 points, target bf
//  C = 100 points, target cg
//  D = 1000 points, target dh

// Use A*, but not the 2D implementation in astar.h

// State:
//  Store the contents of each tile, rooms then corridor, as a string
//  Using the positions "mieanjfbokgcplhd0123456"
//  So the solution is: "AAAABBBBCCCCDDDD......."

// Valid moves:
//  room (a-p) to corridor (0-6)
//  corridor to _correct_ room
//  not passing through another amphipod

std::string key = "mieanjfbokgcplhd0123456";

struct validMove
{
	validMove(std::string _path = "", int _cost = 0, char _amphipod = 0)
		: path(_path)
		, cost(_cost)
		, amphipod(_amphipod)
	{}
	std::string path;
	int64 cost;
	char amphipod;

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
	return i + 'A';
}

void PopulateValidMoves()
{
	// valid moves are from digits to letters and vice versa
	// the path includes all digits + letters passed through (which must be clear), e.g. "01ea" from 0 to a, which would have cost 4 (the Manhattan distance between 0 and a).
	// 
	char map[5][12] =
	{
		"01 2 3 4 56",
		"  m n o p  ",
		"  i j k l  ",
		"  e f g h  ",
		"  a b c d  ",
	};
	coord pos;
	for (pos.second = 0; pos.second < 5; ++pos.second)
	{
		for (pos.first = 0; pos.first < 11; ++pos.first)
		{
			char tile = map[pos.second][pos.first];
			if (isalnum(tile))
			{
				posMap[tile] = pos;
			}
		}
	}

	for (char corridor = '0'; corridor <= '6'; ++corridor)
	{
		coord cpos = posMap[corridor];
		for (char room = 'a'; room <= 'p'; ++room)
		{
			coord rpos = posMap[room];
			validMove v;
			coord distance = rpos;
			distance -= cpos;
			v.cost = abs(distance.first) + abs(distance.second);
			pos = cpos;
			for (; pos.first != rpos.first; pos.first += sgn(rpos.first - cpos.first))
			{
				char tile = map[pos.second][pos.first];
				if (tile != ' ')
				{
					v.path.append(1, tile);
				}
			}
			if (rpos.second >= 2)
			{
				char tile = map[1][rpos.first];
				v.path.append(1, tile);
			}
			if (rpos.second >= 3)
			{
				char tile = map[2][rpos.first];
				v.path.append(1, tile);
			}
			if (rpos.second == 4)
			{
				char tile = map[3][rpos.first];
				v.path.append(1, tile);
			}
			v.path.append(1, room);
			v.amphipod = roomToAmphipod(room);
			validMoves[v.amphipod].insert(v);
			std::reverse(v.path.begin(), v.path.end());
			v.amphipod = 0;
			validMoves[v.amphipod].insert(v);
		}
	}
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
		if (v.amphipod != 0)
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
			char roomAmphipod = roomToAmphipod(amphipod);
			if (roomAmphipod != amphipod)
				return true;
			char roomKey = v.path.front();
			if (roomKey == tolower(amphipod))
				return false;
			if (roomKey <= 'd')
				return true;
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

	void ApplyPath(const validMove& v, std::string& state)
	{
		assert(IsPathValid(v));
		int startIndex = key.find(v.path[0]);
		int endIndex = key.find(v.path.back());
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
		for (int i = 0; i < 4; ++i)
		{
			coord target = goalMap.GetAmphipodPos(amphipod, i);
			coord a = startMap.GetAmphipodPos(amphipod, i);
			if (a == goalMap.GetAmphipodPos(amphipod, 0) || a == goalMap.GetAmphipodPos(amphipod, 1) || a == goalMap.GetAmphipodPos(amphipod, 2) || a == goalMap.GetAmphipodPos(amphipod, 3))
				continue;
			cost += multiplier * (a.second + target.second + abs(a.first - target.first));
		}
		multiplier *= 10;
	}
	return cost;
}

void Print(std::string state)
{
	assert(state.length() == 23);
	printf("#############\n");
	printf("#%c%c.%c.%c.%c.%c%c#\n", state[16], state[17], state[18], state[19], state[20], state[21], state[22]);
	printf("###%c#%c#%c#%c###\n", state[0], state[4], state[8], state[12]);
	printf("  #%c#%c#%c#%c#  \n", state[1], state[5], state[9], state[13]);
	printf("  #%c#%c#%c#%c#  \n", state[2], state[6], state[10], state[14]);
	printf("  #%c#%c#%c#%c#  \n", state[3], state[7], state[11], state[15]);
	printf("  #########  \n");
}

std::vector<std::string> exampleSolution;

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
			for (int index = 0; index < 4; ++index)
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
					currentState.ApplyPath(v, neighbour);

					// tentative_gScore is the distance from start to the neighbour through current
					int64 tentative_gScore = gScore[current] + v.cost * multiplier;
					int64 neighbour_gScore = (gScore.count(neighbour) ? gScore[neighbour] : INT_MAX);
					static bool bDebugCancel = false;
					if (tentative_gScore < neighbour_gScore && !bDebugCancel)
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

void Process(const char* name, std::string startState, int64 expectedPart1 = -1)
{
	PopulateValidMoves();
	std::string goal = "AAAABBBBCCCCDDDD.......";

	Print(startState);
	std::vector<std::pair<std::string, int64>> path;
	int64 part1 = aStarAmphipod(startState, goal, &path);
//	assert(expectedPart1 == -1 || expectedPart1 == part1);
	if (expectedPart1 != -1 && expectedPart1 != part1)
		printf("WRONG! ");
	printf("%s: Part 2: %lld\n", name, part1);
//	for (auto state : path)
//	{
//		Print(state.first);
//		printf("Energy used: %lld\n\n", state.second);
//	}
}

void Load(const char *filename, std::vector<std::string>& solution)
{
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	std::vector<std::string> input;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				input.push_back(thisLine);
			}
			else if (!input.empty())
			{
				assert(input.size() == 7);
				// #############
				// #...........#
				// ###B#C#B#D###
				//   #D#C#B#A#
				//   #D#B#A#C#
				//   #A#D#C#A#
				//   #########

				std::string state;
				state.append(1, input[2][3]);
				state.append(1, input[3][3]);
				state.append(1, input[4][3]);
				state.append(1, input[5][3]);
				state.append(1, input[2][5]);
				state.append(1, input[3][5]);
				state.append(1, input[4][5]);
				state.append(1, input[5][5]);
				state.append(1, input[2][7]);
				state.append(1, input[3][7]);
				state.append(1, input[4][7]);
				state.append(1, input[5][7]);
				state.append(1, input[2][9]);
				state.append(1, input[3][9]);
				state.append(1, input[4][9]);
				state.append(1, input[5][9]);
				state.append(1, input[1][1]);
				state.append(1, input[1][2]);
				state.append(1, input[1][4]);
				state.append(1, input[1][6]);
				state.append(1, input[1][8]);
				state.append(1, input[1][10]);
				state.append(1, input[1][11]);
				exampleSolution.push_back(state);
				input.clear();
			}
		}
	}
}

int main()
{
//	Load("example2.txt", exampleSolution);
//	Process("example", "BACDBCDA.......", 12521);
//	Process("example", "BDDACCBDBBACDACA.......", 44169);
//	Process("input", "BDACABDC.......");
	Process("input", "BDDDACBCABABDACC.......");

	return 0;
}
