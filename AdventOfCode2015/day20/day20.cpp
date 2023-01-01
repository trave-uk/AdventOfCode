// day20.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 01/01/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"
#include "../primes.h"

int64 CountPresents(int64 house, int part = 1)
{
	// Factorize house
	// ~~~~~~~~~~~~~~~
	// First, get the prime factors 
	std::set<int64> primeFactors;
	int64 max = integer_sqrt(house);
	assert(max < primes.back());
	for (int64 p : primes)
	{
		if (p > max)
			break;
		int64 p2 = p;
		// include powers of primes.
		while ((house % p2) == 0)
		{
			primeFactors.insert(p2);
			primeFactors.insert(house / p2);
			p2 *= p;
			if (p2 == 1)
				break;
		}
	}

	// Then, from that, calculate all factors. This is all products of the prime factors; as this includes powers of them, check each one is actually a factor before adding it.
	// Start with all the prime factors, then for each of these, try multiplying by each of the prime factors.
	// Continue doing this (multiplying every found factor by each of the prime factors) until no new factors are added.
	std::set<int64> allFactors = primeFactors;
	std::set<int64> newFactors = allFactors;
	bool found = false;
	do
	{
		found = false;
		for (int64 f : allFactors)
		{
			for (int64 f2 : primeFactors)
			{
				int64 nf = f * f2;
				if ((house % nf) == 0 && newFactors.count(nf) == 0)
				{
					found = true;
					newFactors.insert(nf);
				}
			}
		}
		allFactors = newFactors;
	} while (found);

	// Sum these factors
	int64 multiplier = (part == 1) ? 10 : 11;
	int64 result = 0;
	for (int64 f : allFactors)
	{
		if (part == 2)
		{
			if ((house / f) > 50)
				continue;
		}
		result += f * multiplier;
	}
	return result;
}

void Process(int64 test, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	double start = GetMilliseconds();
	int64 part1 = 0;
	for (int64 house = 1; house < test/10; ++house)
	{
		int64 presents = CountPresents(house);
		if (presents >= test)
		{
			part1 = house;
			break;
		}
	}
	if (expectedPart1 != -1)
	{
		printf("[%.2f] %lld: Part 1 expected: %lld\n", GetMilliseconds() - start, test, expectedPart1);
	}
	printf("[%.2f] %lld: Part 1: %lld\n", GetMilliseconds() - start, test, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	int64 part2 = 0;
	for (int64 house = 1; true; ++house)
	{
		int64 presents = CountPresents(house, 2);
		if (presents >= test)
		{
			part2 = house;
			break;
		}
	}
	if (expectedPart2 != -1)
	{
		printf("[%.2f] %lld: Part 2 expected: %lld\n", GetMilliseconds() - start, test, expectedPart2);
	}
	printf("[%.2f] %lld: Part 2: %lld\n", GetMilliseconds() - start, test, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process(120, 6);
	Process(150, 8);
	Process(130, 8);
	Process(33100000);

	return 0;
}
