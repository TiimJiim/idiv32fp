//==============================================================================
// Copyright (c) 2022 Tim Hughes
//
// Permission to use, copy, modify, and /or distribute this software for any
// purpose with or without fee is hereby granted.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS.IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.
#if defined(__cplusplus)
#pragma once
#include <stdint.h>
#include <assert.h>
#include <algorithm>

namespace idiv32fp {
typedef unsigned uint;
static_assert(sizeof(int) == sizeof(int32_t), "");
static_assert(sizeof(uint) == sizeof(uint32_t), "");
using std::min;
using std::max;
#define IDIV32FP_INLINE inline
#else
#define IDIV32FP_INLINE
#define assert(x)
#endif

IDIV32FP_INLINE uint udiv32fp(uint n, uint d)
{
	// Ensure q (estimate) is less than n/d. Max positive error in qf is < 1024/d so this is conservative.
	float fn = max(float(n) - 16384.0f, 0.0f);
	float fm = 1.0f / float(d);
	float fq = fn * fm;
	uint q = uint(fq);
	uint r = n - q*d;
	// Max error in r is less than d + 1024/d.
	if (r >= d)
	{
		r -= d;
		q += 1u;
		// Magic constant added to fm makes the fp32 divide rounded down exact for all r in [0,32768]
		const float magic = 1.0f / float(1u << 30u)
						  + 1.0f / float(1u << 31u);
		q += uint(float(r) * (fm + magic));
	}
	return q;
}

#if defined(__cplusplus)
}
#endif