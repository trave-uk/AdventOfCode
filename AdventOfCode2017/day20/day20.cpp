// day20.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

class Vector
{
public:
	Vector() = default;
	Vector(const char* input)
	{
		int s = sscanf(input, "%lld,%lld,%lld", &x, &y, &z);
		assert(s==3);
	}

	__int64 Manhattan()
	{
		return abs(x) + abs(y) + abs(z);
	}

	Vector& operator+=(const Vector& o)
	{
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	bool operator==(const Vector& o)
	{
		return x == o.x && y == o.y && z == o.z;
	}

private:
	__int64 x;
	__int64 y;
	__int64 z;
};

class Particle
{
public:
	Particle(const char* input)
	{
		// p=<-1724,-1700,5620>, v=<44,-10,-107>, a=<2,6,-9>
		char p[64], v[64], a[64];
		int s = sscanf(input, "p=<%s v=<%s a=<%s", p, v, a);
		assert(s==3);
		pos = Vector(p);
		vel = Vector(v);
		acc = Vector(a);
	}

	__int64 Manhattan()
	{
		return pos.Manhattan();
	}

	void Tick()
	{
		vel += acc;
		pos += vel;
	}

	bool operator==(const Particle& o)
	{
		return pos == o.pos;
	}

private:
	Vector pos;
	Vector vel;
	Vector acc;
};

std::vector<Particle> particles;

int TickParticles(int part)
{
	int i = 0;
	__int64 low = INT64_MAX;
	int lowest = -1;
	for (Particle& p : particles)
	{
		p.Tick();
		if (part == 1)
		{
			__int64 man = p.Manhattan();
			if (man < low)
			{
				low = man;
				lowest = i;
			}
			else if (man == low)
			{
				lowest = -1;
			}
		}
		++i;
	}
	if (part == 2)
	{
		i = 0;
		std::set<int> remove;
		for (Particle& p : particles)
		{
			for (Particle& q : particles)
			{
				if (&p == &q)
					continue;

				if (p == q)
				{
					remove.insert(i);
					break;
				}
			}
			++i;
		}

		i = 0;
		for (auto iter = particles.begin(); iter != particles.end(); )
		{
			if (remove.count(i) > 0)
			{
				iter = particles.erase(iter);
			}
			else
			{
				++iter;
			}
			++i;
		}
	}
	return lowest;
}

void Load(const char* filename)
{
	particles.clear();
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
				Particle p(thisLine);
				particles.push_back(p);
			}
		}
	}
	fclose(fp);
	delete[] buffer;
}

int main()
{
	const char* filename =
		"input.txt";
		//"example.txt";
		//"example2.txt";
	int result = 0;
	Load(filename);

	int last = -1;
	int count = 0;
	for(;;)
	{
		int lowest = TickParticles(1);
		if ((lowest != -1 && lowest != last) || (count > 0 && (count % 100) == 0))
		{
//			printf("(x%d)\nLowest: %d ", count, lowest);
			if (count == 1000)	// 1000 seems like plenty
			{
				result = last;
				break;
			}
			if (lowest != last)
			{
				last = lowest;
				count = 1;
			}
			else
			{
				++count;
			}
		}
		else if (lowest == last)
		{
			++count;
		}
	}
	printf( "Part 1: %d\n", result );

	Load(filename);
	for (int t=0; t<200; ++t)
	{
		TickParticles(2);
	}
	printf("PArt 2: %zd\n", particles.size());

	return 0;
}
