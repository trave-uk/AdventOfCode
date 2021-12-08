// day08.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 8/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct OutputDigit
{
	OutputDigit(std::string _segments)
		: segments(_segments)
	{}

	std::string segments;
};

OutputDigit outputDigits[10] = 
{
	OutputDigit("abcefg"),  // 0   aaa
	OutputDigit("cf"),      // 1  b   c 
	OutputDigit("acdeg"),   // 2  b   c  
	OutputDigit("acdfg"),   // 3   ddd
	OutputDigit("bcdf"),    // 4  e   f
	OutputDigit("abdfg"),   // 5  e   f
	OutputDigit("abdefg"),  // 6   ggg
	OutputDigit("acf"),     // 7
	OutputDigit("abcdefg"), // 8
	OutputDigit("abcdfg"),  // 9
};

struct Digit
{
	Digit() = default;
	Digit(std::string _signals)
		: signals(_signals)
	{
		if (signals.length() == 2)
			value = 1;
		else if (signals.length() == 4)
			value = 4;
		else if (signals.length() == 3)
			value = 7;
		else if (signals.length() == 7)
			value = 8;
	}
	std::string signals;
	int value = -1;
};

using Convert = std::map<char, std::string>;

struct Display
{
	Display() = default;

	std::array<Digit, 10> input;
	std::array<Digit, 4> output;
	int outputValue = 0;

	Convert mappings;

	void UpdatePossible(char segment, const std::string& possible)
	{
		if (!mappings.count(segment))
		{
			mappings.insert(std::make_pair(segment, possible));
		}
		else
		{
			// combine
			std::string& segments = mappings[segment];
			std::string remove;
			for (char d : segments)
			{
				if (possible.find(d) == std::string::npos)
				{
					remove.append(1, d);
				}
			}
			for (char r : remove)
			{
				auto it = std::find(segments.begin(), segments.end(), r);
				segments.erase(it);
			}
		}
	}

	void UpdateImpossible(char segment, const std::string& impossible)
	{
		if (!mappings.count(segment))
		{
			mappings.insert(std::make_pair(segment, std::string("abcdefg")));
		}
		std::string& segments = mappings[segment];
		for (char r : impossible)
		{
			auto it = std::find(segments.begin(), segments.end(), r);
			segments.erase(it);
		}
	}

	void UpdateMappings(const Digit& d)
	{
		// Find this in input and output, and update the value there if not already set
		for (Digit &i : input)
		{
			if (i.signals == d.signals && i.value != d.value)
			{
				assert(i.value == -1);
				i.value = d.value;
			}
		}
		for (Digit &o : output)
		{
			if (o.signals == d.signals && o.value != d.value)
			{
				assert(o.value == -1);
				o.value = d.value;
			}
		}

		std::string signals = d.signals;
		std::string segments = outputDigits[d.value].segments;
		for (char c = 'a'; c <= 'g'; ++c)
		{
			if (signals.find(c) == std::string::npos)
			{
				UpdateImpossible(c, segments);
			}
			else
			{
				UpdatePossible(c, segments);
			}
		}
	}

	bool CouldBeValue(const Digit& d, int value)
	{
		std::string segments = outputDigits[value].segments;
		if (d.signals.length() != segments.length())
		{
			return false;
		}
		for (char c : d.signals)
		{
			if (mappings.count(c) == 0)
				continue;

			bool bFound = false;
			for (char d : mappings[c])
			{
				if (segments.find(d) != std::string::npos)
				{
					bFound = true;
				}
			}
			if (!bFound)
				return false;
		}
		return true;
	}

	void Decode()
	{
		// So first, for all known values (1, 4, 7, 8), fill in the potential mappings
		// Just work on the inputs, as the inputs contain all 10 digits
		std::vector<Digit> fiveBars;
		std::vector<Digit> sixBars;
		for (Digit d : input)
		{
			if (d.value != -1)
			{
				UpdateMappings(d);
			}
			else if (d.signals.length() == 5)
			{
				fiveBars.push_back(d);
			}
			else if (d.signals.length() == 6)
			{
				sixBars.push_back(d);
			}
			else
			{
				// Shouldn't get here, either we know what the value is or 5 or 6 segments are used.
				assert(false);
			}
		}

		// Now, consider the remaining 6 values (unknown)

		// The ones with 5 segments (2, 3, 5) always light up a,d,g. Of these, only 2 lights up e (with c), only 5 lights up b (with f). 3 lights up c and f.
		// So: we can identify 3 by comparing against 2 and 5.
		
		for (Digit& d : fiveBars)
		{
			int diffs = 0;
			for (const Digit& e : fiveBars)
			{
				if (&d == &e)
					continue;
				for (char ee : e.signals)
				{
					if (d.signals.find(ee) == std::string::npos)
					{
						++diffs;
					}
				}
			}
			if (diffs == 2)
			{
				assert(d.value == -1);
				d.value = 3;
				UpdateMappings(d);
			}
		}
		// As 3 is found, we should be able to identify 2 and 5 given the mappings we already have
		for (Digit& d : fiveBars)
		{
			if (d.value != -1)
				continue;
			if (CouldBeValue(d, 2))
			{
				assert(!CouldBeValue(d, 5));
				d.value = 2;
			}
			else
			{
				assert(CouldBeValue(d, 5));
				d.value = 5;
			}
			UpdateMappings(d);
		}

		// We have all the mappings now, so identify 0, 6 and 9
		for (Digit&d : sixBars)
		{
			if (CouldBeValue(d, 0))
			{
				assert(!CouldBeValue(d, 6));
				assert(!CouldBeValue(d, 9));
				d.value = 0;
			}
			else if (CouldBeValue(d, 6))
			{
				assert(!CouldBeValue(d, 9));
				d.value = 6;
			}
			else
			{
				assert(CouldBeValue(d, 9));
				d.value = 9;
			}
			UpdateMappings(d);
		}

		outputValue = 0;
		for (Digit&d : output)
		{
			if (d.value == -1)
			{
				for (int v = 0; v < 10; ++v)
				{
					if (CouldBeValue(d, v))
					{
						assert(d.value == -1);
						d.value = v;
					}
				}
			}
			assert(d.value != -1);
			outputValue *= 10;
			outputValue += d.value;
		}
	}
};

using Displays = std::vector<Display>;

void Process(const char* filename)
{
	int part1 = 0;
	int part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			Display d;
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				char *inputs = strtok(thisLine, "|");
				char *outputs = strtok(nullptr, "|");

				char* input = strtok(inputs, " ");
				int index = 0;
				while (input)
				{
					Digit digit(input);
					d.input[index] = digit;
					input = strtok(nullptr, " ");
					++index;
				}

				char* output = strtok(outputs, " ");
				index = 0;
				while (output)
				{
					Digit digit(output);
					if (digit.value != -1)
						++part1;
					d.output[index] = digit;
					output = strtok(nullptr, " ");
					++index;
				}

				d.Decode();
				part2 += d.outputValue;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	printf("%s: Part 1: %d\n", filename, part1);
	printf("%s: Part 2: %d\n", filename, part2);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
