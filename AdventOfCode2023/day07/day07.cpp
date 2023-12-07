// day07.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 07/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

/*
Five of a kind, where all five cards have the same label: AAAAA
Four of a kind, where four cards have the same label and one card has a different label: AA8AA
Full house, where three cards have the same label, and the remaining two cards share a different label: 23332
Three of a kind, where three cards have the same label, and the remaining two cards are each different from any other card in the hand: TTT98
Two pair, where two cards share one label, two other cards share a second label, and the remaining card has a third label: 23432
One pair, where two cards share one label, and the other three cards have a different label from the pair and each other: A23A4
High card, where all cards' labels are distinct: 23456
*/

enum Type
{
	NONE,
	HIGH_CARD,
	ONE_PAIR,
	TWO_PAIR,
	THREE_OF_A_KIND,
	FULL_HOUSE,
	FOUR_OF_A_KIND,
	FIVE_OF_A_KIND
};

bool doingPart2 = false;
int CardValue(char card)
{
	if (isdigit(card))
		return card - '0';
	else if (card == 'T')
		return 10;
	else if (card == 'J')
		return doingPart2 ? 1 : 11;
	else if (card == 'Q')
		return 12;
	else if (card == 'K')
		return 13;
	else
	{
		assert(card == 'A');
		return 14;
	}
}
struct Hand
{
	std::string cards;
	int bid = 0;
	Type type = NONE;

	void IdentifyType(std::string _cards = std::string())
	{
		Type newType = NONE;
		std::string s = cards;
		if (_cards.length())
			s = _cards;
		std::sort(s.begin(), s.end());
		if (s[0] != s[1] && s[1] != s[2] && s[2] != s[3] && s[3] != s[4])
			newType = HIGH_CARD;
		else if (s[0] == s[1] && s[1] == s[2] && s[2] == s[3] && s[3] == s[4])
			newType = FIVE_OF_A_KIND;
		else if (s[0] == s[3] || s[1] == s[4])
			newType = FOUR_OF_A_KIND;
		else if (s[0] == s[2])
		{
			if (s[3] == s[4])
				newType = FULL_HOUSE;
			else
				newType = THREE_OF_A_KIND;
		}
		else if (s[2] == s[4])
		{
			if (s[0] == s[1])
				newType = FULL_HOUSE;
			else
				newType = THREE_OF_A_KIND;
		}
		else if (s[1] == s[3])
			newType = THREE_OF_A_KIND;
		else // must be ONE_PAIR or TWO_PAIR
		{
			int pairs = 0;
			for (int i=0; i<4; ++i)
				pairs += s[i] == s[i+1];
			assert(pairs == 1 || pairs == 2);
			if (pairs == 1)
				newType = ONE_PAIR;
			else
				newType = TWO_PAIR;
		}
		assert(newType != NONE);
		if (newType > type)
			type = newType;
	}

	void RecurseReplaceJoker(std::string copy, std::vector<int> jokers)
	{
		std::string values = "AKQT98765432";
		int index = jokers.back();
		jokers.pop_back();
		for (char v : values)
		{
			copy[index] = v;
			IdentifyType(copy);
			if (jokers.size() > 0)
				RecurseReplaceJoker(copy, jokers);
		}
	}

	void MaximizeType()
	{
		// J is a joker that can be any other card
		std::vector<int> jokers;
		int index = 0;
		for (char c : cards)
		{
			if (c == 'J')
				jokers.push_back(index);
			++index;
		}
		if (jokers.size() > 0)
		{
			std::string copy = cards;
			RecurseReplaceJoker(copy, jokers);
		}
	}
};

bool operator<(const Hand& h1, const Hand& h2)
{
	if (h1.type < h2.type)
		return true;
	if (h1.type > h2.type)
		return false;
	for (int i = 0; i < 5; ++i)
	{
		if (CardValue(h1.cards[i]) < CardValue(h2.cards[i]))
			return true;
		if (CardValue(h1.cards[i]) > CardValue(h2.cards[i]))
			return false;
	}
	return false;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	std::set<Hand> hands;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// 32T3K 765
				Hand h;
				char* tok = strtok(thisLine, " ");
				h.cards = std::string(tok);
				tok = strtok(nullptr, " ");
				h.bid = atoi(tok);
				h.IdentifyType();
				hands.insert(h);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int rank = 1;
	for (Hand h : hands)
	{
		part1 += rank * h.bid;
		++rank;
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	doingPart2 = true;
	std::set<Hand> hands2;

	for (Hand h : hands)
	{
		Hand h2 = h;
		h2.MaximizeType();
		hands2.insert(h2);
	}
	rank = 1;
	for (Hand h : hands2)
	{
		part2 += rank * h.bid;
		++rank;
	}

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 6440, 5905);
	Process("input.txt");

	return 0;
}
