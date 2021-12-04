// day14.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 17/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::string md5x(std::string input, int extra = 0)
{
	MD5 md5;
	std::string result = md5.digestString(input.c_str());
	for (int x = 0; x < extra; ++x)
	{
		std::string copy = result;
		result = md5.digestString(copy.c_str());
	}
	return result;
}

using MD5Cache = std::map<int64, std::string>;
using FiveChars = std::map<int64, std::string>;
using PossibleKey = std::pair<int64, char>;
using PossibleKeys = std::vector<PossibleKey>;

char might_be_key(std::string hash)
{
	char p = 0;
	int count = 0;
	for (char c : hash)
	{
		if (c == p)
		{
			count++;
			if (count == 3)
			{
				return p;
			}
		}
		else
		{
			p = c;
			count = 1;
		}
	}
	return 0;
}

std::string contains_five(std::string hash)
{
	std::string found;
	char p = 0;
	int count = 0;
	for (char c : hash)
	{
		if (c == p)
		{
			count++;
			if (count == 5)
			{
				found.append(1, p);
			}
		}
		else
		{
			p = c;
			count = 1;
		}
	}
	return found;
}

int64 result(std::string salt, bool part2 = false)
{
	// calculate the hashes from 0, spotting five chars the same (with a string for each char, there might be more than one)
	// once 1000 hashes calculated, check (index-1000) each iteration for possible keys (3 chars the same, saying which char)
	// if a possible key found, quickly check the FiveChars map for any matching fives in the interim.
	MD5Cache md5s;
	FiveChars fivechars;
	PossibleKeys possiblekeys;
	int64 nKeys = 0;
	for (int64 index = 0; true; ++index)
	{
		std::string test = salt + std::to_string(index);
		std::string hash = md5x(test, part2 ? 2016 : 0 );
		md5s.insert_or_assign(index, hash);

		std::string fives = contains_five(hash);
		if (fives.length() > 0)
		{
			fivechars.insert_or_assign(index, fives);
		}

		if (index >= 1000)
		{
			int64 index2 = index - 1000;
			auto found = md5s.find(index2);
			std::string hash2 = found->second;
			md5s.erase(index2);
			fivechars.erase(index2);
			char ch = might_be_key(hash2);
			if (ch)
			{
				for (auto found5 : fivechars)
				{
					if (found5.second.find(ch) != std::string::npos)
					{
						nKeys++;
						auto found2 = md5s.find(found5.first);
						std::string hash3 = found2->second;
						printf("Key %lld: %s%lld->%s %s%lld->%s\n", nKeys, salt.c_str(), index2, hash2.c_str(), salt.c_str(), found5.first, hash3.c_str());
						if (nKeys == 64)
						{
							return index2;
						}
						break;
					}
				}
			}
		}
	}
}

int main()
{
	printf("Example 1: %lld\n", result("abc"));
	printf("Part 1: %lld\n", result("jlmsuwbz"));
	printf("Example 2: %lld\n", result("abc", true));
	printf("Part 2: %lld\n", result("jlmsuwbz", true));
	return 0;
}
