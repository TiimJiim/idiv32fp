//==============================================================================
// Copyright (C) 2022 by Tim Hughes
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
#include "idiv32fp.h"
#include <stdio.h>
#include <assert.h>
#include <random>

using namespace idiv32fp;

static uint idiv32fp_15bit(int n, uint d)
{
    const float magic = 1.0f/float(1u << 30u) + 1.0f/float(1u << 31u);
    float fm = 1.0f/float(d) + magic;
    return int(float(n) * fm);
}

static void test(uint n, uint d)
{
    if (d > 0)
    {
        uint res = udiv32fp(n, d);
        uint ref = n / d;
        if (res != ref)
        {
            fprintf(stderr, "Failed %u/%u\n", n, d);
            exit(-1);
        }
    }
}

int main()
{
    // random tests
    std::mt19937 gen;
    for (uint i = 0; i < ~0u; ++i)
        test(gen(), gen());

	// prove idiv32fp_15bit by exhaustive search
    for (int a = -32768; a <= 32768; ++a)
    {
        for (int b = 1; b <= abs(a); ++b)
        {
            int res = idiv32fp_15bit(a, b);
            int ref = a / b;
            if (res != ref)
            {
                fprintf(stderr, "Failed %u/%u\n", a, b);
                return -1;
            }
        }
    }

    printf("Okay!\n");
    return 0;
}