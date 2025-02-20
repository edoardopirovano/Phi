#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/type_traits/bool_constant.hpp"
#include "phi/type_traits/is_trivially_constructible.hpp"

#if PHI_HAS_WORKING_IS_TRIVIALLY_CONSTRUCTIBLE()
#    define PHI_HAS_WORKING_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE() 1
#else
#    define PHI_HAS_WORKING_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE() 0
#endif

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename TypeT>
struct is_trivially_default_constructible : public is_trivially_constructible<TypeT>
{};

template <typename TypeT>
struct is_not_trivially_default_constructible
    : public bool_constant<!is_trivially_default_constructible<TypeT>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_trivially_default_constructible_v =
        is_trivially_default_constructible<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_trivially_default_constructible_v =
        is_not_trivially_default_constructible<TypeT>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_TRIVIALLY_DEFAULT_CONSTRUCTIBLE_HPP
