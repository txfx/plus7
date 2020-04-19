#pragma once

#include <array>
#include <cstddef>
#include <utility>

namespace p7 {

template <typename T, std::size_t N1, std::size_t N2, std::size_t... IDs>
constexpr auto concat(
    const std::array<T, N1>& array1,
    const std::array<T, N2>& array2,
    std::integer_sequence<std::size_t, IDs...>)
{
    constexpr auto get = [](const auto& a1, const auto& a2, auto pos) { return pos < N1 ? a1[pos] : a2[pos - N1]; };
    return std::array<T, N1 + N2> { get(array1, array2, IDs)... };
}

template <typename T, std::size_t N1, std::size_t N2>
constexpr std::array<T, N1 + N2> concat(const std::array<T, N1>& array1, const std::array<T, N2>& array2)
{
    return concat(array1, array2, std::make_integer_sequence<std::size_t, N1 + N2> {});
}

} // namespace p7