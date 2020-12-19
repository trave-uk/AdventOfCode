// day19.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

struct Node
{
	using ResultSet = std::set<std::string>;
	using Pattern = std::vector<int>;

	Node() = default;
	Node(const char* def, int part)
	{
		if (part == 2)
		{
			int i = atoi(def);
			if (i == 8)
			{
				def = "8: 42 | 42 8";
			}
			else if (i == 11)
			{
				def = "11: 42 31 | 42 11 31";
			}
		}

		if (strcspn(def, "\"") != strlen(def))
		{
			int count = sscanf(def, "%d: \"%c\"", &index, &value);
			assert(count == 2);
		}
		else if (strcspn(def, "|") != strlen(def))
		{
			char leftString[80];
			char rightString[80];
			int count = sscanf(def, "%d: %[0-9 ] | %[0-9 ]", &index, leftString, rightString);
			assert(count == 3);
			Pattern patt1;
			parseInts(patt1, leftString);
			patterns.push_back(patt1);
			Pattern patt2;
			parseInts(patt2, rightString);
			patterns.push_back(patt2);
		}
		else
		{
			char leftString[80];
			int count = sscanf(def, "%d: %[0-9 ]", &index, leftString);
			assert(count == 2);
			Pattern patt;
			parseInts(patt, leftString);
			patterns.push_back(patt);
		}
		map[index] = *this;
	}

	static Node& Root() { return map[0]; }

	void parseInts(Pattern& output, char* input)
	{
		output.clear();
		char* tok = strtok(input, " ");
		while (tok)
		{
			output.push_back(atoi(tok));
			tok = strtok(nullptr, " ");
		}
	}

	// return true if whole string matches exactly
	bool matchTop(std::string test)
	{
		ResultSet remain = match(test);
		for (std::string s : remain)
		{
			if (s.empty())
			{
				return true;
			}
		}
		return false;
	}

	static void combineSets(ResultSet& result, const ResultSet& extra)
	{
		ResultSet prev = result;
		result.clear();
		std::merge(prev.begin(), prev.end(), extra.begin(), extra.end(), std::inserter(result, result.begin()));
	}

	// return a set of strings to continue matching
	ResultSet match(std::string test) const
	{
		ResultSet result;
		if (value)
		{
			if (test[0] == value)
			{
				result.insert(test.substr(1));
			}
		}
		for (Pattern pattern : patterns)
		{
			ResultSet remain = matchPattern(test, pattern);
			combineSets(result, remain);
		}
		return result;
	}

	// match against a pattern of other nodes
	static std::set<std::string> matchPattern(std::string test, Pattern pattern)
	{
		ResultSet nodeResult;
		nodeResult.insert(test);
		for (int nodeId : pattern)
		{
			const Node& node = map[nodeId];
			ResultSet newResult;
			for (std::string test2 : nodeResult)
			{
				ResultSet thisResult = node.match(test2);
				combineSets(newResult, thisResult);
			}
			nodeResult = newResult;
		}
		return nodeResult;
	}

	int index = -1;
	char value = 0;
	std::vector<Pattern> patterns;

	static void reset()
	{
		map.clear();
	}

	static std::map<int, Node> map;
};

std::map<int, Node> Node::map;

int main()
{
	for (int part = 1; part <= 2; ++part)
	{
		Node::reset();
		int result = 0;
		char* buffer = new char[65536];
		//FILE *fp = fopen( "example.txt", "rt" );
		//FILE *fp = fopen("example2.txt", "rt");
		FILE *fp = fopen( "input.txt", "rt" );
		while (!feof(fp))
		{
			char* thisLine = fgets(buffer, 65536, fp);
			if (thisLine)
			{
				thisLine[strcspn(thisLine, "\n\r")] = '\0';
				if (isdigit(*thisLine))
				{
					Node temp(thisLine, part);
				}
				else if (isalpha(*thisLine))
				{
					if (Node::Root().matchTop(thisLine))
					{
						//printf("%s\n", thisLine);
						++result;
					}
				}
				else if (*thisLine != '#')
				{
					assert(!*thisLine);
				}
			}
		}
		fclose(fp);
		delete[] buffer;

		printf("Part %d: %d\n", part, result);
	}
	return 0;
}
