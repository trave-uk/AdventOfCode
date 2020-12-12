// day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

// Start with
//
// .#.
// ..#
// ###

// 5 iterations
// 3 -3> 4 -2> 6 -2> 9 -3> 12 -2> 18
// 18 iterations? Just make it bigger and assert
const int max = 10000;
int size = 3;
char map[max][max] = { ".#.", "..#", "###" };

struct Rule
{
public:
	void SetOutput(const std::string& out) { output = out; }

	const std::string& Output() { return output; }

	bool Match(std::string search)
	{
		for (int i = 0; i < 8; ++i)
		{
			if (search == input[i])
				return true;
		}
		return false;
	}

	void GenerateInputs(const std::string& input0)
	{
		input[0] = input0;
		for (int i = 1; i < 8; ++i)
		{
			std::string& x = input[i];
			x = input[0];
			if (i >= 4)
			{
				// flip
				char* first = const_cast<char*>(input[i].data());
				for(;;)
				{
					char* sep = first + strcspn(first, "/");
					char* last = sep - 1;
					char t = *last;
					*last = *first;
					*first = t;
					if (!*sep)
						break;
					first = sep + 1;
				}
			}
			// now rotate
			int rotate = i % 4;
			for (int j = 0; j < rotate; ++j)
			{
				int sz;
				if (x.length() == 11)
				{
					// abc/def/ghi => gda/heb/ifc
					sz = 3;
				}
				else
				{
					// ab/cd => ca/db
					assert(x.length() == 5);
					sz = 2;
				}
				std::string copy = x;
				#define ix(k,l) ((k)*(sz+1) + (l))
				for (int k = 0; k < sz; ++k)
				{
					for (int l = 0; l < sz; ++l)
					{
						int i1 = ix(k, l);
						int i2 = ix(sz - 1 - l, k);
						x[i1] = copy[i2];
					}
				}
			}
		}
	}

private:
	std::string input[8];
	std::string output;

};

std::vector<Rule> rules;

int Expand(int from, int to)
{
	int newsize = (size / from) * to;
	assert(newsize < max);
	for (int x = size - from; x >= 0; x -= from)
	{
		int nx = (x / from) * to;
		for (int y = size - from; y >= 0; y -= from)
		{
			std::string search;
			int ny = (y / from) * to;
			for (int dx = 0; dx < from; ++dx)
			{
				for (int dy = 0; dy < from; ++dy)
				{
					search += map[y + dy][x + dx];
				}
				if (dx < from-1)
				{
					search += '/';
				}
			}
			for (Rule r : rules)
			{
				if (r.Match(search))
				{
					const std::string& output = r.Output();
					for (int dy = 0; dy < to; ++dy)
					{
						for (int dx = 0; dx < to; ++dx)
						{
							map[ny+dy][nx+dx] = output[dy*(to+1)+dx];
						}
					}
					break;
				}
			}
		}
	}
	return newsize;
}

int main()
{
	for (int part = 1; part <= 2; ++part)
	{
		size = 3;
		strcpy(map[0], ".#.");
		strcpy(map[1], "..#");
		strcpy(map[2], "###");

		char* buffer = new char[65536];
		int iterations = 2;
		//FILE *fp = fopen( "example.txt", "rt" ); 
		FILE *fp = fopen("input.txt", "rt"); iterations = (part == 1) ? 5 : 18;
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (*thisLine)
				{
					char* input = strtok(thisLine, " => ");
					char* output = strtok(nullptr, " => ");
					Rule r;
					r.GenerateInputs(std::string(input));
					r.SetOutput(std::string(output));
					rules.push_back(r);
				}
			}
		}
		fclose(fp);
		delete[] buffer;

		for (int i = 0; i < iterations; ++i)
		{
			int newsize = size;
			if ((size % 2) == 0)
			{
				newsize = Expand(2, 3);
			}
			else
			{
				assert((size % 3) == 0);
				newsize = Expand(3, 4);
			}
			size = newsize;
		}
		int result = 0;
		for (int y = 0; y < size; ++y)
		{
			for (int x = 0; x < size; ++x)
			{
				if (map[y][x] == '#')
				{
					++result;
				}
				else
				{
					assert(map[y][x] == '.');
				}
			}
		}
		printf("Part %d: %d\n", part, result);
	}
	return 0;
}
