// day19.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 19/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Rule
{
	Rule(std::string target = std::string(""))
		: target(target)
		, category(0)
		, op(0)
		, test(0)
	{}

	Rule(char cat, char cmp, int64 test, std::string target)
		: target(target)
		, category(cat)
		, op(cmp)
		, test(test)
	{
	}

	std::string target;
	char category;
	char op;
	int64 test;
};

using Workflow = std::vector<Rule>;
using Workflows = std::map<std::string, Workflow>;

struct MachinePart
{
	MachinePart(int64 x = 0, int64 m = 0, int64 a = 0, int64 s = 0)
		: x(x)
		, m(m)
		, a(a)
		, s(s)
	{}

	void Set(char category, int64 value)
	{
		switch (category)
		{
		case 'x': x = value; break;
		case 'm': m = value; break;
		case 'a': a = value; break;
		case 's': s = value; break;
		default: assert(false);
		};
	}

	int64 Value(char category) const
	{
		switch (category)
		{
		case 'x': return x;
		case 'm': return m;
		case 'a': return a;
		case 's': return s;
		default: assert(false);
		}
		return 0;
	}

	int64 x;
	int64 m;
	int64 a;
	int64 s;
};

using MachineParts = std::vector<MachinePart>;

int64 Calculate(const MachinePart& part, Workflows& workflows)
{
	std::string w("in");
	while (w != "A" && w != "R")
	{
		const Workflow& workflow = workflows[w];
		for (const Rule& r : workflow)
		{
			if (r.op == '<')
			{
				if (part.Value(r.category) < r.test)
				{
					w = r.target;
					break;
				}
			}
			else if (r.op == '>')
			{
				if (part.Value(r.category) > r.test)
				{
					w = r.target;
					break;
				}
			}
			else
			{
				assert(r.op == 0);
				w = r.target;
				break;
			}
		}
	}
	if (w == "A")
	{
		return part.x + part.m + part.a + part.s;
	}
	else
	{
		return 0;
	}
}

struct MachinePartRange
{
	int64 Total() const
	{
		return (upper.x - lower.x + 1) * (upper.m - lower.m + 1) * (upper.a - lower.a + 1) * (upper.s - lower.s + 1);
	}

	MachinePart lower = MachinePart(1, 1, 1, 1);
	MachinePart upper = MachinePart(4000, 4000, 4000, 4000);
};

using MachinePartRanges = std::vector<MachinePartRange>;

void Scan(const MachinePartRange& range, MachinePartRanges& ranges, std::string w, Workflows& workflows)
{
	if (w == "A")
	{
		ranges.push_back(range);
		return;
	}
	else if (w == "R")
	{
		return;
	}
	const Workflow& workflow = workflows[w];
	MachinePartRange working = range;
	for (const Rule& rule : workflow)
	{
		MachinePartRange range2 = working;
		if (rule.op == '<')
		{
			int64 currentUpper = range2.upper.Value(rule.category);
			if (rule.test - 1 < currentUpper)
				range2.upper.Set(rule.category, rule.test - 1);
			if (rule.test > working.lower.Value(rule.category))
				working.lower.Set(rule.category, rule.test);
		}
		else if (rule.op == '>')
		{
			int64 currentLower = range2.lower.Value(rule.category);
			if (rule.test + 1 > currentLower)
				range2.lower.Set(rule.category, rule.test + 1);
			if (rule.test < working.upper.Value(rule.category))
				working.upper.Set(rule.category, rule.test);
		}
		Scan(range2, ranges, rule.target, workflows);
	}
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Workflows workflows;
	MachineParts parts;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				if (isalpha(line[0]))
				{
					// px{a<2006:qkq,m>2090:A,rfg}
					Workflow w;
					char* nm = strtok(thisLine, "{");
					std::string name(nm);
					char* rules = strtok(nullptr, "}");
					char* rul = strtok(rules, ",");
					while (rul)
					{
						Rule r;
						if (rul[1] == '>' || rul[1] == '<')
						{
							char cat;
							char cmp;
							int64 test;
							char result[80];
							int count = sscanf(rul, "%c%c%lld:%s", &cat, &cmp, &test, result);
							assert(count == 4);
							r = Rule(cat, cmp, test, std::string(result));
						}
						else
						{
							r = Rule(std::string(rul));
						}
						w.push_back(r);
						rul = strtok(nullptr, ",");
					}
					workflows[name] = w;
				}
				else
				{
					assert(line[0] == '{');
					// {x=787,m=2655,a=1222,s=2876}
					MachinePart part;
					char* tok = strtok(thisLine, "{,");
					while (tok)
					{
						assert(tok[1] == '=');
						char cat = tok[0];
						int64 value = atoll(tok + 2);
						part.Set(cat, value);
						tok = strtok(nullptr, ",}");
					}
					parts.push_back(part);
				}

			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (const MachinePart& part : parts)
	{
		part1 += Calculate(part, workflows);
	}

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	MachinePartRanges ranges;
	MachinePartRange range;
	Scan(range, ranges, "in", workflows);
	for (const MachinePartRange& r : ranges)
	{
		part2 += r.Total();
	}

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);

}

int main()
{
	Process("example.txt", 19114, 167409079868000ll);
	Process("input.txt");

	return 0;
}
