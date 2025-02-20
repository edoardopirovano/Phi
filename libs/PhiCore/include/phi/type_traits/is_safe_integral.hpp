#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_INTEGRAL_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_INTEGRAL_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/generated/compiler_support/features.hpp"
#include "phi/type_traits/bool_constant.hpp"
#include "phi/type_traits/is_safe_type.hpp"
#include "phi/type_traits/is_unsafe_integral.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

namespace detail
{
    template <typename TypeT, bool = is_safe_type<TypeT>::value>
    struct is_safe_integral_impl : public is_unsafe_integral<typename TypeT::value_type>
    {};

    template <typename TypeT>
    struct is_safe_integral_impl<TypeT, false> : public false_type
    {};
} // namespace detail

template <typename TypeT>
struct is_safe_integral : public detail::is_safe_integral_impl<TypeT, is_safe_type<TypeT>::value>
{};

template <typename TypeT>
struct is_not_safe_integral : public bool_constant<!is_safe_integral<TypeT>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_safe_integral_v = is_safe_integral<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_safe_integral_v = is_not_safe_integral<TypeT>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_INTEGRAL_HPP
