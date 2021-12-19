// day19.cpp : Advent of Code 2021 https://adventofcode.com/2021
// Solution by trave.uk 19/12/2021 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

struct coord3
{
	coord3(int64 _x = 0, int64 _y = 0, int64 _z = 0)
		: x(_x)
		, y(_y)
		, z(_z)
	{}
	int64 x;
	int64 y;
	int64 z;

	void operator=(const coord3& rhs)
	{
		x = rhs.x; y = rhs.y; z = rhs.z;
	}
	bool operator==(const coord3& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	bool operator!=(const coord3& rhs) const
	{
		return x != rhs.x || y != rhs.y || z != rhs.z;
	}
	bool operator<(const coord3& rhs) const
	{
		return (x < rhs.x || (x == rhs.x && y < rhs.y) || (x == rhs.x && y == rhs.y && z < rhs.z));
	}
	void operator-=(const coord3& rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z;
	}
	void operator+=(const coord3& rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z;
	}
	int64 length()
	{
		return abs(x) + abs(y) + abs(z);
	}
	void rotate(const coord3& rot);
	bool match(const coord3& other, coord3& rotation) const;
};

// rotating around an axis, the other two dimensions are swapped, inverting one of them
void rotateAxis(int64 rot, int64& i, int64& j)
{
	for (; rot > 0; --rot)
	{
		int64 t = i;
		i = j;
		j = -t;
	}
}

// Rotate by Euler angles rot, relative to origin.
// warning: some sets of Euler angles are equivalent. There are 4^3=64 possible Euler angles but only 24 possible orientations
void coord3::rotate(const coord3& rot)
{
	rotateAxis(rot.x, y, z);
	rotateAxis(rot.y, z, x);
	rotateAxis(rot.z, x, y);
}

// See whether two coordinates match, returning the rotation between them if they do
bool coord3::match(const coord3& other, coord3& rot) const
{
	for (rot.x = 0; rot.x < 4; ++rot.x)
	{
		for (rot.y = 0; rot.y < 4; ++rot.y)
		{
			for (rot.z = 0; rot.z < 4; ++rot.z)
			{
				coord3 test = other;
				test.rotate(rot);
				if (test == *this)
				{
					return true;
				}
			}
		}
	}
	return false;
}

struct Scanner : public std::set<coord3>
{
	int id;
	// For each scanner, store the delta between all of its visible beacons.
	// These can then be compared between scanners to find equivalent deltas
	std::vector<std::vector<coord3>> deltas;

	void calculateDeltas();

	struct Parent
	{
		coord3 pos; // Position of this scanner relative to that scanner
		coord3 rot;	// Rotation (0-3) about each axis relative to scanner 0, applied rot.x,rot.y,rot.z
	};
	std::map<int, Parent> parents;

	coord3 pos; // The position of this scanner, relative to scanner 0
};

// Calculate the deltas between all of the beacons this scanner can see
// These are used to match scanners against each other, by rotating the deltas.
void Scanner::calculateDeltas()
{
	deltas.clear();
	std::vector<coord3> empty;
	empty.assign(size(), coord3());
	deltas.assign(size(), empty);

	for (int i = 0; i < size(); ++i)
	{
		const coord3& x = *std::next(begin(), i);
		for (int j = 0; j < size(); ++j)
		{
			const coord3& y = *std::next(begin(), j);
			coord3& d = deltas[j][i];
			d = x;
			d -= y;
		}
	}
}

using Scanners = std::vector<Scanner>;

Scanners scanners;

void Load(const char* filename)
{
	scanners.clear();
	Scanner s;
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
				// Checking the second character as '-' is a valid character for the first character of a number
				if (thisLine[1] == '-')
				{
					int id;
					int matches = sscanf(thisLine, "--- scanner %d ---", &id);
					assert(matches == 1);
					if (!s.empty())
					{
						s.calculateDeltas();
						scanners.push_back(s);
						s = Scanner();
					}
					assert(id == scanners.size());
					s.id = id;
				}
				else
				{
					// If negative, the first digit will be the second character
					assert(isdigit(*thisLine) || isdigit(thisLine[1]));
					coord3 pos;
					int matches = sscanf(thisLine, "%lld,%lld,%lld", &pos.x, &pos.y, &pos.z);
					assert(matches == 3);
					s.insert(pos);
				}
			}
		}
	}
	assert(!s.empty());
	s.calculateDeltas();
	scanners.push_back(s);

	fclose(fp);
	delete[] buffer;
}

// This will contain all of the beacon positions, to calculate the answer to part 1
Scanner master;

// Check whether scanners with indices si (s) and ti (t) have enough matching beacons, by rotating each delta in t in all permutations and comparing each against each delta in s.
bool Compare(int si, int ti)
{
	const Scanner& s = scanners[si];
	Scanner& t = scanners[ti];

	// These two scanners *may* have 12 beacons in common.
	// To check whether they do, for each pair of beacons in s, see whether there is a pair of beacons in t with matching delta

	struct Candidate
	{
		int s1;
		int s2;
		int t1;
		int t2;
	};
	std::map<coord3, std::vector<Candidate>> candidates;
	Candidate c;
	for (c.s1 = 0; c.s1 < s.size(); ++c.s1)
	{
		for (c.s2 = 0; c.s2 < s.size(); ++c.s2)
		{
			if (c.s1 == c.s2)
				continue;
			const coord3& ds = s.deltas[c.s1][c.s2];
			for (c.t1 = 0; c.t1 < t.size(); ++c.t1)
			{
				for (c.t2 = 0; c.t2 < t.size(); ++c.t2)
				{
					if (c.t1 == c.t2)
						continue;
					const coord3& dt = t.deltas[c.t1][c.t2];
					coord3 rot;
					if (ds.match(dt, rot))
					{
						if (!candidates.count(rot))
						{
							candidates.insert(std::make_pair(rot, std::vector<Candidate>()));
						}
						candidates[rot].push_back(c);
					}
				}
			}
		}
	}

	// For 12 beacons in common, there will be at minimum 132 (12*11) pairs:
	int max = 0;
	std::vector<Candidate>* pCan = nullptr;
	coord3 rot;
	for (auto& x : candidates)
	{
		if (x.second.size() > max)
		{
			max = x.second.size();
			pCan = &x.second;
			rot = x.first;
		}
	}
	if (max >= 132)
	{
		// Find the relative distance from s to t, given the rotation: one pairs should be all that is required
		// For each candidate, rotate t1 and t2 by rot and calculate the distance from each to s1 and s2.
		// Two of these distances should match: that is the post-rotation offset to apply. All candidates should agree
		coord3 distance;
		for (Candidate c : *pCan)
		{
			const coord3& s1 = *std::next(s.begin(), c.s1);
			const coord3& s2 = *std::next(s.begin(), c.s2);

			coord3 t1 = *std::next(t.begin(), c.t1);
			coord3 t2 = *std::next(t.begin(), c.t2);
			t1.rotate(rot);
			t2.rotate(rot);

			std::multiset<coord3> theseDistances;
			std::array<coord3, 4> d;
			d[0] = t1;
			d[0] -= s1;
			d[1] = t1;
			d[1] -= s2;
			d[2] = t2;
			d[2] -= s1;
			d[3] = t2;
			d[3] -= s2;
			for (coord3 dd : d)
			{
				theseDistances.insert(dd);
			}

			coord3 thisDistance;
			for (coord3 dd : theseDistances)
			{
				if (theseDistances.count(dd) == 2)
				{
					thisDistance = dd;
				}
			}
			assert(thisDistance != coord3());
			if (distance == coord3())
			{
				distance = thisDistance;
			}
			else
			{
				assert(distance == thisDistance);
			}
		}
		assert(distance != coord3());

		// Record the translation and parent
		Scanner::Parent p;
		p.pos = distance;
		p.rot = rot;
		t.parents.insert_or_assign(si, p);
		return true;
	}
	return false;
}

void Process(const char* filename)
{
	Load(filename);
	// For each pair of scanners, try to find 12 common beacons by rotating all points of one of them in all possible rotations
	for (int i=0; i<scanners.size(); ++i)
	{
		for (int j = 0; j < scanners.size(); ++j)
		{
			if (i == j)
				continue;
			Compare(i, j);
			_putch('.');
		}
		_putch('\n');
	}

	// Having compared all scanners, find a route from each to 0
	master.clear();
	for (auto& s : scanners)
	{
		std::vector<int> path;
		path.push_back(s.id);
		std::set<std::vector<int>> open;
		open.insert(path);
		while (!open.empty())
		{
			path = *open.begin();
			open.erase(path);
			int next = path.back();
			if (next == 0)
			{
				break;
			}
			const Scanner& t = scanners[next];
			for (auto p : t.parents)
			{
				if (std::find(path.begin(), path.end(), p.first) != path.end())
					continue;
				std::vector<int> newPath = path;
				newPath.push_back(p.first);
				open.insert(newPath);
			}
			path.clear();
		}
		open.clear();
		assert(!path.empty());

		const Scanner* t = nullptr;
		for (int ti : path)
		{
			if (ti == s.id)
			{
				t = &(scanners[ti]);
				continue;
			}

			const Scanner::Parent& p = t->parents.find(ti)->second;
			coord3 offset = p.pos;
			coord3 rot = p.rot;
			t = &(scanners[ti]);

			// rotate, translate, replace
			std::vector<coord3> translated;
			for (coord3 pos : s)
			{
				pos.rotate(rot);
				pos -= offset;
				translated.push_back(pos);
			}
			s.pos.rotate(rot);
			s.pos -= offset;
			s.clear();
			for (coord3 pos : translated)
			{
				s.insert(pos);
			}
		}
		// Copy translated points to the master set
		for (coord3 pos : s)
		{
			master.insert(pos);
		}
	}
	printf("%s: Part 1: %lld\n", filename, master.size());

	int64 maxDistance = 0;
	for (auto s : scanners)
	{
		for (auto s2 : scanners)
		{
			coord3 pos = s.pos;
			pos -= s2.pos;
			int64 length = pos.length();
			if (length > maxDistance)
			{
				maxDistance = length;
			}
		}
	}
	printf("%s: Part 2: %lld\n", filename, maxDistance);
}

int main()
{
	Process("example.txt");
	Process("input.txt");

	return 0;
}
