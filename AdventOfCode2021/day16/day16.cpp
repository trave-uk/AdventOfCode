// day16.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 16/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

int64 Value(std::string binary)
{
	assert(binary.length() < 64);
	int64 result = 0;
	std::reverse(binary.begin(), binary.end());
	while (!binary.empty())
	{
		char b = binary.back();
		binary.pop_back();
		result *= 2;
		result += (b == '1');
	}
	return result;
}

int64 DecodePacket(std::string& binary, int& pos, int64& versionSum)
{
	int64 value = 0;
	std::string versionString = binary.substr(pos, 3);
	int64 version = Value(versionString);
	versionSum += version;
	pos += 3;

	std::string typeString = binary.substr(pos, 3);
	int64 type = Value(typeString);
	pos += 3;

	if (type == 4) // literal value
	{
		bool bKeepGoing;
		std::string literalValue;
		do 
		{
			bKeepGoing = binary[pos++] == '1';
			literalValue.append(binary.substr(pos, 4));
			pos += 4;
		} while (bKeepGoing);
		value = Value(literalValue);
	}
	else // operators
	{
		std::vector<int64> values;
		char mode = binary[pos++];
		if (mode == '0')
		{
			std::string lengthString = binary.substr(pos, 15);
			pos += 15;
			int64 length = Value(lengthString);
			int start = pos;
			while (pos < start + length)
			{
				values.push_back(DecodePacket(binary, pos, versionSum));
			}
			assert(pos == start + length);
		}
		else
		{
			assert(mode == '1');
			std::string countString = binary.substr(pos, 11);
			pos += 11;
			int64 count = Value(countString);
			while (count)
			{
				values.push_back(DecodePacket(binary, pos, versionSum));
				--count;
			}
		}
		switch (type)
		{
		case 0: // sum
			value = 0;
			for (int64 v : values)
			{
				value += v;
			}
			break;
		case 1: // product
			value = 1;
			for (int64 v : values)
			{
				value *= v;
			}
			break;
		case 2: // minimum
			value = INT64_MAX;
			for (int64 v : values)
			{
				value = min(v, value);
			}
			break;
		case 3: // maximum
			value = 0;
			for (int64 v : values)
			{
				value = max(v, value);
			}
			break;
		case 5: // gt
			assert(values.size() == 2);
			value = values[0] > values[1];
			break;
		case 6: // lt
			assert(values.size() == 2);
			value = values[0] < values[1];
			break;
		case 7: // equal to
			assert(values.size() == 2);
			value = values[0] == values[1];
			break;
		default:
			assert(false);
		}
	}
	return value;
}

void Process(const char* filename, bool bPart1 = true, bool bPart2 = true)
{
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
				// Input is a hex number, containing bit-packed data
				// Note input.txt is 1300 odd hex digits long
				// May as well decode this into a 4 times longer string containing 1s and 0s.
				std::string binary;
				for (int i = 0; i < strlen(thisLine); ++i)
				{
					char h = thisLine[i];
					assert(isalnum(h));
					int value = (h <= '9') ? (h - '0') : (h - 'A' + 10);
					std::string nibble;
					for (int bit = 0; bit < 4; ++bit)
					{
						int v = value % 2;
						value /= 2;
						nibble.append(v ? "1" : "0");
					}
					std::reverse(nibble.begin(), nibble.end());
					binary.append(nibble);
				}

				int64 versionSum = 0;
				int pos = 0;
				int64 result = DecodePacket(binary, pos, versionSum);
				if ( bPart1 )
					printf("%s has version sum of %lld\n", (strlen(thisLine) < 100) ? thisLine : "\nPart 1", versionSum);
				if ( bPart2)
					printf("%s produces %lld\n", (strlen(thisLine) < 100) ? thisLine : "Part 2", result);
			}
		}
	}
	fclose(fp);
	delete[] buffer;
}

int main()
{
	Process("exampleA.txt", true, true);
	Process("exampleB.txt", true, true);
	Process("exampleC.txt", true, true);

	Process("example1.txt", true, false);
	Process("example2.txt", true, false);
	Process("example3.txt", true, false);
	Process("example4.txt", true, false);

	Process("example21.txt", false, true);
	Process("example22.txt", false, true);
	Process("example23.txt", false, true);
	Process("example24.txt", false, true);
	Process("example25.txt", false, true);
	Process("example26.txt", false, true);
	Process("example27.txt", false, true);
	Process("example28.txt", false, true);

	Process("input.txt");

	return 0;
}
