// day20.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct InvalidRange
{
	unsigned int start;
	unsigned int end;

	InvalidRange(unsigned int _start, unsigned int _end)
		: start(_start)
		, end(_end)
	{
		assert(_start < _end);
	}

	bool operator<(const InvalidRange& other) const
	{
		return start < other.start;
	}

	bool contains(unsigned int pos) const
	{
		return start <= pos && end >= pos;
	}
};

using InvalidRanges = std::set<InvalidRange>;

void Process(std::string title, const char* filename, unsigned int high)
{
	InvalidRanges irs;
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
				unsigned int start, end;
				int matches = sscanf(thisLine, "%d-%d", &start, &end);
				assert(matches == 2);
				// Slot this into irs, somehow, in one of the following ways:
				// * fit within an existing entry, so be ignored
				// * overlap two or more entries, causing them to merge
				// * overlap one entry, causing it to be extended
				// * be distinct from all existing entries, so be inserted
				// Note that irs, being a set, is ordered

				// Actually, just insert this range first (it's ordered by ir.start in irs) and then iterate through irs to resolve merges

				irs.insert(InvalidRange(start, end));

				bool bRestart;
				do
				{
					bRestart = false;
					// If anything is modified, then once the modification has happened, restart the iteration (so set bRestart and break out of the loop)
					for (InvalidRanges::iterator it = irs.begin(); it != irs.end(); ++it)
					{
						const InvalidRange& ir = *it;

						auto it2 = it;
						++it2;
						if (it2 != irs.end())
						{
							const InvalidRange& nextIr = *it2;
							if (ir.contains(nextIr.start) || (nextIr.start == ir.end + 1))
							{
								//merge these two entries
								InvalidRange newIr(ir.start, max(ir.end, nextIr.end));
								irs.erase(ir);
								irs.erase(nextIr);
								irs.insert(newIr);
								bRestart = true;
								break;
							}
						}
					}
				} while (bRestart);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	unsigned int part1 = UINT_MAX;
	unsigned int part2 = 0;
	unsigned int prevEnd = 0;
	for (auto it = irs.begin(); it != irs.end(); ++it)
	{
		if (part1 == UINT_MAX)
		{
			if (it->start > 0)
				part1 = 0;
			else
				part1 = it->end + 1;
			part2 = it->start;
		}
		else
		{
			part2 += it->start - prevEnd - 1;
		}
		prevEnd = it->end;
	}
	part2 += high - prevEnd;

	printf("%s Part 1: %d\n", title.c_str(), part1);
	printf("%s Part 2: %d\n", title.c_str(), part2);
}

int main()
{
	Process("Example", "example.txt", 9);
	Process("My", "input.txt", UINT_MAX);
	return 0;
}
