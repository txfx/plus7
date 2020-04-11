#pragma once

#include <cassert>

#ifndef NDEBUG
#define P7_ASSERT(cond, ...) \
    assert(#cond);
#else // NDEBUG
#define P7_ASSERT(cond, ...) (void)(cond)
#endif // NDEBUG
