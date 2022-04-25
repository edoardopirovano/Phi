#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_NONE_OF_RREF_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_NONE_OF_RREF_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/type_traits/integral_constant.hpp"
#include "phi/type_traits/is_same_rref.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename CheckT, typename FirstT, typename... RestT>
struct is_none_of_rref;

template <typename CheckT, typename FirstT>
struct is_none_of_rref<CheckT, FirstT> : public is_not_same_rref<CheckT, FirstT>
{};

template <typename CheckT, typename FirstT, typename SecondT, typename... RestT>
struct is_none_of_rref<CheckT, FirstT, SecondT, RestT...>
    : public bool_constant<is_same_rref<CheckT, FirstT>::value ?
                                   false :
                                   is_none_of_rref<CheckT, SecondT, RestT...>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename CheckT, typename FirstT, typename... RestT>
PHI_INLINE_VARIABLE constexpr bool is_none_of_rref_v =
        is_none_of_rref<CheckT, FirstT, RestT...>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_NONE_OF_RCVREF_HPP
