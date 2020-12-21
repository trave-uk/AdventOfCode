// day21.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Allergen
{
	Allergen() = default;
	std::set<std::string> ingredients;
};

struct Food
{
	std::set<std::string> ingredients;
	std::set<std::string> allergens;
};

std::vector<Food> foods;
std::map<std::string, Allergen> allergens;

int main()
{
	int result = 0;
	char* buffer = new char[65536];
	//FILE *fp = fopen( "example.txt", "rt" );
	FILE *fp = fopen( "input.txt", "rt" );
	while ( !feof( fp ) )
	{
		char* thisLine = fgets( buffer, 65536, fp );
		if ( thisLine )
		{
			Food food;
			thisLine[strcspn( thisLine, "\n\r" )] = '\0';
			char* ing = strtok(thisLine, "()");
			char* all = strtok(nullptr, "()");
			char* i = strtok(ing, " ");
			while (i)
			{
				food.ingredients.insert(i);
				i = strtok(nullptr, " ");
			}
			char *a = strtok(all, " ,");
			while (a)
			{
				if (strcmp(a, "contains") != 0)
				{
					food.allergens.insert(a);
				}
				a = strtok(nullptr, " ,");
			}
			foods.push_back(food);
		}
	}
	fclose( fp );
	delete[] buffer;

	for (Food food : foods)
	{
		for (std::string a : food.allergens)
		{
			Allergen& allergen = allergens[a];
			if (allergen.ingredients.size())
			{
				std::set<std::string> newList;
				for (std::string i1 : allergen.ingredients)
				{
					for (std::string i2 : food.ingredients)
					{
						if (i1 == i2)
						{
							newList.insert(i1);
						}
					}
				}
				allergen.ingredients = newList;
			}
			else
			{
				allergen.ingredients = food.ingredients;
			}
		}
	}

	bool finished = false;
	while (!finished)
	{
		finished = true;
		for (auto& it : allergens)
		{
			std::string name = it.first;
			const Allergen allergen = it.second;
			if (allergen.ingredients.size() == 1)
			{
				std::string ingredient = *allergen.ingredients.begin();
				for (auto& it2 : allergens)
				{
					std::string name2 = it2.first;
					Allergen& allergen2 = it2.second;
					if (name != name2 && allergen2.ingredients.count(ingredient) > 0)
					{
						allergen2.ingredients.erase(ingredient);
					}
				}
			}
			else
			{
				finished = false;
			}
		}
	}

	std::set<std::string> potentialAlergens;

	for (auto& it : allergens)
	{
		std::string name = it.first;
		const Allergen& allergen = it.second;
		for (std::string ingredient : allergen.ingredients)
		{
			potentialAlergens.insert(ingredient);
		}
		//assert(allergen.ingredients.size() == 1);
	}

	int part1 = 0;
	for (Food food : foods)
	{
		for (std::string ingredent : food.ingredients)
		{
			if (potentialAlergens.count(ingredent) == 0)
			{
				++part1;
			}
		}
	}

	printf( "Part 1: %d\n", part1 );

	std::string part2;
	for (auto it : allergens)
	{
		if ( part2.length() > 0)
			part2 += ",";
		part2 += *it.second.ingredients.begin();
	}

	printf("Part 2: %s\n", part2.data());
	return 0;
}
