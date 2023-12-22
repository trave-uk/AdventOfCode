// Mathematical helpers

using int64 = __int64;
#ifdef __clang__
using int128 = __int128;
#endif

// From https://en.wikipedia.org/wiki/Integer_square_root
inline int64 integer_sqrt(int64 n)
{
	assert(n >= 0); // sqrt works for only non-negative inputs
	if (n < 2) return n;

	// Find the shift amount.See also [[find first set]],
	// shift = ceil(log2(n) * 0.5) * 2 = ceil(ffs(n) * 0.5) * 2
	int64 shift = 2;
	while ((n >> shift) != 0)
	{
		shift += 2;
	}

	// Unroll the bit-setting loop.
	int64 result = 0;
	while (shift >= 0)
	{
		result = result << 1;
		int64 large_cand = result + 1; // Same as result ^ 1 (xor), because the last bit is always 0.
		if (large_cand * large_cand <= n >> shift)
		{
			result = large_cand;
		}
		shift -= 2;
	}
	return result;
}

inline bool is_square(int64 n)
{
	int64 isqrt = integer_sqrt(n);
	return (n == isqrt * isqrt);
}

// From https://en.wikipedia.org/wiki/Euclidean_algorithm
inline int64 gcd(int64 a, int64 b)
{
	int64 t;
	while (b != 0)
	{
		t = b;
		b = a % b;
		a = t;
	}
	return a;
}

struct coord3 : public std::tuple<int64, int64, int64>
{
	coord3(int64 x = 0, int64 y = 0, int64 z = 0)
	{
		get<0>() = x;
		get<1>() = y;
		get<2>() = z;
	}

	coord3(char* input)
	{
		char* x = strtok(input, ",");
		get<0>() = atoll(x);
		char* y = strtok(nullptr, ",");
		get<1>() = atoll(y);
		char* z = strtok(nullptr, ",");
		get<2>() = atoll(z);
	}

	template <std::size_t I>
	int64 get() const
	{
		return std::get<I>(static_cast<coord3 const&>(*this));
	}

	template <std::size_t I>
	int64& get()
	{
		return std::get<I>(static_cast<coord3&>(*this));
	}

	bool operator<(const coord3& other) const
	{
		if (get<2>() < other.get<2>())
			return true;
		else if (get<2>() > other.get<2>())
			return false;
		else if (get<0>() < other.get<0>())
			return true;
		else if (get<0>() > other.get<0>())
			return false;
		else if (get<1>() < other.get<1>())
			return true;
		assert(get<1>() >= other.get<1>());
		return false;
	}

	bool operator>(const coord3& other) const
	{
		if (get<2>() > other.get<2>())
			return true;
		else if (get<2>() < other.get<2>())
			return false;
		else if (get<0>() > other.get<0>())
			return true;
		else if (get<0>() < other.get<0>())
			return false;
		else if (get<1>() > other.get<1>())
			return true;
		assert(get<1>() <= other.get<1>());
		return false;
	}

	bool operator<=(const coord3& other) const
	{
		return *this < other || *this == other;
	}

	bool operator>=(const coord3& other) const
	{
		return *this > other || *this == other;
	}

	void operator-=(const coord3& other)
	{
		get<0>() -= other.get<0>();
		get<1>() -= other.get<1>();
		get<2>() -= other.get<2>();
	}

	void operator+=(const coord3& other)
	{
		get<0>() += other.get<0>();
		get<1>() += other.get<1>();
		get<2>() += other.get<2>();
	}

	void operator/=(int64 d)
	{
		get<0>() /= d;
		get<1>() /= d;
		get<2>() /= d;
	}

	int64 Length() const
	{
		return max(max(get<0>(), get<1>()), get<2>());
	}
};
