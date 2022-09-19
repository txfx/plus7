#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace p7 {

template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, N1 + N2> concat(const std::array<T, N1>& array1, const std::array<T, N2>& array2)
{
    std::array<T, N1 + N2> ret { {} };

    int i = 0;
    for (T e : array1)
    {
        ret[i++] = e;
    }
    for (T e : array2)
    {
        ret[i++] = e;
    }
    return ret;
}

} // namespace p7