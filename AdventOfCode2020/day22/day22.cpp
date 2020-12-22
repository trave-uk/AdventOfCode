// day22.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using Game = std::array<std::deque<int>, 2>;

unsigned int crc(const Game& game)
{
	std::vector<char> buf;
	for (int p = 0; p < 2; ++p)
	{
		for (int i = 0; i < game[p].size(); ++i)
		{
			buf.push_back((char)game[p][i]);
		}
		buf.push_back('/');
	}
	return crc32buf(buf.data(), buf.size());
}


void printDecks(const Game& players)
{
	for (int p = 0; p < 2; ++p)
	{
		std::string hand;
		for (int card : players[p])
		{
			hand += std::to_string(card);
			hand += ", ";
		}
		if (hand.length() >= 2)
		{
			hand.pop_back();
			hand.pop_back();
		}
		printf("Player %d's deck: %s\n", p + 1, hand.data());
	}
}

static int gameID = 0;
static int recursion = 0;
static int fullRecursion = 0;
void PlayCombat(int& winner, Game& players, int part)
{
	const bool logging = false;
	int thisGame = ++gameID;
	if (logging) printf("=== Game %d ===\n", thisGame);
	std::set<unsigned int> history;
	winner = -1;
	unsigned int first = 0;
	int round = 0;
	while (!players[0].empty() && !players[1].empty())
	{
		++round;
		if (logging) printf("\n-- Round %d (Game %d) --\n", round, thisGame);
		if (logging) printDecks(players);
		unsigned int test = crc(players);

		// Check whether we've seen this configuration of cards previously
		if (history.count(test) > 0)
		{
			++recursion;
			if (test == first)
			{
				++fullRecursion;
			}
			if (logging) printf("Recursion detected!\n");
			winner = 0;
			break;
		}
		if (first == 0)
			first = test;
		history.insert(test);

		int p0 = players[0].front(); players[0].pop_front();
		int p1 = players[1].front(); players[1].pop_front();
		if (logging) printf("Player 1 plays: %d\nPlayer 2 plays: %d\n", p0, p1);

		int handWinner = -1;
		if (part == 2 && players[0].size() >= p0 && players[1].size() >= p1)
		{
			// Recurse
			Game newGame;
			for (int p = 0; p < 2; ++p)
			{
				for (int i = 0; i < (p ? p1 : p0); ++i)
				{
					newGame[p].push_back(players[p][i]);
				}
			}
			if (logging) printf("Playing a sub-game to determine the winner...\n\n");
			PlayCombat(handWinner, newGame, part);
			if (logging) printf("\n...anyway, back to game %d.\n", thisGame);
		}
		else
		{
			handWinner = p1 > p0;
		}
		if (logging) printf("Player %d wins round %d of game %d!\n", handWinner+1, round, thisGame);
		if (handWinner == 0)
		{
			players[0].push_back(p0);
			players[0].push_back(p1);
		}
		else
		{
			assert(handWinner == 1);
			players[1].push_back(p1);
			players[1].push_back(p0);
		}
	}
	if (winner == -1)
	{
		winner = players[1].size() > players[0].size();
	}
	if (logging) printf("The winner of game %d is player %d!\n", thisGame, winner+1);
}

int main()
{
	for (int part = 1; part <= 2; ++part)
	{
		gameID = 0;
		Game players;
		char* buffer = new char[65536];
		//FILE *fp = fopen("example.txt", "rt");
		//FILE *fp = fopen("example2.txt", "rt");
		FILE *fp = fopen("input.txt", "rt");
		int playerIndex;
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (*thisLine)
				{
					if (*thisLine == 'P')
					{
						int count = sscanf(thisLine, "Player %d", &playerIndex);
						assert(count == 1);
						--playerIndex;
						players[playerIndex].clear();
					}
					else
					{
						assert(isdigit(*thisLine));
						players[playerIndex].push_back(atoi(thisLine));
					}
				}
			}
		}
		fclose(fp);
		delete[] buffer;

		// play Combat
		int winner;
		PlayCombat(winner, players, part);

		printf("\n\n== Post-game results ==\n");
		printDecks(players);
		printf("%d games played, %d games decided on recursion, %d of those full recursion\n", gameID, recursion, fullRecursion);

		std::deque<int>& winningHand = players[winner];

		int score = 0;
		int multiplier = winningHand.size();
		for (int card : winningHand)
		{
			score += multiplier * card;
			--multiplier;
		}
		assert(multiplier == 0);

		printf("\nPart %d: %d\n\n", part, score);
	}
	return 0;
}
