// day12.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 12/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct Node
{
	std::string id;
	std::vector<std::string> links;
	bool largeCavern() const { return isupper(id[0]); }
};

std::map<std::string, Node> nodes;

void Process(const char* filename)
{
	nodes.clear();
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
				// e.g. "fs-end"
				char node1[10], node2[10];
				int matches = sscanf(thisLine, "%[a-zA-Z]-%[a-zA-Z]", node1, node2);
				assert(matches == 2);
				Node& nodeA = nodes[node1];
				Node& nodeB = nodes[node2];
				nodeA.id = node1;
				nodeB.id = node2;
				nodeA.links.push_back(node2);
				nodeB.links.push_back(node1);
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	for (int part = 1; part <= 2; ++part)
	{
		int64 score = 0;

		std::queue<std::vector<std::string>> openSet;
		std::vector<std::string> start;
		start.push_back("start");
		openSet.push(start);
		while (!openSet.empty())
		{
			std::vector<std::string> path = openSet.front();
			openSet.pop();
			std::string nodeName = path.back();
			if (nodeName.compare("end") == 0)
			{
				++score;
//				for (int i = 0; i < n.size(); ++i)
//				{
//					printf("%s%c", n[i].c_str(), (i == n.size() - 1) ? '\n' : ',');
//				}
				continue;
			}
			const Node& node = nodes[nodeName];
			for (std::string neighbourName : node.links)
			{
				const Node& neighbour = nodes[neighbourName];
				std::vector<std::string> o = path;
				if (!neighbour.largeCavern())
				{
					if (neighbourName.compare("start") == 0)
						continue;
					if (std::find(path.begin(), path.end(), neighbourName) != path.end())
					{
						if (part == 1)
						{
							continue;
						}
						else
						{
							// In part 2, only one small cavern can be visited twice per path
							// Check for duplicate marker
							if (std::find(path.begin(), path.end(), "*") != path.end())
							{
								continue;
							}
							// add duplicate marker, and continue
							o.push_back("*");
						}
					}
				}
				o.push_back(neighbourName);
				openSet.push(o);
			}
		}

		int result = 0;
		printf("%s: Part %d: %lld\n", filename, part, score);
	}
}

int main()
{
	Process("example1.txt");
	Process("example2.txt");
	Process("example3.txt");
	Process("input.txt");

	return 0;
}
