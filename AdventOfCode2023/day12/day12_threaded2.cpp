// day12.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 12/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Ints = std::vector<int64>;
using BitInt = int128;

struct Pattern
{
	static std::array<std::array<BitInt, 128>, 2> bitpatterns;
	static void init()
	{
		BitInt full = 0;
		for (int i = 0; i < 128; ++i)
		{
			bitpatterns[0][i] = 0;
			bitpatterns[1][i] = full;
			full <<= 1;
			full |= 1;
		}
	}

	Pattern(std::string input = "")
	{
		for (char c : input)
		{
			known <<= 1;
			mask <<= 1;
			if (c == '#')
			{
				known |= 1;
			}
			if (c != '?')
			{
				mask |= 1;
			}
			++length;
		}
	}

	void PushBits(int knownbit, int maskbit, int64 bits)
	{
		assert(bits < bitpatterns[0].size());
		known <<= bits;
		mask <<= bits;
		known |= bitpatterns[knownbit][bits];
		mask |= bitpatterns[maskbit][bits];
		length += bits;
	}

	Pattern(const Ints& gaps, const Ints& numbers, int64 extraSpaces)
	{
		auto n_iter = numbers.begin();
		for (int64 gap : gaps)
		{
			PushBits(0, 1, gap);
			if (n_iter != numbers.end())
			{
				int64 block = *n_iter++;
				PushBits(1, 1, block);
				if (n_iter != numbers.end())
				{
					PushBits(0, 1, 1);
				}
			}
		}
		while (n_iter != numbers.end())
		{
			extraSpaces += *n_iter++;
			if (n_iter != numbers.end())
				++extraSpaces;
		}
		known <<= extraSpaces;
		mask <<= extraSpaces;
	}

	void Expand()
	{
		Pattern part = *this;
		for (int i = 0; i < 4; ++i)
		{
			known <<= 1 + length;
			mask <<= 1 + length;
			known |= part.known;
			mask |= part.mask;
		}
		length *= 5;
		length += 4;
	}

	bool Matches(const Pattern& other) const
	{
		BitInt combined_mask = mask & other.mask;
		return (known & combined_mask) == (other.known & combined_mask);
	}

	BitInt known = 0;
	BitInt mask = 0;
	int64 length = 0;
};

std::array<std::array<BitInt, 128>, 2> Pattern::bitpatterns;

struct job
{
	job(const Pattern& pattern, const std::vector<int64>& numbers, int64 size, int64 line, std::atomic_int64_t& part2)
		: pattern(pattern)
		, numbers(numbers)
		, size(size)
		, line(line)
		, part2(part2)
		, numPatterns(0)
	{}

	Pattern pattern;
	const std::vector<int64> numbers;
	int64 size;
	int64 line;
	std::atomic_int64_t& part2;
	int64 numPatterns;
};

bool MatchPattern(const Pattern& pattern, const job& j)
{
	Pattern p = pattern;
	int64 extraSpaces = j.pattern.length - p.length;
	p.PushBits(0, 0, extraSpaces);
	return j.pattern.Matches(p);
}

struct patternTest
{
	patternTest(Pattern& p, int64 nextNumber, int64 extraSpaces, int64 gaps)
		: p(p)
		, nextNumber(nextNumber)
		, extraSpaces(extraSpaces)
		, gaps(gaps)
	{}

	Pattern p;
	int64 nextNumber;
	int64 extraSpaces;
	int64 gaps;
};

void CountPatterns(job& j, Pattern& p, int64 nextNumber, int64 extraSpaces, int64 gaps)
{
	std::deque<patternTest> tests;
	tests.push_back(patternTest(p, nextNumber, extraSpaces, gaps));

	while (!tests.empty())
	{
		patternTest test = tests.back();
		tests.pop_back();

		if (test.extraSpaces == 0)
		{
			while (test.nextNumber < j.numbers.size())
			{
				test.p.PushBits(1, 1, j.numbers[test.nextNumber++]);
				if (test.nextNumber < j.numbers.size())
					test.p.PushBits(0, 1, 1);
			}
			if (MatchPattern(test.p, j))
				++j.numPatterns;
			continue;
		}
		if (test.gaps == 1)
		{
			test.p.PushBits(0, 1, test.extraSpaces);
			if (MatchPattern(test.p, j))
				++j.numPatterns;
			continue;
		}
		if (!MatchPattern(test.p, j))
			continue;
		int64 nextNumber2 = test.nextNumber + 1;
		for (int64 thisGap = 0; thisGap <= test.extraSpaces; ++thisGap)
		{
			patternTest test2 = test;
			test2.p.PushBits(0, 1, thisGap);
			test2.p.PushBits(1, 1, j.numbers[test.nextNumber]);
			if (nextNumber2 < j.numbers.size())
				test2.p.PushBits(0, 1, 1);
			test2.nextNumber = nextNumber2;
			test2.extraSpaces -= thisGap;
			--test2.gaps;
			tests.push_back(test2);
		}
	}
}

int64 CalculateScore(job &j)
{
	// find all patterns that match the numbers
	int64 extraSpaces = j.pattern.length - j.size; // the number of extra spaces that could be between banks of '#'
	int64 gaps = j.numbers.size() + 1; // the number of places that extra spaces can be placed: either side of or between the banks of '#'.

	Pattern p;
	CountPatterns(j, p, 0, extraSpaces, gaps);

	return j.numPatterns;
}


ThreadSafeQueue<job> jobs;

std::array<std::thread, 15> threads;

void worker()
{
	while (true)
	{
		try
		{
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				job j = jobs.pop_front();
				CalculateScore(j);
				printf("line %lld: %lld\n", j.line, j.numPatterns);
				j.part2 += j.numPatterns;
			}
		}
		catch (finishedThread)
		{
			return;
		}
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Pattern::init();
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	std::atomic_int64_t part1(0);
	std::atomic_int64_t part2(0);
	int64 _part1 = 0;
	int64 _part2 = 0;
	int64 maxLength = 0;
	int64 line = 1;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				// ????.######..#####. 1,6,5
				char* p = strtok(thisLine, " ");
				std::string pat(p);
				Pattern pattern(pat);
				maxLength = max(pattern.length, maxLength);
				std::vector<int64> numbers;
				char* number = strtok(nullptr, ",");
				int64 size = -1;
				while (number)
				{
					int64 num = atoll(number);
					numbers.push_back(num);
					number = strtok(nullptr, ",");
					size += num + 1;
				}
				job j(pattern, numbers, size, line, part1);
				int64 part1score = CalculateScore(j);
				_part1 += part1score;
				printf("line %lld: %lld\n", line, part1score);

				// for part 2, brute force it, but multithreaded
				Pattern pattern2 = pattern;
				pattern2.Expand();
				std::vector<int64> numbers2;
				for (int i = 0; i < 5; ++i)
				{
					numbers2.insert(numbers2.end(), numbers.begin(), numbers.end());
				}
				int64 size2 = size * 5 + 4;;
				
				jobs.push(job(pattern2, numbers2, size2, line, part2));
//				int64 part2score = CalculateScore(pattern2, numbers2, size2);
//				part2 += part2score;
//				printf("line %lld: p1: %lld p2: %lld\n", line++, part1score, part2score);
				line++;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == _part1);
	printf("%s: Part 1: %lld\n", filename, _part1);

	for (std::thread& thread : threads)
	{
		thread = std::thread(worker);
	}
	for (std::thread& thread : threads)
	{
		thread.join();
	}
	_part2 = part2;
	assert(expectedPart2 == -1 || expectedPart2 == _part2);
	printf("%s: Part 2: %lld\n", filename, _part2);

}

int main()
{
	// Hacked together to just solve line 632 from the input, which is the awkward one.
	Process("example2.txt", 10, 506250);
	Process("input632.txt", 177);

	return 0;
}
