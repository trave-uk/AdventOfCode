// day23.cpp : Advent of Code 2024 https://adventofcode.com/2024
// Solution by trave.uk 23/12/2024 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Computer
{
	Computer(std::string name = "") : name(name) {}
	std::string name;
	std::set<std::string> connected;
};

using Computers = std::map<std::string, Computer>;
using ComputerGroup = std::set<std::string>;
using ComputerGroups = std::set<ComputerGroup>;

int64 CalculatePart1(const Computers& computers)
{
	ComputerGroups groups;
	for (auto& it : computers)
	{
		const Computer& computer = it.second;
		for (const std::string& connection : computer.connected)
		{
			const Computer& other = computers.at(connection);
			for (const std::string& connection2 : computer.connected)
			{
				if (connection2 != connection && other.connected.count(connection2))
				{
					ComputerGroup group;
					group.insert(computer.name);
					group.insert(connection);
					group.insert(connection2);
					groups.insert(group);
				}
			}
		}
	}
	int64 count = 0;
	for (ComputerGroup group : groups)
	{
		for (std::string computerName : group)
		{
			if (computerName[0] == 't')
			{
				++count;
				break;
			}
		}
	}
	return count;
}

std::string CalculatePart2(const Computers& computers)
{
	int64 largestSize = 0;
	ComputerGroup largestGroup;
	for (auto& it : computers)
	{
		const std::string& name1 = it.first;
		const Computer& pc1 = it.second;
		for (const std::string& name2 : pc1.connected)
		{
			const Computer& pc2 = computers.at(name2);
			if (name2 != name1)
			{
				ComputerGroup group;
				group.insert(name1);
				group.insert(name2);
				for (const std::string& name3 : pc2.connected)
				{
					if (name3 != name2)
					{
						const Computer& pc3 = computers.at(name3);
						bool inGroup = true;
						for (std::string name : group)
						{
							if (!pc3.connected.count(name))
							{
								inGroup = false;
							}
						}
						if (inGroup)
							group.insert(name3);
					}
				}
				if (group.size() > largestSize)
				{
					largestSize = group.size();
					largestGroup = group;
				}
			}
		}
	}
	std::string password;
	for (std::string name : largestGroup)
	{
		if (password.length() != 0)
		{
			password.append(1, ',');
		}
		password.append(name);
	}
	return password;
}

void Process(const char* filename, int64 expectedPart1 = -1, std::string expectedPart2 = "")
{
	Computers computers;

	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				std::string line(thisLine);
				// ab-cd
				std::string pc1 = strtok(thisLine, "-");
				std::string pc2 = strtok(nullptr, "-");
				if (!computers.count(pc1))
				{
					computers[pc1] = Computer(pc1);
				}
				computers[pc1].connected.insert(pc2);
				if (!computers.count(pc2))
				{
					computers[pc2] = Computer(pc2);
				}
				computers[pc2].connected.insert(pc1);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	int64 part1 = CalculatePart1(computers);
	std::string part2 = CalculatePart2(computers);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s: Part 1: %lld\n", filename, part1);

	assert(expectedPart2 == "" || expectedPart2 == part2);
	printf("%s: Part 2: %s\n", filename, part2.c_str());
}

int main()
{
	Process("example.txt", 7, "co,de,ka,ta");
	Process("input.txt");

	return 0;
}
