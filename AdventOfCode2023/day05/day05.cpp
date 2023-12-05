// day05.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 05/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct convert
{
	bool operator<(const convert& other)
	{
		return source < other.source;
	}
	bool containsSource(int64 seed) const
	{
		return (source <= seed && (source + length - 1) >= seed);
	}

	int64 dest;
	int64 source;
	int64 length;
};

bool operator<(const convert& s1, const convert& s2)
{
	return s1.source < s2.source;
}

using converter = std::set<convert>;

struct seedRange
{
	seedRange(int64 s=0, int64 l=1)
	{
		start = s;
		end = s + l - 1;
	}
	int64 length() const
	{
		return end - start + 1;
	}
	int64 start;
	int64 end;
};

int64 Convert(int64 input, const converter &conv)
{
	for (const convert& c : conv)
	{
		if (input >= c.source && input < c.source + c.length)
		{
			return input - c.source + c.dest;
		}
	}
	return input;
}

// In part 2, ranges of seeds are converted
void Convert2(const seedRange& inputRange, std::vector<seedRange>& outputRanges, const converter& conv)
{
	// work through the input range, converting batches of seeds
	int64 start = inputRange.start;
	int64 end = inputRange.end;
	for (const convert& c : conv)
	{
		if (c.source > start)
		{
			seedRange newRange;
			newRange.start = start;
			if (c.containsSource(end) || end >= c.source + c.length)
			{
				newRange.end = c.source - 1;
				outputRanges.push_back(newRange);
				start = c.source;
			}
			else
			{
				newRange = end;
				outputRanges.push_back(newRange);
				return;
			}
		}
		if (c.containsSource(start))
		{
			seedRange newRange;
			newRange.start = Convert(start, conv);
			if (c.containsSource(end))
			{
				newRange.end = Convert(end, conv);
				outputRanges.push_back(newRange);
				return;
			}
			newRange.end = c.dest + c.length - 1;
			outputRanges.push_back(newRange);
			start += newRange.end - newRange.start + 1;
		}
	}
	// Everything left is unconverted so just add it to the output
	if (start <= end)
	{
		seedRange newRange;
		newRange.start = start;
		newRange.end = end;
		outputRanges.push_back(newRange);
	}
}

void ReadInts(char* thisLine, std::vector<int64>& output)
{
	char* tok = strtok(thisLine, " ");
	while (tok)
	{
		output.push_back(atoll(tok));
		tok = strtok(nullptr, " ");
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<int64> seeds;
	std::vector<converter> converters;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	converter *thisConverter = nullptr;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				if (seeds.size() == 0)
				{
					ReadInts(thisLine+7, seeds);
				}
				else
				{
					if (isalpha(line[0]))
					{
						// New converter
						if (thisConverter)
						{
							converters.push_back(*thisConverter);
							delete thisConverter;
						}
						thisConverter = new converter();
					}
					else
					{
						std::vector<int64> c;
						ReadInts(thisLine, c);
						convert conv;
						conv.dest = c[0];
						conv.source = c[1];
						conv.length = c[2];
						thisConverter->insert(conv);
					}
				}
			}
		}
	}
	if (thisConverter)
	{
		converters.push_back(*thisConverter);
		delete thisConverter;
	}
	fclose(fp);
	delete[] buffer;

	part1 = INT64_MAX;
	for (int64 seed : seeds)
	{
		int64 value = seed;
		for (const converter& conv : converters)
		{
			value = Convert(value, conv);
		}
		if (value < part1)
			part1 = value;
	}

	std::vector<seedRange> ranges;
	for (int i = 0; i < seeds.size(); i += 2)
	{
		seedRange range(seeds[i], seeds[i + 1]);
		ranges.push_back(range);
	}

	for (const converter& conv : converters)
	{
		std::vector<seedRange> inputRanges = ranges;
		std::vector<seedRange> outputRanges;
		for (const seedRange& range : inputRanges)
		{
			Convert2(range, outputRanges, conv);
		}
		ranges = outputRanges;
	}

	part2 = INT64_MAX;
	for (const seedRange& range : ranges)
	{
		if (range.start < part2)
			part2 = range.start;
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 35, 46);
	Process("input.txt");

	return 0;
}
