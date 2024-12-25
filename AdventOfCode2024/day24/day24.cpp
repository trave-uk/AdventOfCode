// day24.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 24/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum Operator
{
	NONE,
	AND,
	OR,
	XOR,
};

enum Value
{
	KNOWN_FALSE,
	KNOWN_TRUE,
	UNKNOWN
};

struct Wire;
using Wires = std::map<std::string, Wire>;

struct Wire
{
	Wire(std::string name = "", Value value = UNKNOWN, Operator op = NONE, std::string input1 = "", std::string input2 = "") :
		name(name),
		value(value),
		op(op),
		input1(input1),
		input2(input2)
	{}

	bool GetValue(Wires& wires, int64 depth = 0);

	std::string GetId()
	{
		switch (op)
		{
		case NONE:
			return name;
		case AND:
			return string_format("%s[%s AND %s]", name.c_str(), input1.c_str(), input2.c_str());
		case OR:
			return string_format("%s[%s OR %s]", name.c_str(), input1.c_str(), input2.c_str());
		case XOR:
			return string_format("%s[%s XOR %s]", name.c_str(), input1.c_str(), input2.c_str());
		}
		assert(false);
		return name;
	}

	bool InputsAreXY(int index=-1)
	{
		if (!isdigit(input1[1]) || !isdigit(input2[1]))
			return false;
		if (index == -1)
			return true;
		int index1 = atoi(input1.c_str() + 1);
		int index2 = atoi(input2.c_str() + 1);
		return index == index1 && index == index2;
	}

	std::string name;
	Value value;
	Operator op;
	std::string input1;
	std::string input2;
};

bool Wire::GetValue(Wires& wires, int64 depth)
{
	const int64 wiresSize = wires.size();
	if (depth > wiresSize)
	{
		return true;
	}
	if (value == UNKNOWN)
	{
		int64 nextDepth = depth + 1;
		Wire& in1 = wires[input1];
		Wire& in2 = wires[input2];
		bool newValue = false;
		switch (op)
		{
		case AND:
			newValue = (in1.GetValue(wires, nextDepth) && in2.GetValue(wires, nextDepth)); break;
		case OR:
			newValue = (in1.GetValue(wires, nextDepth) || in2.GetValue(wires, nextDepth)); break;
		case XOR:
			newValue = (in1.GetValue(wires, nextDepth) ^ in2.GetValue(wires, nextDepth)); break;
		default:
			assert(false);
		}
		value = static_cast<Value>(newValue);
	}
	return static_cast<bool>(value);
}

int64 CalculateZ(const Wires& wires_)
{
	Wires wires = wires_;
	
	int64 output = 0;
	int64 bit = 1;
	for (int i = 0; i < 63; ++i)
	{
		std::string outputName = string_format("z%02d", i);
		if (wires.count(outputName) == 0)
		{
			break;
		}
		Wire& wire = wires[outputName];
		bool value = wire.GetValue(wires);
		int64 value64 = ((int64)value) * bit;
		output += value64;
		bit <<= 1;
	}
	return output;
}

int64 CalculateZfromXY(const Wires& wires_, int64 x, int64 y)
{
	Wires wires = wires_;
	for (int i = 0; i < 63; ++i)
	{
		std::string inputName = string_format("x%02d", i);
		int64 bit = x & 1;
		x >>= 1;
		if (wires.count(inputName))
		{
			wires[inputName].value = bit ? KNOWN_TRUE : KNOWN_FALSE;
		}

		inputName = string_format("y%02d", i);
		bit = y & 1;
		y >>= 1;
		if (wires.count(inputName))
		{
			wires[inputName].value = bit ? KNOWN_TRUE : KNOWN_FALSE;
		}
	}
	return CalculateZ(wires);
}

bool TestCircuit(const Wires& wires)
{
	// with 45 input bits and 46 output bits
	int64 maxInput = 1; maxInput <<= 45; --maxInput;
	int64 x = maxInput;
	int64 y = 1;
	int64 expectedZ = x + y;

	int64 z = CalculateZfromXY(wires, x, y);
	if (z != expectedZ)
		return false;

	z = CalculateZfromXY(wires, y, x);
	if (z != expectedZ)
		return false;

	expectedZ = maxInput * 2;;
	z = CalculateZfromXY(wires, maxInput, maxInput);
	if (z != expectedZ)
		return false;

	for (int i = 0; i < 64; ++i)
	{
		x = rand() % (maxInput + 1);
		y = rand() % (maxInput + 1);
		expectedZ = x + y;

		z = CalculateZfromXY(wires, x, y);
		if (z != expectedZ)
			return false;

		z = CalculateZfromXY(wires, y, x);
		if (z != expectedZ)
			return false;
	}

	return true;
}

using Swaps = std::set<std::string>;

void SwapWires(Wires& wires, const std::string& name1, const std::string& name2)
{
	Wire wire1 = wires[name1];
	Wire wire2 = wires[name2];
	wires[name1] = wire2;
	wires[name2] = wire1;
}

using Pair = std::pair<std::string, std::string>;
using Pairs = std::vector<Pair>;

bool TestWiresToSwap(const Wires& wires_, const Swaps& swapWires_, const Pairs& pairs_ = Pairs())
{
	if (!swapWires_.empty())
	{
		Pair pair;
		for (std::string swapWire1 : swapWires_)
		{
			pair.first = swapWire1;
			Swaps swapWires1 = swapWires_;
			swapWires1.erase(swapWire1);
			for (std::string swapWire2 : swapWires1)
			{
				pair.second = swapWire2;
				Pairs pairs = pairs_;
				pairs.push_back(pair);
				Swaps swapWires2 = swapWires1;
				swapWires2.erase(swapWire2);
				Wires wires = wires_;
				SwapWires(wires, swapWire1, swapWire2);
				bool found = TestWiresToSwap(wires, swapWires2, pairs);
				if (found)
					return true;
			}
		}
	}
	else
	{
		static int count = 0;
		++count;
		if (TestCircuit(wires_))
		{
			printf("Swap these wires:\n");
			for (const Pair& pair : pairs_)
			{
				printf(" %s,%s\n", pair.first.c_str(), pair.second.c_str());
			}
			return true;
		}
	}
	return false;
}

std::string CalculatePart2(const Wires& wires_)
{
	Wires wires = wires_;
	// Swap the inputs between (numSwaps) pairs of wires.
	// Step back: there are trillions of possible combinations with 222 wires with inputs.
	// Also, no point working for the AND example. Concentrate on ADD
	// To add two binary numbers, there must be an "add with carry" for each bit.
	// This will look something like:
	// 
	// carry(n) = (x(n) AND y(n)) OR (carry(n-1) AND (x(n) XOR y(n)); // 4 operations
	// z(n) = x(n) XOR y(n) XOR carry(n-1); // 2 operations, but (x(n) XOR y(n)) already calculated above so 1
	//
	// the lowest bit is just:
	// 
	// carry(0) = x(0) AND y(0) // 1 operation
	// z(0) = x(0) XOR y(0); // 1 operation
	//
	// the highest bit is just
	//
	// z(max) = carry(max-1) // 0 operations: can just store the carry bit directly in z(max)
	// 
	// So for 45 bits input, 46 bits output, this needs (2 + 5*44) operations = 222 operations(!)
	//
	// Now, look at the XOR operations. z(n) = [x(n) XOR y(n)] XOR [carry(n-1)].
	// We can find all the [x(n) XOR y(n)] wires and check that those are XORed with another wire to go into z(n). That other wire _should_ be carry(n-1)
	//
	// let's give names to all the temporaries, to identify them
	//
	// carry(0) = x(0) AND y(0)						// AND: named inputs: identifiable
	// z(0) = x(0) XOR y(0);						// XOR: named inputs: identifiable
	// 
	// zTemp(n) = x(n) XOR y(n)						// XOR: named inputs: identifiable
	// z(n) = zTemp(n) XOR carry(n-1)				// XOR: unnamed inputs: all other XORs
	// carryTemp1(n) = x(n) AND y(n)				// AND: named inputs: identifiable
	// carryTemp2(n) = carry(n-1) AND zTemp(n)		// AND: unnamed inputs: all other ANDs
	// carry(n) = carryTemp1(n) OR carryTemp2(n)	// OR: all ORs; both inputs are AND operations, one on identifiable inputs the other not identifiable

	// So: look for mismatched patterns
	std::set<std::string> swapWires;
	for (auto& it : wires)
	{
		Wire& wire = it.second;
		if (wire.op != NONE)
		{
			Wire& wire1 = wires[wire.input1];
			Wire& wire2 = wires[wire.input2];
			if (wire.name[0] == 'z')
			{
				if (wire.name == "z45")
				{
					if (wire.op != OR)
					{
						printf("Bad operator on output: %s should be OR", wire.GetId().c_str());
						swapWires.insert(wire.name);
					}
				}
				// z(n) = zTemp(n) XOR carry(n-1)
				else if (wire.op != XOR)
				{
					printf("Bad operator on output: %s should be XOR\n", wire.GetId().c_str());
					swapWires.insert(wire.name);
				}
			}
			switch (wire.op)
			{
			case AND:
			{
				// carryTemp1(n) = x(n) AND y(n)				// AND: named inputs: identifiable
				// carryTemp2(n) = carry(n-1) AND zTemp(n)		// AND: unnamed inputs: all other ANDs
				//                               [zTemp(n) = x(n) XOR y(n)]
				//                 carry(n) = carryTemp1(n) OR carryTemp2(n)
				if (wire.InputsAreXY())	// carryTemp1(n) = x(n) AND y(n)
					continue;
				// carryTemp2(n) = carry(n-1) AND zTemp(n)
				if (wire1.InputsAreXY() && wire1.op == XOR) // zTemp(n) = x(n) XOR y(n)	
				{
					if (wire2.op == AND && wire2.InputsAreXY()) // carry(0) = x(0) AND y(0)
					{
						continue;	
					}
					if (wire2.op == OR) // carry(n) = carryTemp1(n) OR carryTemp2(n)
					{
						continue;
					}
					printf("Found bad input to AND %s: input %s should be OR or [x0 AND y0]\n", wire.GetId().c_str(), wire2.GetId().c_str());
					swapWires.insert(wire2.name);
				}
				if (wire2.InputsAreXY() && wire2.op == XOR) // zTemp(n) = x(n) XOR y(n)	
				{
					if (wire1.op == AND && wire1.InputsAreXY()) // carry(0) = x(0) AND y(0)
					{
						continue;
					}
					if (wire1.op == OR) // carry(n) = carryTemp1(n) OR carryTemp2(n)
					{
						continue;
					}
					printf("Found bad input to AND %s: input %s should be OR or [x0 AND y0]\n", wire.GetId().c_str(), wire1.GetId().c_str());
					swapWires.insert(wire1.name);
				}
				break;
			}
			case OR:
			{
				// carry(n) = carryTemp1(n) OR carryTemp2(n)	// OR: all ORs; both inputs are AND operations, one on identifiable inputs the other not identifiable
				// carryTemp1(n) = x(n) AND y(n)				// AND: named inputs: identifiable
				// carryTemp2(n) = carry(n-1) AND zTemp(n)		// AND: unnamed inputs: all other ANDs
				//                               [zTemp(n) = x(n) XOR y(n)]
				//                 carry(n) = carryTemp1(n) OR carryTemp2(n)
				
				// Check both inputs are AND
				if (wire1.op != AND)
				{
					printf("Found bad input to OR %s: input %s should be AND\n", wire.GetId().c_str(), wire1.GetId().c_str());
					swapWires.insert(wire1.name);
				}
				if (wire2.op != AND)
				{
					printf("Found bad input to OR %s: input %s should be AND\n", wire.GetId().c_str(), wire2.GetId().c_str());
					swapWires.insert(wire2.name);
				}
				break;
			}
			case XOR:
			{
				// zTemp(n) = x(n) XOR y(n)						// XOR: named inputs: identifiable
				// z(n) = zTemp(n) XOR carry(n-1)				// XOR: unnamed inputs: all other XORs
				
				// Either this is an XOR of XY inputs OR it is an XOR of [an XOR of XY inputs] and [[x0 AND y0] or an OR]
				if (!wire.InputsAreXY())
				{
					Wire& wire1 = wires[wire.input1];
					Wire& wire2 = wires[wire.input2];
					if (wire1.InputsAreXY() && wire1.op == XOR)
					{
						if (wire2.InputsAreXY() && wire2.op == AND)
						{
							continue;
						}
						if (wire2.op == OR)
						{
							continue;
						}
						printf("Found bad input to XOR %s: input %s should be OR or [x0 AND y0]\n", wire.GetId().c_str(), wire2.GetId().c_str());
						swapWires.insert(wire2.name);
					}
					else
					{
						if (wire1.op != OR && (wire1.op != AND || !wire1.InputsAreXY(0)))
						{
							printf("Found bad input to XOR %s: input %s should be XOR, OR or x(n) AND y(n)\n", wire.GetId().c_str(), wire1.GetId().c_str());
							swapWires.insert(wire1.name);
						}
					}
					if (wire2.InputsAreXY() && wire2.op == XOR)
					{
						if (wire1.InputsAreXY() && wire1.op == AND)
						{
							continue;
						}
						if (wire1.op == OR)
						{
							continue;
						}
						printf("Found bad input to XOR %s: input %s should be OR or [x0 AND y0]\n", wire.GetId().c_str(), wire1.GetId().c_str());
						swapWires.insert(wire1.name);
					}
					else
					{
						if (wire2.op != OR && (wire2.op != AND || !wire2.InputsAreXY(0)))
						{
							printf("Found bad input to XOR %s: input %s should be XOR, OR or x(n) AND y(n)\n", wire.GetId().c_str(), wire2.GetId().c_str());
							swapWires.insert(wire2.name);
						}
					}
				}
				break;
			}
			default:
				assert(false);
			}
		}
	}
	
	// From the list of wires, attempt swapping them in all permutations to see if it works
	bool found = TestWiresToSwap(wires, swapWires);
	assert(found);

	// Generate the list of wires as a string
	std::string output = "";
	for (std::string wire : swapWires)
	{
		if (output.length() == 0)
			output = wire;
		else
		{
			output.append(1, ',');
			output.append(wire);
		}
	}

	printf("Found %lld wires to swap\n", swapWires.size());
	assert(swapWires.size() == 8);
	return output;
}

void Process(const char* filename, int64 expectedPart1 = -1, bool doPart2 = true)
{
	Wires wires;

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
				if (line[3] == ':')
				{
					// x00: 1
					char* name = strtok(thisLine, ": ");
					char* value = strtok(nullptr, ": ");
					wires[name] = Wire(name, static_cast<Value>(atoi(value)));;
				}
				else
				{
					// x00 AND y00 -> z00
					char* input1 = strtok(thisLine, " ");
					char* opName = strtok(nullptr, " ");
					char* input2 = strtok(nullptr, " ->");
					char* name = strtok(nullptr, " ->");
					Operator op = (opName[0] == 'A') ? AND : (opName[0] == 'O') ? OR : (opName[0] == 'X') ? XOR : NONE;
					assert(op != NONE);
					wires[name] = Wire(name, UNKNOWN, op, input1, input2);;
					if (!wires.count(input1))
					{
						wires[input1] = Wire(input1);
					}
					if (!wires.count(input2))
					{
						wires[input2] = Wire(input2);
					}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = CalculateZ(wires);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	if (doPart2)
	{
		std::string part2 = CalculatePart2(wires);

		printf("%s: Part 2: %s\n", filename, part2.c_str());
	}
}

int main()
{
	Process("example1.txt", 4, false);
	Process("example2.txt", 2024, false);
	Process("input.txt");

	return 0;
}
