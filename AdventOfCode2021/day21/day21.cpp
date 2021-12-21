// day21.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 21/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct player
{
	player(int id = 0, int _pos = 0)
		: index(id - 1)
		, pos(_pos)
		, startPos(_pos)
		, score(0)
	{
	}

	int index;
	int pos;
	int startPos; // store the start pos for part 2 so that part 1 can change pos.
	int score;
};
using playerArray = std::array<player, 2>;

void Part1(const char* filename, playerArray& players)
{
	// Deterministic dice just increments up to 100 and resets to 1.
	// Players on board with 10 spaces in loop 1 to 10
	// Each turn, roll dice three times, move that many times, add space value to score
	// Winner first to get to 1000

	struct DeterministicDice
	{
		int dice = 1;
		int rolls = 0;

		int Roll()
		{
			++rolls;
			if (dice > 100)
				dice -= 100;
			return dice++;
		}
	} die;

	bool finished = false;
	while (!finished)
	{
		for (player& p : players)
		{
			p.pos += die.Roll();
			p.pos += die.Roll();
			p.pos += die.Roll();
			while (p.pos > 10)
				p.pos -= 10;

			p.score += p.pos;

			if (p.score >= 1000)
			{
				int otherScore = players[1 - p.index].score;
				printf("%s: Part 1: %d\n", filename, otherScore * die.rolls);
				finished = true;
			}
		}
	}
}

void Part2(const char* filename, playerArray& players)
{
	// Ah jeez. So. Dirac dice: 3 sides (1,2,3), universe splits into three each time it is rolled.
	// Winner is first to get to 21. Count the universes each player wins in, result is the higher number, so player 1's count (player 1 starts first).

	// 210 possible player states:
	// pos 1-10
	// score 0-20 (>=21 has already won)
	struct playerState
	{
		playerState(int _pos = 0)
			: pos(_pos)
			, score(0)
		{}
		int pos;
		int score;
		bool operator<(const playerState& other) const
		{
			return score < other.score || (score == other.score && pos < other.pos);
		}
	};

	// Each possible player state has 7 possible next states, with different probabilities.
	struct nextState
	{
		playerState state;
		int universes;
	};
	using nextStates = std::array<nextState, 7>;

	struct _ : public std::map<playerState, nextStates>
	{
		_()
		{
			// Each turn (3 dice) there are 27 possibilities/universes: 1*3, 3*4, 6*5, 7*6, 6*7, 3*8, 1*9.
			// There are 10 possible starting positions.
			// Further: there are 21 possible playing scores: 0 to 20 (considering any score >=21 wins), and 10 possible positions, so 210 possible player playing states. Not all are possible given the puzzle input, as at score 0, only the two starting positions are possible.
			// Considering each player individually, then, each of these states has 7 possible new states, each path adding between 1 and 7 universes.
			playerState key;
			for (key.pos = 1; key.pos <= 10; ++key.pos)
			{
				for (key.score = 0; key.score < 21; ++key.score)
				{
					// No need to store anything for score>=21, since game over at that point

					nextStates nextStates;

					// 1*3, 3*4, 6*5, 7*6, 6*7, 3*8, 1*9
					const std::array<int, 10> universes =
					{
						0, 0, 0, 1, 3, 6, 7, 6, 3, 1
					};

					int roll = 3;

					for (nextState& next : nextStates)
					{
						next.state.pos = ((key.pos + roll - 1) % 10) + 1;
						next.state.score = key.score + next.state.pos;
						next.universes = universes[roll];
						++roll;
					}

					insert_or_assign(key, nextStates);
				}
			}
		}
	} stateMap;

	// For each of the players' starting state (pos = 1 to 10, score = 0), count how many paths there are to 21 points. For each number of turns in a path, store how many universes that occurs in
	// Also store in how many universes each player is still playing (hasn't finished) for each number of turns
	std::array<std::map<int, int64>, 2> winningTurnsToUniverses;
	std::array<std::map<int, int64>, 2> continuingTurnsToUniverses;

	for (auto p : players)
	{
		struct search
		{
			playerState state;
			int turns = 0;
			int64 universes = 1;
		};
		std::vector<search> openSet;

		search start;
		start.state = playerState(p.startPos);
		openSet.push_back(start);

		while (!openSet.empty())
		{
			search node = openSet.back();
			openSet.pop_back();
			const nextStates& nextStates = stateMap[node.state];
			for (const nextState& next : nextStates)
			{
				search node2 = node;
				node2.state = next.state;
				++node2.turns;
				node2.universes *= next.universes;

				if (node2.state.score >= 21)
				{
					winningTurnsToUniverses[p.index][node2.turns] += node2.universes;
				}
				else
				{
					continuingTurnsToUniverses[p.index][node2.turns] += node2.universes;
					openSet.push_back(node2);
				}
			}
		}
	}

	// Check that the numbers generated make sense.
	for (auto p : players)
	{
		auto& winning = winningTurnsToUniverses[p.index];
		auto& continuing = continuingTurnsToUniverses[p.index];
		for (int turns = 2; turns < 10; ++turns)
		{
			assert(continuing[turns] == continuing[turns - 1] * 27 - winning[turns]);
		}
	}

	// So. Now, work out how often player 1 wins. This will be the answer as they start first
	int64 player1wins = 0;
	for (const auto& player1turns : winningTurnsToUniverses[0])
	{
		int turns1 = player1turns.first;
		int64 universes1 = player1turns.second;
		int64 universes2 = continuingTurnsToUniverses[1][turns1-1]; // compare against player 2 on the previous turn
		player1wins += universes1 * universes2;
	}

	// Also work out how often player 2 wins, to confirm my supposition and, as the puzzle gives player 2's result for the example, so it can be confirmed.
	int64 player2wins = 0;
	for (const auto& player2turns : winningTurnsToUniverses[1])
	{
		int turns2 = player2turns.first;
		int64 universes2 = player2turns.second;
		int64 universes1 = continuingTurnsToUniverses[0][turns2]; // compare against player 1 on the same turn
		player2wins += universes2 * universes1;
	}

	assert(player1wins > player2wins);

	printf("%s: Part 2: %lld player 1 wins (player 2 got %lld wins)\n", filename, player1wins, player2wins);
}

void Process(const char* filename)
{
	playerArray players;
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
				// e.g. "Player 1 starting position: 4"
				int id = -1, pos = -1;
				int matches = sscanf(thisLine, "Player %d starting position: %d", &id, &pos);
				assert(matches == 2);

				players[id - 1] = player(id, pos);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	Part1(filename, players);
	Part2(filename, players);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
