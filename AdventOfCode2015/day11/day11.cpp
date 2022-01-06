// day11.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 6/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void Increment(std::string &password)
{
	int pos = password.length() - 1;
	for(;;)
	{
		assert(pos >= 0);
		++password[pos];
		if (password[pos] > 'z')
		{
			password[pos] = 'a';
			--pos;
		}
		else
		{
			break;
		}
	}
}

bool IsValid(std::string password)
{
	// cannot contain 'i', 'o' or 'l'.
	if (password.find('i') != std::string::npos ||
		password.find('o') != std::string::npos ||
		password.find('l') != std::string::npos)
	{
		return false;
	}
	
	bool valid1 = false;
	int count2 = 0;
	char last1 = 0;
	char last2 = 0;
	char last = 0;
	for (char c : password)
	{
		if (c == last1 + 1 && last1 == last2 + 1)
			valid1 = true;
		last2 = last1;
		last1 = c;

		if (last == c)
		{
			++count2;
			last = 0;
		}
		else
		{
			last = c;
		}
	}

	return valid1 && count2 >= 2;
}

std::string Process(std::string input, int part, std::string expected = "")
{
	std::string result = input;
	do
	{
		Increment(result);
	} while (!IsValid(result));
	assert(expected == "" || expected == result);
	printf("%s: Part %d: %s\n", input.c_str(), part, result.c_str());
	return result;
}

int main()
{
	assert(!IsValid("hijklmmn"));
	assert(!IsValid("abbceffg"));
	assert(!IsValid("abbcegjk"));
	Process("abcdefgh", 1, "abcdffaa");
	Process("ghijklmn", 1, "ghjaabcc");
	std::string part1 = Process("hxbxwxba", 1);
	Process(part1, 2);

	return 0;
}
