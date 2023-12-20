// day20.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 20/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"
#include "../primes.h"

enum ModuleType
{
	Untyped,
	FlipFlop,
	Conjunction
};

int64 lowPulses = 0;
int64 highPulses = 0;

struct QueueItem
{
	QueueItem(const std::string& f, const std::string& t, bool p)
		: from(f)
		, to(t)
		, pulse(p)
	{}

	std::string from;
	std::string to;
	bool pulse;
};

using Queue = std::queue<QueueItem>;

struct Module
{
	Module(const std::string& name = "", const std::string& output = "")
		: name(name)
		, type(Untyped)
	{
		outputs.push_back(output);
	}

	Module(char* input)
	{
		// %a -> inv, con
		char* namestring = strtok(input, " ->");
		if (*namestring == '%')
		{
			type = FlipFlop;
			++namestring;
		}
		else if (*namestring == '&')
		{
			type = Conjunction;
			++namestring;
		}
		name = std::string(namestring);

		char* eachoutput = strtok(nullptr, "->, ");
		while (eachoutput)
		{
			outputs.push_back(std::string(eachoutput));
			eachoutput = strtok(nullptr, ", ");
		}
	}

	void Reset()
	{
		receivedPulse = false;
		flipFlopState = false;
		conjunctionMemory.clear();
	}

	void Receive(bool pulse, const std::string& from, Queue& queue)
	{
		if (pulse)
			++highPulses;
		else
			++lowPulses;
		receivedPulse = true;
		bool outputPulse = false;
		switch (type)
		{
		case Untyped:
			outputPulse = pulse;
			break;
		case FlipFlop:
			if (pulse)
				return;
			flipFlopState = !flipFlopState;
			outputPulse = flipFlopState;
			break;
		case Conjunction:
			if (pulse)
			{
				conjunctionMemory.insert(from);
			}
			else
			{
				conjunctionMemory.erase(from);
			}
			if (conjunctionMemory.size() == inputs.size())
			{
				outputPulse = false;
			}
			else
			{
				outputPulse = true;
			}
			break;
		}
		for (std::string& output : outputs)
		{
			queue.push(QueueItem(name, output, outputPulse));
		}
	}

	std::string name;
	ModuleType type = Untyped;
	std::vector<std::string> outputs;
	std::vector<std::string> inputs;	// Used by conjunction modules

	bool receivedPulse = false;
	bool flipFlopState = false;
	// which inputs last sent a high pulse
	std::set<std::string> conjunctionMemory;
};

using Modules = std::map<std::string, Module>;

void PopulateInputs(Modules& modules)
{
	for (auto module : modules)
	{
		for (std::string output : module.second.outputs)
		{
			if (modules.count(output))
			{
				modules[output].inputs.push_back(module.first);
			}
		}
	}
}

void Process(const char* filename, bool attemptPart2, int64 expectedPart1 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Modules modules;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				Module m(thisLine);
				modules[m.name] = m;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	PopulateInputs(modules);

	lowPulses = highPulses = 0;
	int64 i = 0;
	for (i = 1; i <= 1000; ++i)
	{
		Queue q;
		q.push(QueueItem("button", "broadcaster", false));
		while (!q.empty())
		{
			QueueItem qi = q.front();
			q.pop();
			if (modules.count(qi.to))
			{
				modules[qi.to].Receive(qi.pulse, qi.from, q);
			}
			else
			{
				if (qi.pulse)
					++highPulses;
				else
					++lowPulses;
			}
		}
	}
	part1 = lowPulses * highPulses;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (!attemptPart2)
		return;

	// OK, so looking at the input, there are four inputs to the primary conjunction &dt, which all need to go high for a low to sent to rx.
	// Each of those (secondary) conjunction inputs is an inverter conjunction with another (tertiary) conjunction that takes a lot of inputs.
	// So all 4 of the tertiary conjunctions need to go low simultaneously. Each one must have a cycle. All start low so the number of iterations when they go low again for the first time should be the cycle.
	// secondary conjunctions: &ks &pm &dl &vk
	// tertiary conjunctions: &vr &pf &ts &xd
	// Rather than hard-code these, work back from rx.
	std::string primary;
	std::vector<std::string> secondaries;
	std::vector<std::string> tertiaries;
	for (auto& iter : modules)
	{
		Module& m = iter.second;
		m.Reset();
		if (m.type == Conjunction && m.outputs[0] == "rx")
		{
			assert(m.outputs.size() == 1);
			primary = iter.first;
			secondaries = iter.second.inputs;
			for (const std::string& secondary : secondaries)
			{
				const Module& m1 = modules[secondary];
				assert(m1.type == Conjunction && m1.inputs.size() == 1 && m1.outputs.size() == 1);
				tertiaries.push_back(m1.inputs[0]);
			}
		}
	}
	assert(tertiaries.size() == 4);
	std::array<int64, 4> cycles;
	int done = 0;
	for (int64 i = 1; ; ++i)
	{
		Queue q;
		q.push(QueueItem("button", "broadcaster", false));
		while (!q.empty() && !part2)
		{
			QueueItem qi = q.front();
			q.pop();
			if (modules.count(qi.to))
			{
				for (int j = 0; j < 4; ++j)
				{
					if (cycles[j] > 0)
						continue;
					if (!qi.pulse && qi.from == tertiaries[j] && qi.to == secondaries[j])
					{
						// Check that this is a prime number (assuming low enough that we know, but otherwise IsPrime will assert anyway)
						// It isn't required that this is a prime number, but if it is it means we can be certain that we can just multiply the cycles together.
						assert(IsPrime(i));
						cycles[j] = i;
						++done;
					}
				}
				modules[qi.to].Receive(qi.pulse, qi.from, q);
			}
		}
		if (done == 4)
		{
			printf("%lld * %lld * %lld * %lld\n", cycles[0], cycles[1], cycles[2], cycles[3]);
			part2 = cycles[0] * cycles[1] * cycles[2] * cycles[3];
			break;
		}
	}
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", false, 32000000);
	Process("example2.txt", false,  11687500);
	Process("input.txt", true);

	return 0;
}
