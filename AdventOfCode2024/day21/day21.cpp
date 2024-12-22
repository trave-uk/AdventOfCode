// day21.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 21/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

#define USE_BRUTE_FORCE 0

using Codes = std::vector<std::string>;

using Keypad = std::map<char, coord>;

// numeric keypad:
// +---+---+---+
// | 7 | 8 | 9 |
// +---+---+---+
// | 4 | 5 | 6 |
// +---+---+---+
// | 1 | 2 | 3 |
// +---+---+---+
//     | 0 | A |
//     +---+---+
Keypad numericKeypad =
{
	{'A', {0, 0}},
	{'0', {-1, 0}},
	{'1', {-2, -1}},
	{'2', {-1, -1}},
	{'3', {0, -1}},
	{'4', {-2, - 2}},
	{'5', {-1, -2}},
	{'6', {0, -2}},
	{'7', {-2, -3}},
	{'8', {-1, -3}},
	{'9', {0, -3}}
};

// directional keypad:
//     +---+---+
//     | ^ | A |
// +---+---+---+
// | < | v | > |
// +---+---+---+
Keypad directionalKeypad =
{
	{'A', {0, 0}},
	{'^', {-1, 0}},
	{'<', {-2, 1}},
	{'v', {-1, 1}},
	{'>', {0, 1}}
};

// All robots start pointing at 'A'.

// One directional keypad that you are using.
// Two directional keypads that robots are using.
// One numeric keypad(on a door) that a robot is using.

bool CornerIsEmpty(const coord& corner, const Keypad& keypad)
{
	for (const auto& k : keypad)
	{
		if (k.second == corner)
		{
			return false;
		}
	}
	return true;
}

// fragmentHashMap is used to reduce memory usage for brute force algorithm.
using FragmentHashMap = std::map<uint32_t, std::string>;
FragmentHashMap fragmentHashMap;

const std::string& GetFragmentByHash(uint32_t fragmentHash)
{
	static std::string emptyString;
	if (!fragmentHash)
		return emptyString;
	return fragmentHashMap[fragmentHash];
}

uint32_t SetFragmentByHash(const std::string& fragment)
{
	uint32_t crc = crc32buf(fragment.c_str(), fragment.length());
	if (!fragmentHashMap.count(crc))
	{
		fragmentHashMap[crc] = fragment;
	}
	else
	{
		assert(fragmentHashMap[crc] == fragment);
	}
	return crc;
}

// Inputs is a graph of sequence fragments. Each eventual input is a path through the graph.
// Each node contains a sequence fragment hash and up to two next node pointers.
// Branches diverge, but will meet again.
class Inputs
{
public:
	Inputs(std::string fragment = "") : children(), fragmentHash(0) { children.fill(nullptr); SetFragment(fragment); }

	std::array<Inputs*, 2> children;
	uint32_t fragmentHash;

	void SetFragment(const std::string& fragment)
	{
		fragmentHash = SetFragmentByHash(fragment);
	}
	const std::string& GetFragment() const
	{
		return GetFragmentByHash(fragmentHash);
	}

	void SetChild(int index, Inputs* child)
	{
		children[index] = child;
	}
	Inputs* GetChild(int index) const
	{
		return children[index];
	}

	void Destroy()
	{
		std::set<Inputs*> seen;
		std::set<Inputs*> stack;
		stack.insert(this);
		while (!stack.empty())
		{
			Inputs* self = *stack.begin();
			stack.erase(self);
			for (int i = 0; i < 2; ++i)
			{
				Inputs* child = self->GetChild(i);
				if (!child)
					continue;
				if (seen.count(child))
					continue;
				stack.insert(child);
				seen.insert(child);
			}
			self->children.fill(nullptr);
		}
		for (Inputs* child : seen)
		{
			delete child;
		}
		seen.clear();
	}

	int64 CountChildren() const
	{
		if (children[0])
		{
			if (children[1])
					return 2;
			else
				return 1;
		}
		else
			return 0;
	}

#if USE_BRUTE_FORCE
	// Length is only used in the brute-force method.
	int64 Length(bool bCheckAll) const
	{
		const std::string& fragment = GetFragment();
		int64 length = fragment.length();
		if (CountChildren())
		{
			if (bCheckAll)
			{
				// get min length
				int64 minLength = INT64_MAX;
				for (int i = 0; i < 2; ++i)
				{
					Inputs* child = GetChild(i);
					if (!child)
						continue;
					int64 childLength = child->Length(bCheckAll);
					if (childLength < minLength)
						minLength = childLength;
				}
				length += minLength;
			}
			else
			{
				const Inputs* child = GetChild(0);
				length += child->Length(bCheckAll);
			}
		}
		return length;
	}
#endif

	void CloneFrom(const Inputs& other, Inputs* child0 = nullptr, Inputs* child1 = nullptr)
	{
		using CloneRecord = std::pair<const Inputs*, Inputs*>;
		using CloneStack = std::stack<CloneRecord>;
		using ClonedInputs = std::map<const Inputs*, Inputs*>;
		CloneStack stack;
		ClonedInputs done;
		CloneRecord self = std::make_pair(&other, this);
		stack.push(self);
		while (!stack.empty())
		{
			CloneRecord next = stack.top();
			stack.pop();
			const Inputs* from = next.first;
			Inputs* to = next.second;
			to->fragmentHash = from->fragmentHash;
			for (int i = 0; i < 2; ++i)
			{
				const Inputs* fromChild = from->GetChild(i);
				if (!fromChild)
				{
					if (i == 0)
					{
						to->SetChild(0, child0);
						to->SetChild(1, child1);
					}
					break;
				}
				Inputs* newChild = nullptr;
				if (done.count(fromChild))
				{
					newChild = done[fromChild];
				}
				else
				{
					newChild = new Inputs();
					CloneRecord newRecord = std::make_pair(fromChild, newChild);
					stack.push(newRecord);
					done[fromChild] = newChild;
				}
				to->SetChild(i, newChild);
			}
		}
	}
};

// There are a limited number of sequence fragments. Ignoring the numeric pad, there seem to be 160 unique fragments (after 15 robots on the first code anyway).
// Also, all fragments end in A. Many will start on A, move to another position, then move back to A. These _should_ always result in the same transformation
// So, for each (input fragment / robot position) pair when first seen, store results (as an Inputs object sequence) in a map.
// For the next N times it is seen (so: store a counter against each hash), recalculate the Inputs object sequence and check whether it matches. If not, this hash cannot be used.
// If the sequence matched each time, then no need to recalculate it each time, in fact, just store the hash (in a new member).
// Each time we iterate to a new robot, it _may_ be that the previous map is invalidated so start again. But I think it should continue to work.

using SequenceId = uint32_t;
using SequenceMap = std::map<SequenceId, Inputs>;

SequenceMap sequenceMap;

void ExpandInputs(Inputs* outputInputs, coord& robotPos, const Keypad& keypad)
{
	Inputs original = *outputInputs;
	SequenceId sequenceId = outputInputs->fragmentHash;
	if (sequenceMap.count(sequenceId))
	{
		Inputs& prev = sequenceMap[sequenceId];
		outputInputs->SetFragment("");
		outputInputs->SetChild(0, nullptr);
		outputInputs->SetChild(1, nullptr);
		outputInputs->CloneFrom(prev, original.GetChild(0), original.GetChild(1));
		return;
	}

	Inputs* start = outputInputs;
	outputInputs->SetFragment("");
	outputInputs->SetChild(0, nullptr);
	outputInputs->SetChild(1, nullptr);
	for (char nextKey : original.GetFragment())
	{
		coord nextKeyPos = keypad.at(nextKey);
		coord move = nextKeyPos; move -= robotPos;

		char horizontalButton = (move.first < 0) ? '<' : ((move.first > 0) ? '>' : 0);
		char verticalButton = (move.second < 0) ? '^' : ((move.second > 0) ? 'v' : 0);
		// check for no movement required
		if (!horizontalButton && !verticalButton)
		{
			std::string fragment = outputInputs->GetFragment();
			fragment.append("A");
			outputInputs->SetFragment(fragment);
			assert(robotPos == nextKeyPos);
		}
		// check for pure vertical movement
		else if (!horizontalButton)
		{
			std::string fragment = outputInputs->GetFragment();
			fragment.append(abs(move.second), verticalButton);
			fragment.append("A");
			outputInputs->SetFragment(fragment);
		}
		// check for pure horizontal movement
		else if (!verticalButton)
		{
			std::string fragment = outputInputs->GetFragment();
			fragment.append(abs(move.first), horizontalButton);
			fragment.append("A");
			outputInputs->SetFragment(fragment);
		}
		// check whether horizontal movement hits the empty space
		else if (CornerIsEmpty(coord(nextKeyPos.first, robotPos.second), keypad))
		{
			std::string fragment = outputInputs->GetFragment();
			fragment.append(abs(move.second), verticalButton);
			fragment.append(abs(move.first), horizontalButton);
			fragment.append("A");
			outputInputs->SetFragment(fragment);
		}
		// check whether vertical movement hits the empty space
		else if (CornerIsEmpty(coord(robotPos.first, nextKeyPos.second), keypad))
		{
			std::string fragment = outputInputs->GetFragment();
			fragment.append(abs(move.first), horizontalButton);
			fragment.append(abs(move.second), verticalButton);
			fragment.append("A");
			outputInputs->SetFragment(fragment);
		}
		else
		{
			// Either way could be used. Branch.
			std::string firstInput;
			firstInput.append(abs(move.second), verticalButton);
			firstInput.append(abs(move.first), horizontalButton);
			firstInput.append("A");
			Inputs* leftBranch = new Inputs(firstInput);

			std::string secondInput;
			secondInput.append(abs(move.first), horizontalButton);
			secondInput.append(abs(move.second), verticalButton);
			secondInput.append("A");
			Inputs* rightBranch = new Inputs(secondInput);

			outputInputs->SetChild(0, leftBranch);
			outputInputs->SetChild(1, rightBranch);

			// Regroup
			Inputs* newBranch = new Inputs();
			leftBranch->SetChild(0, newBranch);
			rightBranch->SetChild(0, newBranch);
			outputInputs = newBranch;
		}
		robotPos = nextKeyPos;
	}

	if (!sequenceMap.count(sequenceId))
	{
		Inputs storedSequence;
		storedSequence.CloneFrom(*start);
		sequenceMap[sequenceId] = storedSequence;
	}

	outputInputs->children = original.children;
}


using Stack = std::stack<std::pair<Inputs*, coord>>;
void FindInputs(Inputs& inputsOutput, const Keypad& keypad, const coord start = coord(0, 0))
{
	std::set<const Inputs*> inputsSeen;
	Stack stack;
	stack.push(std::make_pair(&inputsOutput, start));

	while (!stack.empty())
	{
		auto it = stack.top();
		stack.pop();

		coord robotPos(it.second);	// start pointing at 'A'.
		inputsSeen.insert(it.first);
		Inputs* outputInputs = it.first;
		Inputs original = *outputInputs;
		ExpandInputs(outputInputs, robotPos, keypad);
		for (int i = 0; i < 2; ++i)
		{
			Inputs* child = original.GetChild(i);

			if (!child)
				continue;
			if (inputsSeen.count(child) == 0)
			{
				stack.push(std::make_pair(child, robotPos));
			}
		}
	}
}

using ScanningCache = std::map<std::pair<const Inputs*, int>, int64>;

ScanningCache scanningCache;

int64 GetShortestSolution(const Inputs& inputs, int robotCount)
{
	auto cacheId = std::make_pair(&inputs, robotCount);
	if (scanningCache.count(cacheId))
	{
		return scanningCache[cacheId];
	}
	const std::string& fragment = inputs.GetFragment();
	int64 score = 0;
	if (robotCount == 0)
	{
		score = fragment.length();
	}
	if (robotCount > 0)
	{
		if (!sequenceMap.count(inputs.fragmentHash))
		{
			coord robotPos;
			Inputs expanded = inputs;
			ExpandInputs(&expanded, robotPos, directionalKeypad);
			assert(sequenceMap.count(inputs.fragmentHash));
		}
		const Inputs& deeperInputs = sequenceMap[inputs.fragmentHash];
		score += GetShortestSolution(deeperInputs, robotCount - 1);
	}
	if (inputs.CountChildren())
	{
		int64 childScore = INT64_MAX;
		for (int i = 0; i < 2; ++i)
		{
			Inputs* child = inputs.GetChild(i);
			if (!child)
				break;
			int64 thisScore = GetShortestSolution(*child, robotCount);
			if (thisScore < childScore)
				childScore = thisScore;
		}
		score += childScore;
	}
	scanningCache[cacheId] = score;
	return score;
}

int64 CalculateComplexity(const std::string& code, int64 sequenceLength)
{
	int64 numericPart = atoll(code.c_str());
	int64 complexity = numericPart * sequenceLength;
	return complexity;
}

#if USE_BRUTE_FORCE
int64 CalculateComplexity(const std::string& code, const Inputs& prunedInputs)
{
	int64 sequenceLength = prunedInputs.Length(true);
	return CalculateComplexity(code, sequenceLength);
}
#endif

int64 Calculate(const Codes& codes, int robotCount = 2)
{
	int64 totalScore = 0;
	for (const std::string& code : codes)
	{
		// One numeric keypad(on a door) that a robot is using.
		// This is the code
		printf("Calculating for code %s with %d robots\n", code.c_str(), robotCount);

		// Two/25 directional keypads that robots are using.
		// One directional keypad that you are using.
		Inputs inputs;
		inputs.SetFragment(code);
		FindInputs(inputs, numericKeypad);

#if USE_BRUTE_FORCE
		// Brute force algorithm: expand the input for each robot. This gets stuck with robots > ~16
		for (int i = 0; i < robotCount; ++i)
		{
			printf("%d\r", i + 1);
			FindInputs(inputs, directionalKeypad);
		}

		int64 complexity = CalculateComplexity(code, inputs);
#else
		// Rather than actually expand the input graph for each successive robot, *just* look up what each sequence expands into, recursively, to depth robotCount
		scanningCache.clear();
		int64 shortestLength = GetShortestSolution(inputs, robotCount);
		int64 complexity = CalculateComplexity(code, shortestLength);
#endif
		totalScore += complexity;
		inputs.Destroy();
		printf("Complexity: %lld\n", complexity);
	}
	return totalScore;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Codes codes;
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				codes.push_back(line);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = Calculate(codes);
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (expectedPart2 != -2)
	{
		int64 part2 = Calculate(codes, 25);
		assert(expectedPart2 == -1 || expectedPart2 == part2);
		printf("%s: Part 2: %lld\n", filename, part2);
	}
}

int main()
{
	Process("example.txt", 126384, -2);
	Process("input.txt");

	return 0;
}
