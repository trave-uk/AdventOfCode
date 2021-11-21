#pragma once

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
#include <wincon.h>
#include <conio.h>

#define _ARRAYSIZE_(x) (sizeof(x)/sizeof(*x))

#include "crc.h"
#include "md5.h"

using int64 = __int64;