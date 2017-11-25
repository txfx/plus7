#pragma once

#include <cassert>

#define P7_ASSERT(cond, ...) \
    if (!cond)               \
        __assert(#cond, __FILE__, __LINE__);