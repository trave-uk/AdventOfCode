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
