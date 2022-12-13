// day13.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 13/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum TYPE
{
	INVALID,
	NUMBER,
	LIST
};

struct Object
{
	Object() = default;
	Object(std::string input);
	Object(int64 num)
	{
		type = TYPE::LIST;
		Object o;
		o.type = TYPE::NUMBER;
		o.number = num;
		list.push_back(o);
	}
	Object Read(std::string input, int& index);
	TYPE type = TYPE::INVALID;
	int64 number = -1;
	std::vector<Object> list;
	bool operator<(const Object& other) const;
	bool operator>(const Object& other) const { return other < *this; }
	bool operator==(const Object& other) const { return !(other < *this) && !(*this < other); }
};

bool Object::operator<(const Object& other) const
{
	if (type == TYPE::NUMBER && other.type == TYPE::NUMBER)
	{
		return number < other.number;
	}
	else if (type == TYPE::LIST && other.type == TYPE::LIST)
	{
		size_t size = max(list.size(), other.list.size());
		for (size_t i = 0; i < size; ++i)
		{
			if (i >= other.list.size())
			{
				return false;
			}
			else if (i >= list.size())
			{
				return true;
			}
			else if (list[i] < other.list[i])
			{
				return true;
			}
			else if (list[i] > other.list[i])
			{
				return false;
			}
		}
		// If this is reached, then these two lists are identical, so neither is smaller than the other
		return false;
	}
	else if (type == TYPE::NUMBER)
	{
		Object num(number);
		return num < other;
	}
	else
	{
		Object otherNum(other.number);
		return *this < otherNum;
	}
}

Object::Object(std::string input)
{
	int index = 0;
	Object o = Read(input, index);
	*this = o;
}

Object Object::Read(std::string input, int& index)
{
	Object o;
	for (; index < input.length(); ++index)
	{
		char c = input[index];
		switch (c)
		{
		case '[':
		{
			assert(o.type == TYPE::INVALID);
			o.type = TYPE::LIST;
			++index;
			Object p = Read(input, index);
			if (p.type != TYPE::INVALID)
			{
				o.list.push_back(p);
			}
			--index;
			break;
		}
		case ']':
		{
			if (o.type == TYPE::LIST)
				++index;
			return o;
		}
		case ',':
		{
			assert(o.type != TYPE::INVALID);
			if (o.type == TYPE::NUMBER)
			{
				return o;
			}
			else
			{
				++index;
				Object p = Read(input, index);
				o.list.push_back(p);
				--index;
			}
			break;
		}
		default:
		{
			assert(isdigit(c));
			if (o.type == TYPE::INVALID)
			{
				o.type = TYPE::NUMBER;
				o.number = c - '0';
			}
			else
			{
				assert(o.type == TYPE::NUMBER);
				o.number *= 10;
				o.number += c - '0';
			}
		}
		}
	}
	return o;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	Object left;
	int index = 1;
	std::set<Object> objects;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				Object object(thisLine);
				objects.insert(object);
				if (left.type == TYPE::INVALID)
					left = object;
				else
				{
					if (left < object)
					{
						part1 += index;
					}
					else
					{
						assert(object < left);
					}
					left.type = TYPE::INVALID;
					++index;
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	Object d1("[[2]]");
	Object d2("[[6]]");
	objects.insert(d1);
	objects.insert(d2);
	index = 1;
	part2 = 1;
	for (auto& o : objects)
	{
		if (o == d1 || o == d2)
		{
			part2 *= index;
		}
		++index;
	}

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 13, 140);
	Process("input.txt");

	return 0;
}
