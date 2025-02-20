#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_COMPLETE_OR_UNBOUNDED_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_COMPLETE_OR_UNBOUNDED_HPP

#include "phi/phi_config.hpp"
#include <type_traits>

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/type_traits/bool_constant.hpp"
#include "phi/type_traits/is_complete.hpp"
#include "phi/type_traits/is_unbounded_array.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename TypeT>
struct is_complete_or_unbounded
    : public bool_constant<is_complete<TypeT>::value || is_unbounded_array<TypeT>::value>
{};

template <typename TypeT>
struct is_not_complete_or_unbounded : public bool_constant<!is_complete_or_unbounded<TypeT>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_complete_or_unbounded_v =
        is_complete_or_unbounded<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_complete_or_unbounded_v =
        is_not_complete_or_unbounded<TypeT>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_COMPLETE_OR_UNBOUNDED_HPP
