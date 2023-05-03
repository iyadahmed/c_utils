// Reference: https://www.state-machine.com/fast-deterministic-and-portable-clz

#pragma once

#include <stdint.h>

#if defined(__IAR_SYSTEMS_ICC__)
#define clz(x) __CLZ(x)

#elif defined(__KEIL__)
#define clz(x) __clz(x)

#elif defined(__GNUC__)
#define clz(x) __builtin_clz(x)

#elif defined(__XC32)
#define clz(x) _clz(x)

#elif defined(__XC16_VERSION__)
#define clz(x) __builtin_fbcl(x)

#elif defined(_MSC_VER)
#define clz(x) __lzcnt(x)

#else
// Fallback to software implementation
#define clz(x) CLZ1(x)
#endif

static inline uint32_t CLZ1(uint32_t x)
{
    static uint8_t const clz_lkup[] = {
        32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
        28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
        27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
        27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U};
    uint32_t n;
    if (x >= (1U << 16))
    {
        if (x >= (1U << 24))
        {
            n = 24U;
        }
        else
        {
            n = 16U;
        }
    }
    else
    {
        if (x >= (1U << 8))
        {
            n = 8U;
        }
        else
        {
            n = 0U;
        }
    }
    return (uint32_t)clz_lkup[x >> n] - n;
}

static inline uint32_t CLZ2(uint32_t x)
{
    static uint8_t const clz_lkup[] = {
        32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
        28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U};
    uint32_t n;

    if (x >= (1U << 16))
    {
        if (x >= (1U << 24))
        {
            if (x >= (1 << 28))
            {
                n = 28U;
            }
            else
            {
                n = 24U;
            }
        }
        else
        {
            if (x >= (1U << 20))
            {
                n = 20U;
            }
            else
            {
                n = 16U;
            }
        }
    }
    else
    {
        if (x >= (1U << 8))
        {
            if (x >= (1U << 12))
            {
                n = 12U;
            }
            else
            {
                n = 8U;
            }
        }
        else
        {
            if (x >= (1U << 4))
            {
                n = 4U;
            }
            else
            {
                n = 0U;
            }
        }
    }
    return (uint32_t)clz_lkup[x >> n] - n;
}
