// day07.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 2/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct wire
{
	wire(std::string input = ". -> .");
	int64 evaluate();

	std::string name;
	std::string expression;
	int64 value = 0;
	bool bEvaluated = false;
};

wire::wire(std::string input)
{
	char _name[10];
	char _expression[32];
	int matches = sscanf(input.c_str(), "%[^-]-> %s", _expression, _name);
	assert(matches == 2);
	name = _name;
	_expression[strlen(_expression) - 1] = '\0';
	expression = _expression;
}

std::map<std::string, wire> wires;

int64 getValue(std::string wire)
{
	if (isdigit(wire[0]))
	{
		return atoll(wire.c_str());
	}
	else
	{
		assert(wires.count(wire) > 0);
		return wires[wire].evaluate();
	}
}

int64 wire::evaluate()
{
	if (bEvaluated) return value;

	// NOT
	if (expression.substr(0, 3) == "NOT")
	{
		int64 v = getValue(expression.substr(4));
		value = ~v;
		bEvaluated = true;
	}
	else
	{
		// AND, OR, LSHIFT, RSHIFT
		char left[10];
		char op[10];
		char right[10];
		int matches = sscanf(expression.c_str(), "%s %s %s", left, op, right);
		if (matches == 3)
		{
			int64 l = getValue(left);
			int64 r = getValue(right);
			std::string o(op);
			if (o == "AND")
			{
				value = l & r;
				bEvaluated = true;
			}
			if (o == "OR")
			{
				value = l | r;
				bEvaluated = true;
			}
			if (o == "LSHIFT")
			{
				value = l << r;
				bEvaluated = true;
			}
			if (o == "RSHIFT")
			{
				value = l >> r;
				bEvaluated = true;
			}
		}
		else
		{
			value = getValue(expression);
			bEvaluated = true;
		}
	}
	assert(bEvaluated);
	return value;
}

void Process(const char* filename, std::string wirePart1, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	wires.clear();
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
				wire w(thisLine);
				wires.insert_or_assign(w.name, w);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = wires[wirePart1].evaluate();
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (wires.count("b") > 0)
	{
		for (auto& w : wires)
		{
			w.second.value = 0;
			w.second.bEvaluated = false;
		}

		wire& b = wires["b"];
		b.expression = std::to_string(part1);
		int64 part2 = wires[wirePart1].evaluate();
		assert(expectedPart2 == -1 || expectedPart2 == part2);
		printf("%s: Part 2: %lld\n", filename, part2);
	}
}

int main()
{
	Process("example.txt", "g", 114);
	Process("input.txt", "a");

	return 0;
}
