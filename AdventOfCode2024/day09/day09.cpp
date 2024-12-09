// day09.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 09/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using FileMap = std::vector<int64>;
using FreeMap = std::deque<int64>;
const int64 FreeSpace = -1ll;

void ExpandLine(const std::string& line, FileMap& files, FreeMap& free)
{
	int64 current = 0;
	int64 pos = 0;
	for (char c : line)
	{
		int amount = c - '0';
		int64 file = FreeSpace;
		if ((current % 2) == 0)
		{
			file = current / 2;
		}
		for (int i=0; i<amount; ++i)
		{
			files.push_back(file);
			if (file == FreeSpace)
			{
				free.push_back(pos);
			}
			++pos;
		}
		++current;
	}
}

void DefragFiles(FileMap& files, FreeMap& free)
{
	while (!free.empty())
	{
		int64 move = files.back();
		files.pop_back();
		if (move == FreeSpace)
		{
			continue;
		}
		int64 pos = free.front();
		free.pop_front();
		if (pos < files.size())
			files[pos] = move;
		else
			files.push_back(move);
	}
}

int64 Checksum(FileMap& files)
{
	int64 checksum = 0;
	int64 pos = 0;
	for (auto file : files)
	{
		if (file != FreeSpace)
		{
			checksum += pos * file;
		}
		++pos;
	}
	return checksum;
}

int64 ProcessPart1(std::string line)
{
	FileMap files;
	FreeMap free;
	ExpandLine(line, files, free);
	DefragFiles(files, free);
	return Checksum(files);
}

bool FindFile(FileMap& files, int minLen, int64 file, FileMap::iterator& first, int& len)
{
	auto start = files.begin();
	do
	{
		first = std::find(start, files.end(), file);
		if (first == files.end())
			return false;
		len = 0;
		auto test = first;
		while (test != files.end() && *test == file)
		{
			++len;
			++test;
		}
		start = test;
	} while (len < minLen);
	return true;
}

void DefragFiles2(FileMap& files)
{
	// keep files contiguous
	int64 file = files.back();
	while (file >= 0)
	{
		// find file
		FileMap::iterator first;
		int len;
		FindFile(files, 0, file, first, len);
		// Now file a space big enough for this file
		FileMap::iterator firstSpace;
		int spaceLen;
		if (FindFile(files, len, FreeSpace, firstSpace, spaceLen))
		{
			assert(spaceLen >= len);
			if (firstSpace < first)
			{
				for (int i = 0; i < len; ++i)
				{
					*firstSpace = *first;
					*first = FreeSpace;
					++firstSpace;
					++first;
				}
			}
		}
		--file;
	}
}

int64 ProcessPart2(std::string line)
{
	FileMap files;
	FreeMap free;
	ExpandLine(line, files, free);
	DefragFiles2(files);
	return Checksum(files);
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int pos = 0;
				std::string line(thisLine);
				part1 = ProcessPart1(line);
				part2 = ProcessPart2(line);
				break;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 1928, 2858);
	Process("input.txt");

	return 0;
}
