// day21.cpp : Advent of Code 2016 https://adventofcode.com/2016
// Solution by trave.uk 27/11/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

#define QUIET
void printf_quiet(const char *fmt, ...)
{
}

#ifdef QUIET
#define print printf_quiet
#else
#define print printf
#endif

void Swap(std::string& password, int x, int y)
{
	// swap position X with position Y
	char ch = password[x];
	password[x] = password[y];
	password[y] = ch;
}

void RotateRight(std::string& password, int x)
{
	// rotate right X steps
	x %= password.length();
	std::string newPassword = password.substr(password.length() - x) + password.substr(0, password.length() - x);
	password = newPassword;
}

void RotateLeft(std::string& password, int x)
{
	// rotate left X steps
	x %= password.length();
	x = password.length() - x;
	RotateRight(password, x);
}

void Reverse(std::string& password, int x, int y)
{
	assert(x < y);
	// reverse positions X through Y
	while (x < y)
	{
		Swap(password, x, y);
		++x;
		--y;
	}
}

void Move(std::string& password, int x, int y)
{
	// move position X to position Y
	std::string newPassword;
	if (x < y)
	{
		newPassword = password.substr(0, x) + password.substr(x + 1, y - x) + password.substr(x, 1) + password.substr(y+1);
	}
	else if (x > y)
	{
		newPassword = password.substr(0, y) + password.substr(x, 1) + password.substr(y, x - y) + password.substr(x+1);
	}
	assert(password.length() == newPassword.length());
	password = newPassword;
}

enum COMMAND
{
	SWAP_POSITION,
	SWAP_LETTER,
	ROTATE_BASED,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	REVERSE,
	MOVE
};

struct Command
{
	COMMAND c;
	int x;
	int y;

	void run(std::string& password)
	{
		switch (c)
		{
		case SWAP_POSITION:
			print("%s swap positions %d, %d: ", password.c_str(), x, y);
			Swap(password, x, y);
			print("%s\n", password.c_str());
			break;
		case SWAP_LETTER:
		{
			print("%s swap letters %c, %c: ", password.c_str(), x, y);
			int xx = password.find(char(x));
			int yy = password.find(char(y));
			Swap(password, xx, yy);
			print("%s\n", password.c_str());
			break;
		}
		case ROTATE_BASED:
		{
			print("%s rotate based on %c: ", password.c_str(), x);
			// the whole string should be rotated to the right based on the index of letter X (counting from 0) as determined before this instruction does any rotations. 
			// Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index, plus one additional time if the index was at least 4.
			int xx = password.find(x);
			RotateRight(password, xx + ((xx >= 4) ? 2 : 1));
			print("%s\n", password.c_str());
			break;
		}
		case ROTATE_LEFT:
			print("%s rotate left %d: ", password.c_str(), x);
			RotateLeft(password, x);
			print("%s\n", password.c_str());
			break;
		case ROTATE_RIGHT:
			print("%s rotate right %d: ", password.c_str(), x);
			RotateRight(password, x);
			print("%s\n", password.c_str());
			break;
		case REVERSE:
			print("%s reverse %d, %d: ", password.c_str(), x, y);
			Reverse(password, x, y);
			print("%s\n", password.c_str());
			break;
		case MOVE:
			print("%s move %d, %d: ", password.c_str(), x, y);
			Move(password, x, y);
			print("%s\n", password.c_str());
			break;
		}
	}

	void reverse(std::string& password)
	{
		switch (c)
		{
		case SWAP_POSITION:
			print("%s invert swap positions %d, %d: ", password.c_str(), x, y);
			Swap(password, x, y);
			print("%s\n", password.c_str());
			break;
		case SWAP_LETTER:
		{
			print("%s invert swap letters %c, %c: ", password.c_str(), x, y);
			int xx = password.find(char(x));
			int yy = password.find(char(y));
			Swap(password, xx, yy);
			print("%s\n", password.c_str());
			break;
		}
		case ROTATE_BASED:
		{
			print("%s invert rotate based on %c: ", password.c_str(), x);
			// Reverse this:
			// the whole string should be rotated to the right based on the index of letter X (counting from 0) as determined before this instruction does any rotations. 
			// Once the index is determined, rotate the string to the right one time, plus a number of times equal to that index, plus one additional time if the index was at least 4.
			// int xx = password.find(x);
			// RotateRight(password, xx + ((xx >= 4) ? 2 : 1));

			// So:
			// Rotate the whole string to the left by an amount.
			// It could be any amount (1 to length+1), so try each one and see whether the given letter made it work when rotated by that amount.
			// If >1 possibility is found, then I'll have to deal with a search space...
			std::vector<int> maybes;
			for (int i = 1; i <= password.length() + 1; ++i)
			{
				std::string testPassword = password;
				RotateLeft(testPassword, i);
				int xx = testPassword.find(x);
				int rotation = xx + ((xx >= 4) ? 2 : 1);
				if (i == rotation)
				{
					maybes.push_back(i);
				}
			}
			assert(maybes.size() == 1);
			RotateLeft(password, maybes[0]);
			print("%s\n", password.c_str());
			break;
		}
		case ROTATE_LEFT:
			print("%s invert rotate left %d: ", password.c_str(), x);
			RotateRight(password, x);
			print("%s\n", password.c_str());
			break;
		case ROTATE_RIGHT:
			print("%s invert rotate right %d: ", password.c_str(), x);
			RotateLeft(password, x);
			print("%s\n", password.c_str());
			break;
		case REVERSE:
			print("%s invert reverse %d, %d: ", password.c_str(), x, y);
			Reverse(password, x, y);
			print("%s\n", password.c_str());
			break;
		case MOVE:
			print("%s invert move %d, %d: ", password.c_str(), x, y);
			Move(password, y, x);
			print("%s\n", password.c_str());
			break;
		}
	}
};

void Process(const char* filename, std::string& password, bool bReverse = false)
{
	std::vector<Command> commands;
	int result = 0;
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
				Command command;
				char* tok = strtok(thisLine, " ");
				if (strcmp(tok, "swap") == 0)
				{
					tok = strtok(nullptr, " ");
					if (strcmp(tok, "position") == 0)
					{
						// swap position X with position Y
						command.c = SWAP_POSITION;
						tok = strtok(nullptr, " ");
						command.x = atoi(tok);
						strtok(nullptr, " "); // "with"
						strtok(nullptr, " "); // "position"
						tok = strtok(nullptr, " ");
						command.y = atoi(tok);
					}
					else if (strcmp(tok, "letter") == 0)
					{
						// swap letter X with letter Y
						command.c = SWAP_LETTER;
						tok = strtok(nullptr, " ");
						command.x = *tok;
						strtok(nullptr, " "); // "with"
						strtok(nullptr, " "); // "letter"
						tok = strtok(nullptr, " ");
						command.y = *tok;
					}
					else
					{
						assert(false);
					}
				}
				else if (strcmp(tok, "rotate") == 0)
				{
					char* tok = strtok(nullptr, " ");
					if (strcmp(tok, "based") == 0)
					{
						// rotate based on position of letter X
						command.c = ROTATE_BASED;
						strtok(nullptr, " "); // "on"
						strtok(nullptr, " "); // "position"
						strtok(nullptr, " "); // "of"
						strtok(nullptr, " "); // "letter"
						tok = strtok(nullptr, " ");
						command.x = *tok;
					}
					else if (strcmp(tok, "left") == 0)
					{
						// rotate left X steps
						command.c = ROTATE_LEFT;
						tok = strtok(nullptr, " ");
						command.x = atoi(tok);
					}
					else if (strcmp(tok, "right") == 0)
					{
						// rotate right X steps
						command.c = ROTATE_RIGHT;
						tok = strtok(nullptr, " ");
						command.x = atoi(tok);
					}
					else
					{
						assert(false);
					}
				}
				else if (strcmp(tok, "reverse") == 0)
				{
					// reverse positions X through Y
					command.c = REVERSE;
					strtok(nullptr, " "); // "positions"
					tok = strtok(nullptr, " ");
					command.x = atoi(tok);
					strtok(nullptr, " "); // "through"
					tok = strtok(nullptr, " ");
					command.y = atoi(tok);
				}
				else if (strcmp(tok, "move") == 0)
				{
					// move position X to position Y
					command.c = MOVE;
					strtok(nullptr, " "); // "position"
					tok = strtok(nullptr, " ");
					command.x = atoi(tok);
					strtok(nullptr, " "); // "to"
					strtok(nullptr, " "); // "position"
					tok = strtok(nullptr, " ");
					command.y = atoi(tok);
				}
				else
				{
					assert(false);
				}
				commands.push_back(command);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	if (bReverse)
	{
		for (auto it = commands.rbegin(); it != commands.rend(); ++it)
		{
			Command c = *it;
			c.reverse(password);
		}
	}
	else
	{
		for (Command c : commands)
		{
			c.run(password);
		}
	}
}

int main()
{
	std::string password = "abcde", start = password;
	Process("example.txt", password);
	printf("Example Part 1: %s -> %s\n", start.c_str(), password.c_str());

	password = "abcdefgh", start = password;
	Process("input.txt", password);
	printf("My Part 1: %s -> %s\n", start.c_str(), password.c_str());

	start = password;
	Process("input.txt", password, true);
	printf("My Part 1 undone: %s -> %s\n", start.c_str(), password.c_str());

	password = "fbgdceah", start = password;
	Process("input.txt", password, true);
	printf("My Part 2: %s -> %s\n", start.c_str(), password.c_str());

	start = password;
	Process("input.txt", password);
	printf("My Part 2 undone: %s -> %s\n", start.c_str(), password.c_str());

	return 0;
}
