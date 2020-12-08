// day08.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// registers
// 0 = program counter
// 1 = accumulator
enum REGISTER
{
	REG_PC = 0,
	REG_ACC = 1,
	NUM_REGS
};
std::array<int, NUM_REGS> registers;

void reset()
{
	for (int i = 0; i < NUM_REGS; ++i)
	{
		registers[i] = 0;
	}
}

// opcodes
// acc (modify accumulator).
void acc(int value)
{
	registers[REG_ACC] += value;
	registers[REG_PC] += 1;
}

// jmp jumps by value
void jmp(int value)
{
	registers[REG_PC] += value;
}

// nop does nothing
void nop(int value)
{
	registers[REG_PC] += 1;
}

enum OPCODE
{
	OP_ACC,
	OP_JMP,
	OP_NOP,
	NUM_OPS
};

std::array<const char*, NUM_OPS> opnames = {
	"acc",
	"jmp",
	"nop"
};

typedef void(*OpCode)(int);
std::array<OpCode, NUM_OPS> operations = {
	acc,
	jmp,
	nop
};

// commands are read in as opcode / value, and stored as index / value
struct Command
{
	OPCODE opCode = NUM_OPS;
	int value = 0;
};
std::vector<Command> program;

void operate(const Command& com)
{
	operations[com.opCode](com.value);
}

void load(const char* filename)
{
	printf("Loading %s\n", filename);

	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			char* op = strtok(thisLine, " ");
			char* val = strtok(nullptr, " ");
			Command com;
			for (int i = 0; i < NUM_OPS; ++i)
			{
				if (_stricmp(op, opnames[i]) == 0)
				{
					com.opCode = OPCODE(i);
					break;
				}
			}
			com.value = atoi(val);
			program.push_back(com);
		}
	}
	fclose(fp);
	delete[] buffer;
}

bool process( int& result)
{
	std::set<int> seen;
	// Part 1: run until a loop
	while (true)
	{
		if (seen.count(registers[REG_PC]) > 0)
		{
			// Seen this line before
			result = registers[REG_ACC];
			return false;
		}
		else if (registers[REG_PC] == program.size())
		{
			result = registers[REG_ACC];
			return true;
		}
		else if (registers[REG_PC] > program.size())
		{
			result = registers[REG_ACC];
			return false;
		}
		seen.insert(registers[REG_PC]);
		Command com = program[registers[REG_PC]];
		operate(com);
	} 
}

int main()
{
	//load("example.txt");
	load("input.txt");

	int result;
	process(result);

	printf( "Part 1: %d\n", result );

	std::vector<Command> program2 = program; // backup
	for (int i = 0; i < program.size(); ++i)
	{
		program = program2;
		if (program[i].opCode == OP_JMP)
		{
			program[i].opCode = OP_NOP;
		}
		else if (program[i].opCode == OP_NOP)
		{
			program[i].opCode = OP_JMP;
		}
		else
		{
			continue;
		}
		int result2;
		reset();
		if (process(result2))
		{
			printf("Part 2: %d\n", result2);
		}
	}

	return 0;
}
