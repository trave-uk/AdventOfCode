// day21.cpp : Advent of Code 2023 https://adventofcode.com/2023
// Solution by trave.uk 21/12/2023 https://github.com/trave-uk/AdventOfCode
//

#include "stdafx.h"

using Map = std::map<coord, char>;

void WrapCoord(coord& pos, const coord& size)
{
	pos.first %= size.first;
	if (pos.first < 0)
	{
		pos.first += size.first;
	}

	pos.second %= size.second;
	if (pos.second < 0)
	{
		pos.second += size.second;
	}
}

bool OddEven(int64 c, int64 size)
{
	if (c < 0)
	{
		c -= 10;
	}
	int64 index = c / size;
	return (index % 2) == 0;
}

int64 CountPoints(const std::set<coord>& points, const coord& minXY, const coord& maxXY)
{
	int64 count = 0;
	for (const coord& pos : points)
	{
		if (pos.isContained(minXY, maxXY))
			++count;
	}
	return count;
}

void PrintSizes(const Map& map, const std::set<coord> points, const coord& size, const coord& topLeft, const coord& bottomRight)
{
	TermColour odd = TermColour::Cyan;
	TermColour even = TermColour::Yellow;

	assert(topLeft.first == topLeft.second && bottomRight.first == bottomRight.second && size.first == size.second);
	int64 from = topLeft.first;
	if (from < 0)
		from -= size.first;
	from /= size.first;
	int64 to = bottomRight.first / size.first;
	const char* digits = "%03lld";
	if (size.first > 11)
		digits = "%05lld";

	for (int64 y = from; y < to; ++y)
	{
		bool verticalParity = OddEven(y, 1);
		for (int64 x = from; x < to; ++x)
		{
			bool horizontalParity = OddEven(x, 1);

			coord minXY = size;
			minXY.first *= x;
			minXY.second *= y;
			coord maxXY = size;
			maxXY.first *= x;
			maxXY.second *= y;
			maxXY += size;
			maxXY -= coord(1, 1);

			int64 numPoints = CountPoints(points, minXY, maxXY);
			if (verticalParity ^ horizontalParity)
				setcolour(odd);
			else
				setcolour(even);
			printf(digits, numPoints);
		}
		_putch('\n');
	}
	setcolour(TermColour::Default);
}

void PrintMap(const Map& map, const std::set<coord> points, const coord& size, const coord& topLeft, const coord &bottomRight)
{
//	if (bottomRight.first - topLeft.first > 800)
	{
		PrintSizes(map, points, size, topLeft, bottomRight);
		return;
	}
	TermColour odd = TermColour::Cyan;
	TermColour even = TermColour::Yellow;
	coord pos(0, 0);
	bool verticalParity = false;
	bool horizontalParity = false;
	TermColour lastColour = TermColour::White;
	for (pos.second = topLeft.second; pos.second < bottomRight.second; ++pos.second)
	{
		verticalParity = OddEven(pos.second, size.second);
		for (pos.first = topLeft.first; pos.first < bottomRight.first; ++pos.first)
		{
			horizontalParity = OddEven(pos.first, size.first);
			TermColour colour = (verticalParity ^ horizontalParity) ? odd : even;
			if (colour != lastColour)
			{
				setcolour(colour);
				lastColour = colour;
			}
			char c;
			if (points.count(pos) > 0)
			{
				c = 'O';
			}
			else
			{
				coord pos2 = pos;
				WrapCoord(pos2, size);
				c = map.at(pos2);
			}
			_putch(c);
		}
		_putch('\n');
	}
	setcolour(TermColour::Default);
	_putch('\n');
}

bool Passable(const Map& map, const coord& size, const coord& pos)
{
	coord pos2 = pos;
	WrapCoord(pos2, size);
	return map.at(pos2) != '#';
}

int64 CalculatePart1(coord start, const Map& map, const coord& size, int64 maxSteps, std::set<coord>* points = nullptr)
{
	// Flood-fill from start maxSteps times. Every even step, increment total.
	std::map<coord, int64> filled;
	std::set<coord> part1points;
	std::set<coord>* pPoints = points;
	if (!pPoints)
		pPoints = &part1points;
	std::set<std::pair<coord, int64>> test;
	int64 steps = 0;
	test.insert(std::make_pair(start, steps));
	int64 parity = maxSteps % 2;
	coord topLeft(0,0);
	coord bottomRight(size);
	while (!test.empty())
	{
		auto t = test.begin();
		coord pos = t->first;
		steps = t->second;
		test.erase(t);
		if (filled.count(pos) > 0 && filled[pos] <= steps)
			continue;
		filled[pos] = steps;
		if (!points && !pos.isContained(topLeft, bottomRight))
		{
			while (pos.first < topLeft.first) topLeft.first -= size.first;
			while (pos.first > bottomRight.first) bottomRight.first += size.first;
			while (pos.second < topLeft.second) topLeft.second -= size.second;
			while (pos.second > bottomRight.second) bottomRight.second += size.second;
		}
		if ((steps % 2) == parity)
		{
			pPoints->insert(pos);
		}
		++steps;
		if (steps > maxSteps)
			continue;
		for (int d = 0; d < 4; ++d)
		{
			coord delta((d == 0) - (d == 1), (d == 2) - (d == 3));
			delta += pos;
			if (Passable(map, size, delta))
			{
				test.insert(std::make_pair(delta, steps));
			}
		}
	}
	if (points || maxSteps <= 100)
	{
		PrintMap(map, *pPoints, size, topLeft, bottomRight);
	}
	return pPoints->size();
}

// Part 2 helper that calls CalculatePart1 to fill in space from a point, and returns how many points returned were within the actual map and not its echos.
int64 MeasureCentreTile(const coord& start, const Map& map, const coord& size, int64 testSteps)
{
	std::set<coord> points;
	CalculatePart1(start, map, size, testSteps, &points);

	// Count how many of the collect points actually occur within the base map
	coord origin(0, 0);
	coord origout(size.first - 1, size.second - 1);
	int64 result = 0;
	for (const coord& point : points)
	{
		if (point.isContained(origin, origout))
		{
			++result;
		}
	}
	return result;
}

void CheckAssumptions(const coord& start, const coord& size)
{
	// Confirm some suppositions:
	// the map is square: it has the same width and height.
	assert(size.first == size.second);
	// size has odd dimensions
	assert((size.first % 2) == 1);
	// start is in the centre of the base map.
	assert(start.first == start.second);
	assert(start.first == (size.first - 1) / 2);
}

int64 CalculatePart2_First(coord start, Map& map, const coord& size, int64 maxSteps)
{
	CheckAssumptions(start, size);
	// For part 2, the map is tiled infinitely in all directions to account for larger maximum step distances
	// CalculatePart1 has been modified so that it copes with this using "brute force", but for part 2 something cleverer is needed!
	int64 counter = 0;

	// First, calculate how many points are filled in in the base map, i.e. without tiling (but using tiling to get to the edges if necessary, which is probably only needed in the example).
	int64 testSteps = max(size.first, size.second) * 2 + (maxSteps % 2);
	int64 filledTileSize1 = MeasureCentreTile(start, map, size, testSteps);

	// Note that alternate tiles have opposite patterns (owing to the size being odd), so find a space next to the centre, and measure again with that as the centre.
	coord start2;
	for (int d = 0; d < 4; ++d)
	{
		coord delta((d == 0) - (d == 1), (d == 2) - (d == 3));
		delta += start;
		if (Passable(map, size, delta))
		{
			start2 = delta;
			break;
		}
	}
	int64 filledTileSize2 = MeasureCentreTile(start2, map, size, testSteps);

	// So, given the actual maxSteps, what happens in the tiles at the edges? Assume that entirely internal tiles are completely full
	// Move to the top left corner of the base tile and then see how many complete tiles are to the left.
	// Moving to a corner takes 2*(|start|) steps. Note that in the example, moving to the bottom right corner actually takes more steps than this. That's specific to the example though.
	int64 toCorner = 2 * start.first;
	int64 fullTileLength = (maxSteps - toCorner) / size.first;
	int64 remainingStepsFromCorner = (maxSteps - toCorner) % size.first;
	int64 remainingStepsFromEdge = maxSteps - start.first - fullTileLength * size.first;

	// The inner diamond of full tiles is (fullTileLength * 2) + 1 wide and high.
	// The number of these full tiles is (fullTileLength^2 + (fullTileLength + 1)^2), where the higher 
	counter += fullTileLength * fullTileLength * filledTileSize2;
	counter += (fullTileLength + 1) * (fullTileLength + 1) * filledTileSize1;

	// The partially-filled tiles at the edges are of a few types.
	//   __          __              __     __    __  ____   ____  __   ____
	//  /  |        /  |   /\       |  \   |  \  | / |    | |    | \ | |    |
	// /   |       /   |  /  \      |   \  |   \ |/  |    | |    |  \| |    |
	// \   |   /| |    | |    | |\  |    | |   /     |   /   \  /       \   |
	//  \__|  /_| |____| |____| |_\ |____| |__/      |__/     \/         \__|
	//   PL   TUL   SUL    PU   TUR  SUR    PR   TDR  SDR     PD   TDL   SDL
	// There will be one of each of PL, PU, PR, PD
	// There will be (fullTileLength) of each of SUL, SUR, SDR, SDL
	// There will be (fullTileLength+1) of each of TUL, TUR, TDR and TDL
	// 
	// Each of these can be populated by starting from a differnt position.
	// points (PL, PU, PR, PD) are populated by starting from the centre of the side opposite the point.
	// triangles (TUL, TUR, TDR, TDL) are populated by starting from the right-angle corner
	// squares* (SUL, SUR, SDR, SDL) are populated by starting from the opposite corner.
	// *technically these are irregular pentagons, not squares, but "P" was already taken, and the "points" are also irregular pentagons

	// Calculate the number of points within each of these different shapes
	for (int i = 0; i < 4; ++i)
	{
		// Px the start position is one tile past the opposite edge.
		coord offset((i == 0) - (i == 1), (i == 2) - (i == 3));
		offset *= start.first + 1;
		offset += start;
		int64 sizeP = MeasureCentreTile(offset, map, size, remainingStepsFromEdge);
		counter += sizeP;

		// Txy and Sxy the start position is one tile diagonally past one corner
		offset = coord((i == 1) | (i == 3), (i == 2) | (i == 3));
		offset *= size.first + 2;
		offset += coord(-1, -1);
		int64 sizeT = MeasureCentreTile(offset, map, size, remainingStepsFromCorner);
		counter += (fullTileLength + 1) * sizeT;

		int64 sizeS = MeasureCentreTile(offset, map, size, remainingStepsFromCorner + size.first);
		counter += fullTileLength * sizeS;
	}

	return counter;
	// 591449364843137 too low
	// 592723883540864 too low!
}

struct MiniMap : public Map
{
	MiniMap(const Map& origMap, std::set<coord> origPoints, const coord& size, int x, int y)
		: Map(origMap)
		, points()
		, numPoints(0)
		, size(size)
		, x(x)
		, y(y)
	{
		coord minXY(0, 0);
		coord maxXY(size);
		coord offset(size.first * x, size.second * y);
		minXY += offset;
		maxXY += offset;
		maxXY -= coord(1, 1);
		for (const coord& p : origPoints)
		{
			if (p.isContained(minXY, maxXY))
			{
				points.insert(p);
				++numPoints;
			}
		}
	}
	bool Match(const MiniMap& other)
	{
		if (numPoints != other.numPoints)
			return false;
		int dx = other.x - x;
		int dy = other.y - y;
		coord delta(dx * size.first, dy * size.second);
		auto it1 = points.begin();
		auto it2 = other.points.begin();
		for (; it1 != points.end(); ++it1, ++it2)
		{
			coord my = *it1;
			coord cmp = *it2;
			my += delta;
			if (my != cmp)
				return false;
		}
		return true;
	}
	std::set<coord> points;
	int64 numPoints;
	coord size;
	int x;
	int y;
};

int64 CalculatePart2(coord start, Map& map, const coord& size, int64 maxSteps)
{
	CheckAssumptions(start, size);
	// Rather than calculate separately all the different jigsaw pieces, actually just calculate a smaller-than-the-large-number version which encompasses sufficient extra map tiles
	// to allow it to be chopped up and made into jigsaw pieces.

	// A 7x7 map tiles version should be sufficient for enough information for larger sizes, so that is maxSteps between 2.5 and 3.5 times the size.

	int64 testSteps = maxSteps % (size.first * 2);
	testSteps += size.first * 2;
	// Note that the parity of the testSteps needs to match that of maxSteps!
	if ((testSteps % 2) != (maxSteps % 2))
	{
		testSteps += size.first;
	}
	if (testSteps >= maxSteps)
	{
		return CalculatePart1(start, map, size, maxSteps);
	}
	std::set<coord> points;
	if ((testSteps * 2 - 1) > (size.first * 7))
		testSteps -= size.first-1;
	int64 testResult = CalculatePart1(start, map, size, testSteps, &points);

	// Check the extents of the points set
	coord minXY(INT64_MAX, INT64_MAX);
	coord maxXY(INT64_MIN, INT64_MIN);
	for (const coord& p : points)
	{
		if (p.first < minXY.first)
			minXY.first = p.first;
		if (p.first > maxXY.first)
			maxXY.first = p.first;
		if (p.second < minXY.second)
			minXY.second = p.second;
		if (p.second > maxXY.second)
			maxXY.second = p.second;
	}
	// Should be 7x7
	PrintMap(map, points, size, minXY, maxXY);

	// Split points up into the various segments
	// The partially-filled tiles at the edges are of a few types.
	//   __          __              __     __    __  ____   ____  __   ____
	//  /  |        /  |   /\       |  \   |  \  | / |    | |    | \ | |    |
	// /   |       /   |  /  \      |   \  |   \ |/  |    | |    |  \| |    |
	// \   |   /| |    | |    | |\  |    | |   /     |   /   \  /       \   |
	//  \__|  /_| |____| |____| |_\ |____| |__/      |__/     \/         \__|
	//   PL   TUL   SUL    PU   TUR  SUR    PR   TDR  SDR     PD   TDL   SDL
	int64 ftl = (testSteps - start.first) / size.first + 1;

	MiniMap empty(map, points, size, -ftl, -ftl);
	assert(empty.numPoints == 0);
	MiniMap centre(map, points, size, 0, 0);
	MiniMap offCentre(map, points, size, 0, 1);

	MiniMap pointLeft(map, points, size, -ftl, 0);
	MiniMap pointRight(map, points, size, ftl, 0);
	MiniMap pointUp(map, points, size, 0, -ftl);
	MiniMap pointDown(map, points, size, 0, ftl);

	MiniMap pointLeftIn(map, points, size, 1-ftl, 0);
	MiniMap pointRightIn(map, points, size, ftl-1, 0);
	MiniMap pointUpIn(map, points, size, 0, 1-ftl);
	MiniMap pointDownIn(map, points, size, 0, ftl-1);

	MiniMap triangleUpLeft(map, points, size, -ftl, -1);
	MiniMap triangleUpRight(map, points, size, ftl, -1);
	MiniMap triangleDownLeft(map, points, size, -ftl, 1);
	MiniMap triangleDownRight(map, points, size, ftl, 1);

	MiniMap squareUpRight(map, points, size, ftl-1, -1);
	MiniMap squareDownLeft(map, points, size, 1-ftl, 1);
	MiniMap squareDownRight(map, points, size, ftl-1, 1);
	MiniMap squareUpLeft(map, points, size, 1-ftl, -1);

	MiniMap inUpLeft(map, points, size, 2-ftl, 2-ftl);
	MiniMap inUpRight(map, points, size, ftl-2, 2-ftl);
	MiniMap inDownLeft(map, points, size, 2-ftl, ftl-2);
	MiniMap inDownRight(map, points, size, ftl-2, ftl-2);

	if (ftl == 3)
	{
		// Check everything repeats as expected (at least that the centres and the triangles match)
		std::array<std::array<MiniMap*, 7>, 7> expected =
		{{
			{{ &empty, &empty, &triangleUpLeft, &pointUp, &triangleUpRight, &empty, &empty }},
			{{ &empty, &triangleUpLeft, &squareUpLeft, &pointUpIn, &squareUpRight, &triangleUpRight, &empty }},
			{{ &triangleUpLeft, &squareUpLeft, &inUpLeft, &offCentre, &inUpRight, &squareUpRight, &triangleUpRight }},
			{{ &pointLeft, &pointLeftIn, &offCentre, &centre, &offCentre, &pointRightIn, &pointRight }},
			{{ &triangleDownLeft, &squareDownLeft, &inDownLeft, &offCentre, &inDownRight, &squareDownRight, &triangleDownRight }},
			{{ &empty, &triangleDownLeft, &squareDownLeft, &pointDownIn, &squareDownRight, &triangleDownRight, &empty }},
			{{ &empty, &empty, &triangleDownLeft, &pointDown, &triangleDownRight, &empty, &empty }}
		}};
		for (int y = 0; y < 7; ++y)
		{
			for (int x = 0; x < 7; ++x)
			{
				MiniMap test(map, points, size, x - 3, y - 3);
				assert(test.Match(*expected[y][x]));
			}
		}
	}
	// Calculate as in CalculatePart2_First, expanded to include the extra "in" ones

	// +-----+-----+-----+-----+-----+-----+-----+
	// |     |     | TUL | PU  | TUR |     |     |
	// +-----+-----+-----+-----+-----+-----+-----+
	// |     | TUL | SUL | PUI | SUR | TUR |     |
	// +-----+-----+-----+-----+-----+-----+-----+
	// | TUL | SUL | IUL |  OC | IUR | SUR | TUR |
	// +-----+-----+-----+-----+-----+-----+-----+
	// | PL  | PLI | OC  |  C  |  OC | PRI |  PR |
	// +-----+-----+-----+-----+-----+-----+-----+
	// | TDL | SDL | IDL |  OC | IDR | SDR | TDR |
	// +-----+-----+-----+-----+-----+-----+-----+
	// |     | TDL | SDL | PDI | SDR | TDR |     |
	// +-----+-----+-----+-----+-----+-----+-----+
	// |     |     | TDL | PD  | TDR |     |     |
	// +-----+-----+-----+-----+-----+-----+-----+
	// So, "fullTileLength" (ftl) is 3 here (distance from centre to the edges), and there are:
	// (ftl-1)*(ftl-1) OC tiles
	// (ftl-2)*(ftl-2) C tiles
	// (ftl) Tyx tiles
	// (ftl-1) Syx tiles
	// (ftl-2) Iyx tiles
	// 1 Pd tiles
	// 1 PdI tiles
	int64 part2 = 0;
	testSteps += size.first * 2;
	points.clear();
	testResult = CalculatePart1(start, map, size, testSteps, &points);
	coord offset = size;
	ftl = (testSteps - start.first) / size.first + 1;
	offset *= ftl;
	minXY = coord(0, 0);
	minXY -= offset;
	maxXY = size;
	maxXY += offset;
	PrintMap(map, points, size, minXY, maxXY);
	for (int i = 0; i < 2; ++i)
	{
		int64 steps = (i == 0) ? testSteps : maxSteps;

		ftl = (steps - start.first) / size.first + 1;
		part2 = 0;
		if ((ftl % 2) == 0)
		{
			part2 += (ftl - 1) * (ftl - 1) * centre.numPoints;
			part2 += (ftl - 2) * (ftl - 2) * offCentre.numPoints;
		}
		else
		{
			part2 += (ftl - 1) * (ftl - 1) * offCentre.numPoints;
			part2 += (ftl - 2) * (ftl - 2) * centre.numPoints;
		}
		part2 += ftl * triangleUpLeft.numPoints;
		part2 += ftl * triangleDownLeft.numPoints;
		part2 += ftl * triangleUpRight.numPoints;
		part2 += ftl * triangleDownRight.numPoints;
		part2 += (ftl - 1) * squareUpLeft.numPoints;
		part2 += (ftl - 1) * squareDownLeft.numPoints;
		part2 += (ftl - 1) * squareUpRight.numPoints;
		part2 += (ftl - 1) * squareDownRight.numPoints;
		part2 += (ftl - 2) * inUpLeft.numPoints;
		part2 += (ftl - 2) * inDownLeft.numPoints;
		part2 += (ftl - 2) * inUpRight.numPoints;
		part2 += (ftl - 2) * inDownRight.numPoints;
		part2 += pointUp.numPoints + pointUpIn.numPoints;
		part2 += pointDown.numPoints + pointDownIn.numPoints;
		part2 += pointLeft.numPoints + pointLeftIn.numPoints;
		part2 += pointRight.numPoints + pointRightIn.numPoints;
		if (i == 0 && maxSteps > 1000000)
		{
			assert(part2 == testResult);
		}
	}
	return part2;
}

void Process(const char* filename, int64 part1steps, int64 part2steps, int64 expectedPart1 = -1, int64 expectedPart2 = -1)
{
	char* buffer = new char[65536];
	FILE* fp = fopen(filename, "rt");
	int64 part1 = 0;
	int64 part2 = 0;
	Map map;
	coord pos(0, 0);
	coord start(0, 0);
	while (!feof(fp))
	{
		char* thisLine = fgets(buffer, 65536, fp);
		if (thisLine)
		{
			thisLine[strcspn(thisLine, "\n\r")] = '\0';
			if (*thisLine)
			{
				pos.first = 0;
				std::string line(thisLine);
				for (char c : line)
				{
					map[pos] = c;
					if (c == 'S')
						start = pos;
					++pos.first;
				}
				++pos.second;
			}
		}
	}
	fclose(fp);
	delete[] buffer;
	coord size = pos;

	Map part1map = map;
	part1 = CalculatePart2(start, part1map, size, part1steps);

	assert(expectedPart1 == -1 || expectedPart1 == part1);
	printf("%s (%lld): Part 1: %lld\n", filename, part1steps, part1);

	Map part2map = map;
	part2 = CalculatePart2(start, part2map, size, part2steps);

	assert(expectedPart2 == -1 || expectedPart2 == part2);
	printf("%s (%lld): Part 2: %lld\n", filename, part2steps, part2);
}

int main()
{
	Process("example.txt", 6, 10, 16, 50);
	Process("example.txt", 50, 100, -1, -1);//1594, 6536);
	Process("example.txt", 500, 1000, -1, -1);//167004, 668697);
	Process("example.txt", 6, 5000, 16, -1);//16733044);
	Process("input.txt", 64, 26501365, 3562);

	return 0;
}

// part 1: 2835 too low
// part 2: 591449364843137 too low
// part 2: 592723883540864 too low
//         592723929260582
// part 2: 592729846565092 too high
