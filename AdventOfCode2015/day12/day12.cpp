// day12.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 26/1/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 Count(char* thisLine, char* endLine = nullptr)
{
	int64 total = 0;
	char *tok = thisLine;
	do
	{
		tok += strcspn(tok, "-0123456789");
		total += atoll(tok);
		tok += strspn(tok, "-0123456789");
	} while (*tok && (endLine == nullptr || tok < endLine));
	return total;
}

int64 Count2(std::string line)
{
	struct h
	{
		h(char _t) : type(_t) {}
		char type;	// {, [, " or '
		bool illegal = false;
		int64 value = 0;
	};
	std::stack<h> history;
	char top = '.';
	history.push(top);
	std::string text;
	bool key = false;
	bool negative = false;
	int64 value = 0;
	for (char& c : line)
	{
		h& _current = history.top();
		if (top == '"' || top == '\'')
		{
			if (c != top)
			{
				text.append(1, c);
			}
			else
			{
				history.pop();
				h& _top = history.top();
				top = _top.type;
				if (top == '{' && !key && text == "red")
				{
					_top.illegal = true;
				}
			}
			continue;
		}

		switch (c)
		{
		case '"':
		case '\'':
			text.clear();
			top = c;
			history.push(c);
			break;

		case '{':
		case '[':
			key = true;
			_current.value += value;
			negative = false;
			value = 0;
			top = c;
			history.push(c);
			break;

		case ':':
			assert(top == '{');
			key = false;
			negative = false;
			_current.value += value;
			value = 0;
			break;

		case ',':
			assert(top == '{' || top == '[');
			key = true;
			_current.value += value;
			negative = false;
			value = 0;
			break;

		case '-':
			negative = true;
			break;

		case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
			value *= 10;
			if (negative)
				value -= c - '0';
			else
				value += c - '0';
			break;

		case ']':
		{
			assert(top == '[');
			_current.value += value;
			negative = false;
			value = _current.value;
			history.pop();
			h& _top = history.top();
			_top.value += value;
			value = 0;
			top = _top.type;
			break;
		}
		case '}':
		{
			assert(top == '{');
			_current.value += value;
			negative = false;
			value = _current.illegal ? 0 : _current.value;
			history.pop();
			h& _top = history.top();
			_top.value += value;
			value = 0;
			top = _top.type;
			break;
		}

		default:
			assert(false);
		}
	}
	h& _current = history.top();
	assert(_current.type == '.');
	return _current.value;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
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
				part1 = Count(thisLine);
				part2 = Count2(thisLine);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 6, 4);
	Process("input.txt");

	return 0;
}
