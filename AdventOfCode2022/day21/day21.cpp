// day21.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 21/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Operator
{
	NONE,
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE
};

struct Node;

using Tree = std::map<const std::string, Node>;

struct Node
{
	Node() = default;
	Node(char* line)
	{
		// root: pppw + sjmn
		// dbpl: 5
		name = strtok(line, ": ");
		char* tok = strtok(nullptr, " ");
		if (isdigit(*tok))
		{
			value = atoll(tok);
		}
		else
		{
			left = tok;
			tok = strtok(nullptr, " ");
			assert(tok);
			switch (*tok)
			{
			case '+': op = ADD; break;
			case '-': op = SUBTRACT; break;
			case '*': op = MULTIPLY; break;
			case '/': op = DIVIDE; break;
			default: assert(false); break;
			}
			right = strtok(nullptr, " ");
		}
	}

	int64 Value(Tree& tree)
	{
		switch (op)
		{
		case NONE:
			return value;
		case ADD:
			return tree[left].Value(tree) + tree[right].Value(tree);
		case SUBTRACT:
			return tree[left].Value(tree) - tree[right].Value(tree);
		case MULTIPLY:
			return tree[left].Value(tree) * tree[right].Value(tree);
		case DIVIDE:
			return tree[left].Value(tree) / tree[right].Value(tree);
		}
		assert(false);
		return 0;
	}

	bool ContainsNode(std::string node, Tree& tree)
	{
		if (name == node)
		{
			return true;
		}
		if (op == NONE)
		{
			return false;
		}
		return tree[left].ContainsNode(node, tree) || tree[right].ContainsNode(node, tree);
	}

	// This is only called on the side of the branch that contains "humn"
	int64 FindHumn(int64 compValue, Tree& tree)
	{
		if (op == NONE)
		{
			assert(name == "humn");
			return compValue;
		}

		Node& leftBranch = tree[left];
		Node& rightBranch = tree[right];

		if (leftBranch.ContainsNode("humn", tree))
		{
			int64 testValue = rightBranch.Value(tree);
			// leftBranch op testValue == compValue, so leftBranch == compValue ^op testValue
			switch (op)
			{
			case ADD: testValue = compValue - testValue; break;
			case SUBTRACT: testValue = compValue + testValue; break;
			case MULTIPLY: testValue = compValue / testValue; break;
			case DIVIDE: testValue = compValue * testValue; break;
			}
			return leftBranch.FindHumn(testValue, tree);
		}
		else
		{
			assert(rightBranch.ContainsNode("humn", tree));
			int64 testValue = leftBranch.Value(tree);
			// testValue op rightBranch == compValue
			// compValue ^op rightBranch == testValue
			switch (op)
			{
			case ADD:
			{
				// testValue + rightBranch == compValue
				// rightBranch == compValue - testValue
				testValue = compValue - testValue;
				break;
			}
			case SUBTRACT: 
			{
				// testValue - rightBranch == compValue
				// -rightBranch == compValue - testValue
				testValue = testValue - compValue;
				break;
			}
			case MULTIPLY: 
			{
				// testValue * rightBranch == compValue
				// rightBranch == compValue / testValue
				testValue = compValue / testValue;
				break;
			}
			case DIVIDE:
			{
				// testValue / rightBranch == compValue
				// rightBranch == testValue / compValue
				testValue = testValue / compValue;
				break;
			}
			}
			return rightBranch.FindHumn(testValue, tree);
		}
	}

	std::string name;
	int64 value = -1;
	std::string left;
	Operator op = NONE;
	std::string right;
};

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Tree tree;
	double start = GetMilliseconds();
	int64 part1 = 0;
	int64 part2 = 0;
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
				Node n(thisLine);
				tree.insert(std::make_pair(n.name, n));
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	Node& root = tree["root"];
	part1 = root.Value(tree);

	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	Node& left = tree[root.left];
	Node& right = tree[root.right];

	bool leftValue = !left.ContainsNode("humn", tree);
	Node& test = leftValue ? right : left;
	int64 value = leftValue ? left.Value(tree) : right.Value(tree);

	part2 = test.FindHumn(value, tree);

	// Check the result
	Node& humn = tree["humn"];
	humn.value = part2;
	assert(left.Value(tree) == right.Value(tree));

	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 152, 301);
	Process("input.txt");

	return 0;
}
