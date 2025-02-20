#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/type_traits/add_const.hpp"
#include "phi/type_traits/add_lvalue_reference.hpp"
#include "phi/type_traits/bool_constant.hpp"
#include "phi/type_traits/is_constructible.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

#define PHI_HAS_WORKING_IS_COPY_CONSTRUCTIBLE() PHI_HAS_WORKING_IS_CONSTRUCTIBLE()

template <typename TypeT>
struct is_copy_constructible
    : public is_constructible<TypeT, add_lvalue_reference_t<add_const_t<TypeT>>>
{};

template <typename TypeT>
struct is_not_copy_constructible : public bool_constant<!is_copy_constructible<TypeT>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_copy_constructible_v = is_copy_constructible<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_copy_constructible_v =
        is_not_copy_constructible<TypeT>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_COPY_CONSTRUCTIBLE_HPP
