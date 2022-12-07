// day07.cpp : Advent of Code 2022 https://adventofcode.com/2022
// Solution by trave.uk 07/12/2022 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct FileObject
{
	std::string name;
	enum { FILE, DIR } type;
	int64 fileSize = 0;
	FileObject* parent;
	std::vector<FileObject> dirContents;

	FileObject(int64 size, std::string n, FileObject* p = nullptr)
	{
		type = FILE;
		fileSize = size;
		name = n;
		parent = p;
	}
	FileObject(std::string n, FileObject* p = nullptr)
	{
		type = DIR;
		dirContents.clear();
		name = n;
		parent = p;
	}
	int64 size()
	{
		switch (type)
		{
		case FILE:
			return fileSize;
		case DIR:
		{
			int64 dirSize = 0;
			for (FileObject& f : dirContents)
			{
				dirSize += f.size();
			}
			return dirSize;
		}
		}
		assert(false);
		return -1;
	}
};


void Process(const char* filename, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	FileObject root("/");
	int64 part1 = 0;
	int64 part2 = 0;
	char* buffer = new char[65536];
	FileObject* cd = &root;
	FILE *fp = fopen(filename, "rt");
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				switch (*thisLine)
				{
				case '$':
				{
					char command[80];
					char relpath[80];
					int m = sscanf(thisLine, "$ %s %s", command, relpath);
					assert(m >= 1);
					std::string cmd(command);
					std::string rpath(relpath);
					if (cmd == "cd")
					{
						assert(m == 2);
						if (rpath == "/")
							cd = &root;
						else if (rpath == "..")
						{
							assert(cd->parent);
							cd = cd->parent;
						}
						else
						{
							bool found = false;
							for (FileObject& f : cd->dirContents)
							{
								if (f.name == rpath)
								{
									assert(f.type == FileObject::DIR);
									cd = &f;
									found = true;
									break;
								}
							}
							assert(found);
						}
					}
					else
					{
						// Ignore ls commands
						assert(cmd == "ls");
					}
					break;
				}
				case 'd':
				{
					char dirname[80];
					int m = sscanf(thisLine, "dir %s", dirname);
					assert(m == 1);
					FileObject d(dirname, cd);
					cd->dirContents.push_back(d);
					break;
				}
				default:
				{
					assert(isdigit(*thisLine));
					int64 fileSize;
					char filename[80];
					int m = sscanf(thisLine, "%lld %s", &fileSize, filename);
					assert(m == 2);
					FileObject f(fileSize, filename, cd);
					cd->dirContents.push_back(f);
					break;
				}
				}
			}
		}
	}
	fclose(fp);
	delete[] buffer;

	std::stack<FileObject*> search;
	search.push(&root);

	int64 requiredSize = root.size() - 40000000;
	part2 = INT64_MAX;

	while (!search.empty())
	{
		FileObject* d = search.top();
		search.pop();
		assert(d->type == FileObject::DIR);
		if (d->size() <= 100000)
		{
			part1 += d->size();
		}
		if (d->size() >= requiredSize && d->size() < part2)
		{
			part2 = d->size();
		}

		for (FileObject& fo : d->dirContents)
		{
			if (fo.type == FileObject::DIR)
			{
				search.push(&fo);
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
	Process("example.txt", 95437, 24933642);
	Process("input.txt");

	return 0;
}
