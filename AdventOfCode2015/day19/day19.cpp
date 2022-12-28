// day19.cpp : Advent of Code 2015 https://adventofcode.com/2015
// Solution by trave.uk 28/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Molecule : public std::vector<std::string>
{
	Molecule() = default;

	Molecule(std::string input)
	{
		std::string element = "";
		for (char c : input)
		{
			if (isupper(c))
			{
				if (element.length() > 0)
				{
					push_back(element);
				}
				element.clear();
				element.append(1, c);
			}
			else
			{
				element.append(1, c);
			}
		}
		if (element.length() > 0)
		{
			push_back(element);
		}
	}

	bool operator<(const Molecule& rhs)
	{
		if (size() < rhs.size())
		{
			return true;
		}
		else if (size() == rhs.size())
		{
			const std::vector<std::string>* left = this;
			const std::vector<std::string>* right = &rhs;
			return (*left) < (*right);
		}
		else
		{
			return false;
		}
	}

	UINT32 Hash()
	{
		UINT32 h = 0;
		for (const auto& e : *this)
		{
			for (const char c : e)
			{
				h = updateCRC32(c, h);
			}
		}
		return h;
	}
};

struct Substitution
{
	Substitution() = default;
	Substitution(std::string source, std::string dest)
		: sourceElement(source)
		, destMolecule(dest)
	{}

	std::string sourceElement;
	Molecule destMolecule;
};

using SubstitutionDictionary = std::vector<Substitution>;

int64 Calibrate(Molecule initialMolecule, const SubstitutionDictionary& subs)
{
	std::set<Molecule> distinctMolecules;

	for (const Substitution& s : subs)
	{
		for (int i = 0; i < initialMolecule.size(); ++i)
		{
			if (initialMolecule[i] == s.sourceElement)
			{
				Molecule m = initialMolecule;
				m.erase(m.begin() + i);
				m.insert(m.begin() + i, s.destMolecule.begin(), s.destMolecule.end());
				distinctMolecules.insert(m);
			}
		}
	}
	return distinctMolecules.size();
}

int64 Generate(Molecule targetMolecule, const SubstitutionDictionary& substitutions)
{
	// Sort substitions so that the longest ones are at the start.
	SubstitutionDictionary subs = substitutions;
	std::sort(subs.begin(), subs.end(), [](const Substitution& a, const Substitution& b) { return a.destMolecule.size() > b.destMolecule.size(); });

	Molecule start("e");
	Molecule current(targetMolecule);
	std::vector<Molecule> stack;
	std::map<UINT32, int64> best;
	stack.push_back(current);
	best[current.Hash()] = 0;
	while (stack.size() > 0)
	{
		current = *stack.begin();
		stack.erase(stack.begin());
		int64 size = best[current.Hash()];
		if (current == start)
		{
			return size;
		}
		// Look for backward substitutions within current
		for (const Substitution& s : subs)
		{
			auto it = std::search(current.begin(), current.end(), s.destMolecule.begin(), s.destMolecule.end());
			while (it != current.end())
			{
				Molecule prev = current;
				int64 pos = it - current.begin();
				prev.erase(prev.begin() + pos, prev.begin() + pos + s.destMolecule.size());
				prev.insert(prev.begin() + pos, s.sourceElement);
				if (s.sourceElement != "e" || prev.size() == 1)
				{
					auto in = std::make_pair(prev, size + 1);
					UINT32 prevHash = prev.Hash();
					if (best.count(prevHash) == 0 || best[prevHash] > size + 1)
					{
						stack.push_back(prev);
						std::sort(stack.begin(), stack.end(), [](const Molecule& a, const Molecule& b) { return a.size() < b.size(); });
						best[prevHash] = size + 1;
					}
				}
				it = std::search(it + 1, current.end(), s.destMolecule.begin(), s.destMolecule.end());
			}
		}
	}
	return -1;
}

void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	double start = GetMilliseconds();
	char* buffer = new char[65536];
	FILE *fp = fopen(filename, "rt");
	bool finishedSubstitutions = false;

	SubstitutionDictionary subs;
	Molecule medicineMolecule;

	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				if (!finishedSubstitutions)
				{
					char source[80], dest[80];
					int m = sscanf(thisLine, "%s => %s", source, dest);
					assert(m == 2);
					Substitution s(source, dest);
					subs.push_back(s);
				}
				else
				{
					assert(medicineMolecule.empty());
					medicineMolecule = Molecule(thisLine);
				}
			}
			else
			{
				finishedSubstitutions = true;
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = Calibrate(medicineMolecule, subs);

	if (expectedPart1 != -1)
	{
		printf("[%.2f] %s: Part 1 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart1);
	}
	printf("[%.2f] %s: Part 1: %lld\n", GetMilliseconds() - start, filename, part1);
	assert(expectedPart1 == -1 || expectedPart1 == part1);

	int64 part2 = Generate(medicineMolecule, subs);

	if (expectedPart2 != -1)
	{
		printf("[%.2f] %s: Part 2 expected: %lld\n", GetMilliseconds() - start, filename, expectedPart2);
	}
	printf("[%.2f] %s: Part 2: %lld\n", GetMilliseconds() - start, filename, part2);
	assert(expectedPart2 == -1 || expectedPart2 == part2);
}

int main()
{
	Process("example.txt", 4, 3);
	Process("input.txt");

	return 0;
}
