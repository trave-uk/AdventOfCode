#pragma once

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <assert.h>
#include <basetsd.h>
#include <corecrt_math_defines.h>
#include <corecrt_malloc.h>
#include <set>
#include <map>
#include <array>
#include <algorithm>
#include <vector>
#include <stack>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <deque>
#include <bitset>

#include <windows.h>
#include <processenv.h>
#include <winnt.h>

#define _ARRAYSIZE_(x) (sizeof(x)/sizeof(*x))

#include "crc.h"
#include "md5.h"
#include "astar.h"
#include "term.h"

using int64 = __int64;

// From https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val)
{
	return (T(0) < val) - (val < T(0));
}

// From https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
static inline int64 GetTicks()
{
    LARGE_INTEGER ticks;
    if (!QueryPerformanceCounter(&ticks))
    {
        assert(false);
    }
    return ticks.QuadPart;
}

static inline double GetMilliseconds()
{
    static double frequency = 0;
    if (!frequency)
    {
        LARGE_INTEGER freq;
        if (!QueryPerformanceFrequency(&freq))
        {
            assert(false);
        }
        frequency = freq.QuadPart / 1000.0;
    }

    int64 currentTick = GetTicks();
    return static_cast<double>(currentTick) / frequency;
}

