#ifndef INCG_PHI_CORE_MATH_COMMON_HPP
#define INCG_PHI_CORE_MATH_COMMON_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline.hpp"
#include <limits>

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename TypeT>
constexpr PHI_ALWAYS_INLINE TypeT square(const TypeT val) noexcept
{
    return val * val;
}

template <typename TypeT>
constexpr PHI_ALWAYS_INLINE TypeT cube(const TypeT val) noexcept
{
    return val * val * val;
}

// Credits: https://gist.github.com/alexshtf/eb5128b3e3e143187794
/// \cond detail
namespace detail
{
    template <typename TypeT>
    constexpr TypeT sqrtNewtonRaphson(const TypeT x, const TypeT curr, const TypeT prev) noexcept
    {
        return curr == prev ? curr : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }
} // namespace detail
/// \endcond

/*!
 * \brief Constexpr version of the square root function
 *
 * \param x Value to calculate the square root of.
 *
 * \return For a finite and non-negative value of "x", returns an approximation for the square root of "x"
 *         Otherwise, returns NaN.
 *
**/
template <typename TypeT>
constexpr TypeT sqrt(const TypeT x) noexcept
{
    return x >= 0 && x < std::numeric_limits<TypeT>::infinity() ?
                   detail::sqrtNewtonRaphson(x, x, 0) :
                   std::numeric_limits<TypeT>::quiet_NaN();
}

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_MATH_COMMON_HPP
