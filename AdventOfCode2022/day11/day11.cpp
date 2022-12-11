// day11.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 11/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

// Monkey 0:
//   Starting items: 79, 98
//   Operation: new = old * 19
//   Test: divisible by 23
//     If true: throw to monkey 2
//     If false: throw to monkey 3

enum OP
{
	INVALID,
	ADD,
	MULTIPLY
};

struct Monkey
{
	Monkey() = default;
	Monkey( int index ) :
		index(index)
	{}
	void Reset()
	{
		items = startingItems;
		throws = 0;
	}
	int index = -1;
	OP op = OP::INVALID;
	int64 val = 0; // -1 = "old"
	int testDiv = 0;
	int trueMonkey = 0;
	int falseMonkey = 0;
	std::queue<int64> startingItems;
	std::queue<int64> items;
	int64 throws = 0;
};

int64 CalculateResult(std::vector<Monkey>& monkeys)
{
	int64 first = 0;
	int64 second = 0;
	for (Monkey& m : monkeys)
	{
		if (m.throws > first)
		{
			if (first > second)
			{
				second = first;
			}
			first = m.throws;
		}
		else if (m.throws > second)
		{
			second = m.throws;
		}
	}
	assert(second);
	return first * second;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	std::vector<Monkey> monkeys;
	Monkey buffer;
	char* textBuffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(textBuffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (*thisLine == 'M')
				{
					if (buffer.index != -1)
					{
						buffer.Reset();
						monkeys.push_back(buffer);
					}
					int index;
					int m = sscanf(thisLine, "Monkey %d", &index);
					assert(m == 1);
					buffer = Monkey(index);
				}
				else
				{
					switch (thisLine[8])
					{
					case 'n':
						//   Starting items: 79, 98
					{
						char startingItems[80];
						int m = sscanf(thisLine, "  Starting items: %[^:]s", startingItems);
						assert(m == 1);
						char* tok = strtok(startingItems, ", ");
						while (tok)
						{
							buffer.startingItems.push(atoi(tok));
							tok = strtok(nullptr, ", ");
						}
						break;
					}
					case 'i':
						//   Operation: new = old * 19
					{
						char op;
						char val[80];
						int m = sscanf(thisLine, "  Operation: new = old %c %s", &op, val);
						assert(m == 2);
						buffer.op = (op == '+') ? OP::ADD : (op == '*') ? OP::MULTIPLY : OP::INVALID;
						assert(buffer.op != OP::INVALID);
						if (isdigit(*val))
							buffer.val = atoi(val);
						else
						{
							assert(strcmp(val, "old") == 0);
							buffer.val = -1;
						}
						break;
					}
					case 'd':
						//   Test: divisible by 23
					{
						int m = sscanf(thisLine, "  Test: divisible by %d", &buffer.testDiv);
						assert(m == 1);
						break;
					}
					case 'r':
						//     If true: throw to monkey 2
					{
						int m = sscanf(thisLine, "    If true: throw to monkey %d", &buffer.trueMonkey);
						assert(m == 1);
						assert(m == 1);
						break;
					}
					case 'a':
						//     If false: throw to monkey 3
					{
						int m = sscanf(thisLine, "    If false: throw to monkey %d", &buffer.falseMonkey);
						assert(m == 1);
						break;
					}
					default:
					{
						assert(false);
					}
					}

				}
			}
		}
	}
	fclose(fp);
	delete[] textBuffer;
	if (buffer.index != -1)
	{
		buffer.Reset();
		monkeys.push_back(buffer);
	}

	// Part 1
	for (int i = 0; i < 20; ++i)
	{
		for (Monkey& m : monkeys)
		{
			while (!m.items.empty())
			{
				int64 item = m.items.front();
				m.items.pop();
				int64 val = (m.val == -1) ? item : m.val;
				switch (m.op)
				{
				case OP::ADD:
					item += val;
					break;
				case OP::MULTIPLY:
					item *= val;
					break;
				default:
					assert(false);
				}
				item /= 3;
				if ((item % m.testDiv) == 0)
				{
					monkeys[m.trueMonkey].items.push(item);
				}
				else
				{
					monkeys[m.falseMonkey].items.push(item);
				}
				++m.throws;
			}
		}
	}

	int64 part1 = CalculateResult(monkeys);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	int64 testDivProduct = 1; 
	for (Monkey& m : monkeys)
	{
		// As the worry levels will exceed 64-bit integer arithmetic, and we only care about testing divisibility by a few relatively tiny ints, we can multiply all those ints together to get the worry levels within a much smaller (but still testable) range
		testDivProduct *= m.testDiv;
		// Each monkey also needs to be reset, to return to the starting position
		m.Reset();
	}

	// Part 2
	for (int i = 0; i < 10000; ++i)
	{
		for (Monkey& m : monkeys)
		{
			while (!m.items.empty())
			{
				int64 item = m.items.front();
				m.items.pop();
				int64 val = (m.val == -1) ? item : m.val;
				switch (m.op)
				{
				case OP::ADD:
					item += val;
					break;
				case OP::MULTIPLY:
					item *= val;
					break;
				default:
					assert(false);
				}
				item %= testDivProduct;
				if ((item % m.testDiv) == 0)
				{
					monkeys[m.trueMonkey].items.push(item);
				}
				else
				{
					monkeys[m.falseMonkey].items.push(item);
				}
				++m.throws;
			}
		}
	}

	int64 part2 = CalculateResult(monkeys);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 10605, 2713310158);
	Process("input.txt");

	return 0;
}
