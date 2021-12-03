// day03.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 3/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

void CountBits(const std::set<std::string>& data, int bit, int& nOnes, int& nZeros)
{
	nOnes = 0;
	nZeros = 0;
	for (std::string line : data)
	{
		if (line[bit] == '1')
			++nOnes;
		else if (line[bit] == '0')
			++nZeros;
		else
			assert(false);
	}
}

void Process(const char* filename)
{
	std::set<std::string> data;

	int result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int bits = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				data.insert(line);
				if (!bits)
				{
					bits = line.length();
				}
				else
				{
					assert(bits == line.length());
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int gamma = 0;
	int epsilon = 0;
	std::set<std::string> oxygen = data;
	std::set<std::string> co2 = data;

	for (int i=0; i<bits; ++i)
	{
		int nOnes = 0, nZeros = 0;
		CountBits(data, i, nOnes, nZeros);

		gamma *= 2;
		epsilon *= 2;
		if ( nOnes > nZeros)
		{
			++gamma;
		}
		else if (nZeros > nOnes)
		{
			++epsilon;
		}
		else
		{
			assert(false);
		}


		std::vector<std::string> remove;
		if (oxygen.size() > 1)
		{
			CountBits(oxygen, i, nOnes, nZeros);
			char keepThisBit = (nOnes >= nZeros) ? '1' : '0';
			for (auto it = oxygen.begin(); it != oxygen.end(); ++it)
			{
				if ((*it)[i] != keepThisBit)
				{
					remove.push_back(*it);
				}
			}
			for (std::string s : remove)
			{
				oxygen.erase(s);
			}
		}

		remove.clear();
		if (co2.size() > 1)
		{
			CountBits(co2, i, nOnes, nZeros);
			char keepThisBit = (nOnes >= nZeros) ? '0' : '1';
			for (auto it = co2.begin(); it != co2.end(); ++it)
			{
				if ((*it)[i] != keepThisBit)
				{
					remove.push_back(*it);
				}
			}
			for (std::string s : remove)
			{
				co2.erase(s);
			}
		}

	}

	assert(oxygen.size() == 1);
	assert(co2.size() == 1);

	int ox = 0;
	for (char c : (*oxygen.begin()))
	{
		ox *= 2;
		ox += (c == '1') ? 1 : 0;
	}
	int co = 0;
	for (char c : (*co2.begin()))
	{
		co *= 2;
		co += (c == '1') ? 1 : 0;
	}
	printf("%s: Part 1: %d\n", filename, gamma * epsilon);
	printf("%s: Part 2: %d\n", filename, ox * co);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
