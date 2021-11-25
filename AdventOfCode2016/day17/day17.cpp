// day17.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 20/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct State1
{
	State1(char _x, char _y, std::string _path)
		: path(_path)
		, x(_x)
		, y(_y)
	{}

	std::string path;
	char x;
	char y;
};

bool isOpen(char c)
{
	return c >= 'b' && c <= 'f';
}

const char *dir = "UDLR";

void Part1(std::string title, std::string passcode)
{
	MD5 md5;

	md5.Init();
	md5.Update((const unsigned char*)passcode.c_str(), passcode.length());
	std::queue<State1> q;
	q.push(State1(0, 0, ""));

	while (!q.empty())
	{
		State1 s = q.front();
		q.pop();

		char h[5];
		MD5 md5;
		md5.Update((const unsigned char*)passcode.c_str(), passcode.length());
		md5.Update((const unsigned char*)s.path.c_str(), s.path.length());
		md5.Final(h, 4);

		for (int i = 0; i < 4; ++i)
		{
			State1 t = s;
			t.x += (i == 3) - (i == 2);
			t.y += (i == 1) - (i == 0);
			t.path.append(1, dir[i]);

			if (t.x < 0 || t.x > 3 || t.y < 0 || t.y > 3 || !isOpen(h[i]))
				continue;

			if (t.x == 3 && t.y == 3)
			{
				printf("%s Part 1: %s -> %s\n", title.c_str(), passcode.c_str(), t.path.c_str());
				return;
			}

			q.push(t);
		}
	}
	printf("%s Part 1: no solution\n", title.c_str());
}

struct State2
{
	State2(char _x, char _y, const MD5& _md5)
		: md5(_md5)
		, l(0)
		, x(_x)
		, y(_y)
	{}

	MD5 md5;
	int l;
	char x;
	char y;
};


void Part2(std::string title, std::string passcode)
{
	MD5 md5;

	md5.Init();
	md5.Update((const unsigned char*)passcode.c_str(), passcode.length());
	std::stack<State2> q;
	q.push(State2(0, 0, md5));

	int longest = 0;

	while (!q.empty())
	{
		State2 s = q.top();
		q.pop();

		char h[5];
		md5 = s.md5;
		md5.Final(h, 4);

		for (int i = 0; i < 4; ++i)
		{
			if (!isOpen(h[i]))
				continue;
			State2 t = s;
			t.x += (i == 3) - (i == 2);
			t.y += (i == 1) - (i == 0);
			if (t.x < 0 || t.x > 3 || t.y < 0 || t.y > 3)
				continue;

			t.l++;
			if (t.x == 3 && t.y == 3)
			{
				if (t.l > longest)
					longest = t.l;
				continue;
			}

			t.md5.Update((const unsigned char*)(dir + i), 1);
			q.push(t);
		}
	}
	printf("%s Part 2: %s -> %d\n", title.c_str(), passcode.c_str(), longest);
}

int main()
{
	Part1("Example 1", "ihgpwlah");
	Part1("Example 2", "kglvqrro");
	Part1("Example 3", "ulqzkmiv");
	Part1("My", "edjrjqaa");

	Part2("Example 1", "ihgpwlah");
	Part2("Example 2", "kglvqrro");
	Part2("Example 3", "ulqzkmiv");
	Part2("My", "edjrjqaa");

	return 0;
}
