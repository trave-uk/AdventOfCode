// day20.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 20/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using File = std::vector<std::pair<int64,int64>>;

int64 GetResult(const File& file)
{
	const int64 size = file.size();
	// Find 0
	int index0 = 0;
	while (file[index0].second != 0)
		++index0;
	assert(file[index0].second == 0);
	int64 first = file[(index0 + 1000) % size].second;
	int64 second = file[(index0 + 2000) % size].second;
	int64 third = file[(index0 + 3000) % size].second;
	int64 result = first + second + third;
	return result;

}

void Shuffle(File& file, int64 num, int64 index)
{
	auto orig = file[index];
	const int64 size = file.size();
	// Move num by num
	int64 move = num % (size - 1);
	if (move < 0)
		move += size - 1;
	int64 newIndex = (index + move) % (size - 1);
	if (newIndex < 0)
		newIndex += size - 1;

	if (newIndex < index)
	{
		memmove(&file[newIndex + 1], &file[newIndex], (index - newIndex) * sizeof(file[0]));
		file[newIndex] = orig;
	}
	else if (newIndex > index)
	{
		memmove(&file[index], &file[index + 1], (newIndex - index) * sizeof(file[0]));
		file[newIndex] = orig;
	}
}

void Process(const char* filename, int64 encryptionKey, int loop, int64 expectedResult = -1)
{
	File file;
	int64 result = 0;
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	int64 index = 0;
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				int64 num = atoi(thisLine) * encryptionKey;
				file.push_back(std::make_pair(index, num));
				++index;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// Need the original order so make a copy to iterate
	File copy = file;
	int64 size = file.size();
	for (int l = 0; l < loop; ++l)
	{
		index = 0;
		for (auto pair : copy)
		{
			assert(pair.first == index);
			int64 num = pair.second;
			if (!num)
			{
				++index;
				continue;
			}

			// Find num in file. It was originally at index but will have moved, though not necessarily very far.
			// Therefore, search in a 2d "spiral" from index for the number
			int64 index2 = index, i = -1;
			for (; file[index2].first != index; index2 += i, index2 = (index2 + size) % size, i += sgn(i), i = -i)
				;
			assert(file[index2].second == num);

			Shuffle(file, num, index2);

			++index;
		}
	}

	int part = (encryptionKey == 1) ? 1 : 2;
	result = GetResult(file);
	if (expectedResult != -1)
	{
		printf("%s: Part %d expected: %lld\n", filename, part, expectedResult);
	}
	printf("%s: Part %d: %lld\n", filename, part, result);
	assert(expectedResult == -1 || expectedResult == result);
}

int main()
{
	Process("example.txt", 1, 1, 3);
	Process("example.txt", 811589153, 10, 1623178306);
	Process("input.txt", 1, 1);
	Process("input.txt", 811589153, 10);

	return 0;
}
