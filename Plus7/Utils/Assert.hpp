#pragma once

#include <cassert>

#ifndef NDEBUG
#define P7_ASSERT(cond, ...)                 \
    if (!(cond))                             \
    {                                        \
        __assert(#cond, __FILE__, __LINE__); \
    }
#else // NDEBUG
#define P7_ASSERT(cond, ...) (void)(cond)
#endif // NDEBUG
