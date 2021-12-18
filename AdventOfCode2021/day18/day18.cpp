// day18.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 18/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

enum class Output
{
	None,
	ReduceDetail,
	AddDetail,
};

struct SnailFishNumber
{
	bool highlight = false;
	virtual int64 magnitude() = 0;
	virtual void reduce(Output output = Output::None) {}
	virtual SnailFishNumber* copy() = 0;
	virtual bool isPair() { return false; }
	virtual bool isNumber() { return false; }
	virtual void print() = 0;
	void printString()
	{
		print();
		_putch('\n');
	}
};

struct Number : public SnailFishNumber
{
	Number(int _value = 0)
		: value(_value)
	{
	}
	int value;
	Number *leftNeighbour = nullptr;
	Number *rightNeighbour = nullptr;

	virtual int64 magnitude() override { return value; }
	virtual bool isNumber() override { return true; }
	virtual SnailFishNumber* copy() override { return new Number(value); }

	virtual void print() override
	{
		if (highlight)
			setcolour(TermColour::LightGrey);
		printf("%d", value);
		if (highlight)
			setcolour(TermColour::DarkGrey);
	}
};

struct Pair : public SnailFishNumber
{
	int depth = 0;
	SnailFishNumber* left = nullptr;
	SnailFishNumber* right = nullptr;

	virtual ~Pair()
	{
		delete(left);
		delete(right);
	}

	virtual int64 magnitude() override { return left->magnitude() * 3 + right->magnitude() * 2; }
	virtual bool isPair() override { return true; }

	virtual SnailFishNumber* copy() override
	{
		Pair* newPair = new Pair();
		newPair->left = left->copy();
		newPair->right = right->copy();
		return newPair;
	}

	virtual void print() override
	{
		if (highlight)
			setcolour(TermColour::LightGrey);
		_putch('[');
		left->print();
		_putch(',');
		right->print();
		_putch(']');
		if (highlight)
			setcolour(TermColour::DarkGrey);
	}

	virtual void reduce(Output output = Output::None) override
	{
		// This is called on the outer pair
		if (output == Output::ReduceDetail)
		{
			printf("after addition:\t");
		}
		bool bActed = false;
		do
		{
			bActed = false;
			// record depths of all pairs, and neighbours of all numbers
			std::stack<SnailFishNumber*> s;
			depth = 0;
			s.push(this);
			Number* prevNumber = nullptr;
			Number* firstNumber = nullptr;
			while (!s.empty())
			{
				SnailFishNumber* n = s.top();
				s.pop();
				if (n->isPair())
				{
					Pair* p = static_cast<Pair*>(n);

					if (p->right->isPair())
					{
						Pair *r = static_cast<Pair*>(p->right);
						r->depth = p->depth + 1;
					}
					s.push(p->right);
					if (p->left->isPair())
					{
						Pair *l = static_cast<Pair*>(p->left);
						l->depth = p->depth + 1;
					}
					s.push(p->left);
				}
				else
				{
					assert(n->isNumber());
					Number *l = static_cast<Number*>(n);
					if (!firstNumber)
					{
						firstNumber = l;
					}
					if (prevNumber)
					{
						prevNumber->rightNeighbour = l;
					}
					l->leftNeighbour = prevNumber;
					l->rightNeighbour = nullptr;
					prevNumber = l;
				}
			}

			// Now, explode the leftmost pair of depth 4.
			s.push(this);
			while (!s.empty())
			{
				SnailFishNumber* n = s.top();
				s.pop();
				assert(n->isPair());
				Pair* p = static_cast<Pair*>(n);
				if (p->right->isPair())
					s.push(p->right);
				if (p->left->isPair())
					s.push(p->left);
				if (p->left->isPair())
				{
					Pair *l = static_cast<Pair*>(p->left);
					if (l->depth == 4)
					{
						if (output == Output::ReduceDetail)
						{
							p->left->highlight = true;
							printString();
						}

						assert(l->left->isNumber() && l->right->isNumber());
						Number* ll = static_cast<Number*>(l->left);
						Number* lr = static_cast<Number*>(l->right);
						if (ll->leftNeighbour)
						{
							ll->leftNeighbour->value += ll->value;
						}
						if (lr->rightNeighbour)
						{
							lr->rightNeighbour->value += lr->value;
						}
						delete(l);
						p->left = new Number();
						bActed = true;
						break;
					}
				}
				if (p->right->isPair())
				{
					Pair *r = static_cast<Pair*>(p->right);
					if (r->depth == 4)
					{
						if (output == Output::ReduceDetail)
						{
							p->right->highlight = true;
							printString();
						}

						assert(r->left->isNumber() && r->right->isNumber());
						Number* rl = static_cast<Number*>(r->left);
						Number* rr = static_cast<Number*>(r->right);
						if (rl->leftNeighbour)
						{
							rl->leftNeighbour->value += rl->value;
						}
						if (rr->rightNeighbour)
						{
							rr->rightNeighbour->value += rr->value;
						}
						delete(r);
						p->right = new Number();
						bActed = true;
						break;
					}
				}
			}
			s = std::stack<SnailFishNumber*>();
			if (bActed)
			{
				if (output == Output::ReduceDetail)
				{
					printf("after explode:\t");
				}
				continue;
			}

			// No explosions, so find leftmost number >= 10 and split it
			// Rather than iterate down the tree immediately, use the neighbour info to find the leftmost (first) number >10 then find that SnailFishNumber pointer in the tree.
			Number* firstLargeNumber = firstNumber;
			while (firstLargeNumber && firstLargeNumber->value < 10)
				firstLargeNumber = firstLargeNumber->rightNeighbour;

			s.push(this);
			while (!s.empty())
			{
				SnailFishNumber* n = s.top();
				s.pop();
				if (n->isPair())
				{
					Pair* p = static_cast<Pair*>(n);
					if (p->right->isPair())
						s.push(p->right);
					if (p->left->isPair())
						s.push(p->left);
					if (p->left->isNumber())
					{
						Number *n = static_cast<Number*>(p->left);
						if (n == firstLargeNumber)
						{
							if (output == Output::ReduceDetail)
							{
								p->left->highlight = true;
								printString();
							}
							Pair *newp = new Pair();
							p->left = newp;
							newp->left = new Number(n->value / 2);
							newp->right = new Number(n->value / 2 + n->value % 2);
							delete(n);
							bActed = true;
							break;
						}
					}
					if (p->right->isNumber())
					{
						Number *n = static_cast<Number*>(p->right);
						if (n == firstLargeNumber)
						{
							if (output == Output::ReduceDetail)
							{
								p->right->highlight = true;
								printString();
							}
							Pair *newp = new Pair();
							p->right = newp;
							newp->left = new Number(n->value / 2);
							newp->right = new Number(n->value / 2 + n->value % 2);
							delete(n);
							bActed = true;
							break;
						}
					}
				}
			}
			s = std::stack<SnailFishNumber*>();
			if (bActed)
			{
				if (output == Output::ReduceDetail)
				{
					printf("after split:\t");
				}
			}
		} while (bActed);
		if (output == Output::ReduceDetail)
		{
			printString();
		}
	}
};

SnailFishNumber* add(SnailFishNumber* left, SnailFishNumber* right, Output output = Output::None)
{
	if (output == Output::AddDetail)
	{
		printf("  ");
		left->printString();
		printf("+ ");
		right->printString();
		printf("= ");
	}
	Pair *pair = new Pair();
	pair->left = left->copy();
	pair->right = right->copy();
	pair->reduce(output);
	if (output == Output::AddDetail)
	{
		pair->printString();
		_putch('\n');
	}
	return pair;
}

std::vector<SnailFishNumber*> numbers;

void Load(const char* filename)
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
				// e.g. [[[[1,2],[3,4]],[[5,6],[7,8]]],9]
				std::stack<SnailFishNumber*> s;
				std::string line(thisLine);
				SnailFishNumber* outer = nullptr;
				for (char c : line)
				{
					switch (c)
					{
					case '[':
					{
						// push a new pair onto the stack
						s.push(new Pair());
						break;
					}
					case ']':
					{
						// take the two entries at the top of the stack, and make them the left and right of the next value which should be an empty pair
						SnailFishNumber *right = s.top();
						s.pop();
						SnailFishNumber *left = s.top();
						s.pop();
						Pair *pair = static_cast<Pair*>(s.top());
						assert(pair->isPair() && !pair->left && !pair->right);
						pair->left = left;
						pair->right = right;
						break;
					}
					case ',':
					{
						// nothing to do on a comma, it just separates two entries.
						break;
					}
					default:
					{
						// anything else is a digit
						assert(isdigit(c));
						// push a number onto the stack
						s.push(new Number(c - '0'));
						break;
					}
					}
				}
				// having resolved all pairs, there should be one pair left.
				assert(s.size() == 1);
				numbers.push_back(s.top());
				s.pop();
			}
		}
	}
	fclose(fp);
	delete[] buffer;
}

void Process(const char* filename, Output output = Output::None)
{
	setcolour(TermColour::LightGrey);
	printf("%s:\n", filename);
	numbers.clear();
	Load(filename);

	// dark grey, so that light grey can be used to highlight
	setcolour(TermColour::DarkGrey);

	// For part 1, add all the numbers in the list together and give the magnitude of the sum
	SnailFishNumber* result = nullptr;
	for (SnailFishNumber* n : numbers)
	{
		if (!result)
			result = n;
		else
		{
			result = add(result, n, output);
		}
	}

	printf("Sum = ");
	result->printString();
	printf("Part 1: magnitude = %lld\n", result->magnitude());

	// for part 2, find the largest magnitude as the result of adding any two numbers
	int64 highest = 0;
	for (SnailFishNumber *n : numbers)
	{
		for (SnailFishNumber *m : numbers)
		{
			SnailFishNumber *nm = add(n, m);
			int64 magnitude = nm->magnitude();
			if (magnitude > highest)
				highest = magnitude;
		}
	}

	printf("Part 2: largest magnitude of adding two Snailfish numbers: %lld\n", highest);
}

int main()
{
	Process("example1.txt", Output::ReduceDetail);
	Process("example2.txt");
	Process("example3.txt");
	Process("example4.txt");
	Process("example5.txt", Output::AddDetail);
// Examples 5a and 5b were steps in example 5 which went wrong so I used the detailed output to debug the reduce code.
//	Process("example5a.txt", Output::ReduceDetail);
//	Process("example5b.txt", Output::ReduceDetail);
	Process("example6.txt");
	Process("input.txt");

	setcolour(TermColour::LightGrey);
	return 0;
}
