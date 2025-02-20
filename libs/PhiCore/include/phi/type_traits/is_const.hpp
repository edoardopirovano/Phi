#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_CONST_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_CONST_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/compiler_support/intrinsics/is_const.hpp"
#include "phi/type_traits/bool_constant.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

#if PHI_SUPPORTS_IS_CONST()

template <typename TypeT>
struct is_const : public bool_constant<PHI_IS_CONST(TypeT)>
{};

template <typename TypeT>
struct is_not_const : public bool_constant<!PHI_IS_CONST(TypeT)>
{};

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_const_v = PHI_IS_CONST(TypeT);

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_const_v = !PHI_IS_CONST(TypeT);

#    endif

#else

template <typename TypeT>
struct is_const : public false_type
{};

template <typename TypeT>
struct is_const<const TypeT> : public true_type
{};

template <typename TypeT>
struct is_not_const : public bool_constant<!is_const<TypeT>::value>
{};

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_const_v = is_const<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_const_v = is_not_const<TypeT>::value;

#    endif

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_CONST_HPP
