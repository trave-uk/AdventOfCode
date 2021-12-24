// day24.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 24/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

std::vector<std::string> assembly;

bool load(const char* filename)
{
	assembly.clear();
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	if (!fp)
		return false;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				assembly.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	return true;
}


#define CASE(x) (x[0] + x[1] * 256 + x[2] * 65536)

class Processor
{
public:
	std::map<std::string, int64> registers;
	size_t pc = 0;
	bool finished = false;

	int64 value(const char* val)
	{
		if (isalpha(*val))
		{
			if (registers.count(val) == 0)
			{
				return 0;
			}
			else
			{
				return registers[val];
			}
		}
		else
		{
			return (atoll(val));
		}
	}

	int64 process( std::string input )
	{
		int at = 0;
		finished = false;

		while (!finished && pc >= 0 && pc < assembly.size())
		{
			std::string line = assembly[pc];
			char* command = strtok(&*line.begin(), " ");
			char* r = strtok(nullptr, " ");
			std::string reg(r);
			char* val = strtok(nullptr, " ");
			if (registers.count(reg) == 0)
			{
				registers.insert(std::make_pair(reg, 0));
				if (!isalpha(*r))
				{
					// Hack: if a number is used, store it as a register with its own value
					registers[reg] = atoi(r);
				}
			}
			switch (CASE(command))
			{
			case CASE("inp"): // e.g. "inp a"
				registers[reg] = input[at++] - '0';
				break;
			case CASE("add"):
				registers[reg] += value(val);
				break;
			case CASE("mul"):
				registers[reg] *= value(val);
				break;
			case CASE("div"):
				registers[reg] /= value(val);
				break;
			case CASE("mod"):
				registers[reg] %= value(val);
				break;
			case CASE("eql"):
				registers[reg] = (registers[reg] == value(val));
				break;
			default:
				assert(false);
				finished = true;
				break;
			}
			if (!finished)
			{
				++pc;
			}
		}
		return registers["z"];
	}
};

void run(const char* filename, std::string input, int64 expectedZ, int part)
{
	printf("%s:\n", filename);
	if (load(filename))
	{
		Processor p;
		int64 z = p.process(input);
		if (z == expectedZ)
			printf("Part %i: %s -> %lld\n", part, input.c_str(), z);
		else
			printf("Part %i: %s doesn't work (z == %lld)\n", part, input.c_str(), z);
	}
}

int main()
{
	run("example.txt", "9", 1, 0);

	// See input_notes.txt for how the following numbers were worked out
	run("input.txt", "74929995999389", 0, 1);
	run("input.txt", "11118151637112", 0, 2);
	
	return 0;
}
