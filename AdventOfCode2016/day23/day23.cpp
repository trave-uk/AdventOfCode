// day23.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 25/12/2020 https://github.com/trave-uk/AdventOfCode

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
	Processor(int64 numEggs)
	{
		// start with the number of eggs in register a.
		registers["a"] = numEggs;
	}

	std::map<std::string, int64> registers;
	int64 pc = 0;
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

	int64 process()
	{
		finished = false;

		while (!finished && pc >= 0 && pc < assembly.size())
		{
			std::string line = assembly[pc];
			char* command = strtok(&*line.begin(), " ");
			char* r = strtok(nullptr, " ");
			std::string reg(r);
			char* val = strtok(nullptr, " ");
			//			char reg = line[4];
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
			case CASE("inc"): // e.g. "inc a"
				registers[reg]++;
				break;
			case CASE("dec"): // e.g. "dec a"
				registers[reg]--;
				break;
			case CASE("cpy"): // e.g. "cpy 0 a"
				if (isalpha(*val))
				{
					// ignore, e.g. "cpy x 0", possible after tgl
					registers[val] = registers[reg];
				}
				break;
			case CASE("jnz"): // e.g. "jnz a 2"
				if (registers[reg] != 0)
				{
					pc += value(val);
					continue;
				}
				break;
			case CASE("tgl"): // e.g. "tgl a"
			{
				int64 tgl = pc + registers[reg];
				if (tgl >= 0 && tgl < assembly.size())
				{
					std::string &tglLine = assembly[tgl];
					char* tgldCommand = &*tglLine.begin();
					switch (CASE(tgldCommand))
					{
					case CASE("inc"):
						strncpy(tgldCommand, "dec", 3);
						break;
					case CASE("dec"):
					case CASE("tgl"):
						strncpy(tgldCommand, "inc", 3);
						break;
					case CASE("cpy"):
						strncpy(tgldCommand, "jnz", 3);
						break;
					case CASE("jnz"):
						strncpy(tgldCommand, "cpy", 3);
						break;
					}
				}
				break;
			}
			// NB: set, mul, sub, add, mod, jgz are from AoC2017. Not used here, but left in as they still make sense in this context
			case CASE("set"):
				registers[reg] = value(val);
				break;
			case CASE("mul"):
				registers[reg] *= value(val);
				break;
			case CASE("sub"):
				registers[reg] -= value(val);
				break;
			case CASE("add"):
				registers[reg] += value(val);
				break;
			case CASE("mod"):
				registers[reg] %= value(val);
				break;
			case CASE("jgz"):
				if (registers[reg] > 0)
				{
					pc += value(val);
					continue;
				}
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
		return registers["a"];
	}
};

void run(const char* filename)
{
	printf("%s:\n", filename);
	if (load(filename))
	{
		Processor p(7);
		int64 part1 = p.process();
		printf("Part 1: %lld\n", part1);
	}
	if (load(filename))
	{
		Processor p(12);
		int64 part2 = p.process();
		printf("Part 2: %lld\n", part2);
	}
}

int main()
{
	run("example.txt");
	run("input.txt");

	return 0;
}
