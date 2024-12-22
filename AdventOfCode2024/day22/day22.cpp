// day22.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 22/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 NextPrice(int64 lastPrice)
{
	int64 next1 = lastPrice;
	next1 *= 64;
	next1 ^= lastPrice;
	next1 %= 16777216;

	int64 next2 = next1;
	next2 /= 32;
	next2 ^= next1;
	next2 %= 16777216;

	int64 next3 = next2;
	next3 *= 2048;
	next3 ^= next2;
	next3 %= 16777216;

	return next3;
}

using Price = int8_t;
using PriceSequence = std::array<Price, 2000>;
using PriceSequences = std::vector<PriceSequence>;

using ChangeSequence = std::array<Price, 4>;
using ChangeSequences = std::set<ChangeSequence>;
using ChangeSequenceses = std::vector<ChangeSequences>;

Price BestPriceForSequence(const ChangeSequence& cs, const PriceSequence& ps, const PriceSequence& pc)
{
	for (int i = 0; i < 1997; ++i)
	{
		if (pc[i] == cs[0] && pc[i + 1] == cs[1] && pc[i + 2] == cs[2] && pc[i + 3] == cs[3])
		{
			return ps[i + 3];
		}
	}
	return 0;
}

int64 ScoreForChangeSequence(const ChangeSequence& cs, const PriceSequences& priceSequences, const PriceSequences& priceChanges)
{
	int64 score = 0;
	assert(priceSequences.size() == priceChanges.size());
	int64 merchantCount = priceSequences.size();
	for (int64 i = 0; i < merchantCount; ++i)
	{
		const PriceSequence& ps = priceSequences[i];
		const PriceSequence& pc = priceChanges[i];
		Price price = BestPriceForSequence(cs, ps, pc);
		score += int64(price);
	}
	return score;
}

int64 GetBestSequence(const std::vector<int64>& inputPrices)
{
	PriceSequences priceSequences;
	PriceSequences priceChanges;

	ChangeSequences changeSequences;

	for (int64 price : inputPrices)
	{
		PriceSequence ps;
		PriceSequence pc;
		for (int i = 0; i < 2000; ++i)
		{
			int64 nextPrice = NextPrice(price);
			ps[i] = Price(nextPrice % 10);
			Price lastPrice = Price(price % 10);
			pc[i] = Price(ps[i] - lastPrice);
			price = nextPrice;
		}
		for (int i = 3; i < 2000; ++i)
		{
			if (ps[i] == 9)
			{
				ChangeSequence cs;
				cs[0] = pc[i - 3];
				cs[1] = pc[i - 2];
				cs[2] = pc[i - 1];
				cs[3] = pc[i];

				if (BestPriceForSequence(cs, ps, pc) == 9)
				{
					changeSequences.insert(cs);
				}
			}
		}
		priceSequences.push_back(ps);
		priceChanges.push_back(pc);
	}

	int64 bestScore = 0;
	for (const ChangeSequence& cs : changeSequences)
	{
		int64 score = ScoreForChangeSequence(cs, priceSequences, priceChanges);
		if (score > bestScore)
		{
			bestScore = score;
		}
	}
	return bestScore;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<int64> prices;

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
				prices.push_back(atoi(thisLine));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = 0;
	for (int64 price : prices)
	{
		for (int i = 0; i < 2000; ++i)
		{
			price = NextPrice(price);
		}
		part1 += price;
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (expectedPart2 != -2)
	{
		int64 part2 = GetBestSequence(prices);

		assert(expectedPart2 == -1 || expectedPart2 == part2);
		printf("%s: Part 2: %lld\n", filename, part2);
	}
}

int main()
{
	Process("example1.txt", 37327623, -2);
	Process("example2.txt", -1, 23);
	Process("input.txt");

	return 0;
}
