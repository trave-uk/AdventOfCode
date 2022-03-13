// day15.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 13/03/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Ingredient
{
	std::string name;
	int64 capacity = 0;
	int64 durability = 0;
	int64 flavor = 0;
	int64 texture = 0;
	int64 calories = 0;

	int count = 0;

	int64 Capacity() const { return count * capacity; }
	int64 Durability() const { return count * durability; }
	int64 Flavor() const { return count * flavor; }
	int64 Texture() const { return count * texture; }
	int64 Calories() const { return count * calories; }

	Ingredient& operator+=(const Ingredient& rhs)
	{
		capacity += rhs.Capacity();
		durability += rhs.Durability();
		flavor += rhs.Flavor();
		texture += rhs.Texture();
		calories += rhs.Calories();
		return *this;
	}
};

using Ingredients = std::vector<Ingredient>;

void Calculate(Ingredients& ingredients, int64& part1, int64& part2)
{
	Ingredient total;
	for (Ingredient i: ingredients)
	{
		total += i;
	}
	int64 result = 1;
	if (total.capacity <= 0 || total.durability <= 0 || total.flavor <= 0 || total.texture <= 0)
	{
		return;
	}
	int64 product = total.capacity * total.durability * total.flavor * total.texture;
	if (product > part1)
		part1 = product;
	if (total.calories == 500 && product > part2)
		part2 = product;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	Ingredients ingredients;
	int nIngredients = 0;
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
				// Sprinkles: capacity 2, durability 0, flavor -2, texture 0, calories 3
				Ingredient i;
				char name[80];
				int found = sscanf(thisLine, "%[^:]: capacity %lld, durability %lld, flavor %lld, texture %lld, calories %lld", name, &i.capacity, &i.durability, &i.flavor, &i.texture, &i.calories);
				assert(found == 6);
				i.name = name;
				ingredients.push_back(i);
				++nIngredients;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	// 100 total ingredients, so 0-100 of each one.
	for (ingredients[0].count = 0; ingredients[0].count <= 100; ++ingredients[0].count)
	{
		if (nIngredients == 2)
		{
			ingredients[1].count = 100 - ingredients[0].count;
			Calculate(ingredients, part1, part2);
		}
		else
		{
			assert(nIngredients == 4);
			for (ingredients[1].count = 0; ingredients[1].count <= 100 - ingredients[0].count; ++ingredients[1].count)
			{
				for (ingredients[2].count = 0; ingredients[2].count < 100 - ingredients[0].count - ingredients[1].count; ++ingredients[2].count)
				{
					ingredients[3].count = 100 - ingredients[0].count - ingredients[1].count - ingredients[2].count;
					Calculate(ingredients, part1, part2);
				}
			}
		}
	}
	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s: Part 2: %lld\n", filename, part2);
}

int main()
{
	Process("example.txt", 62842880, 57600000);
	Process("input.txt");

	return 0;
}
