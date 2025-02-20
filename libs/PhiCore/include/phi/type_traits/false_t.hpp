#ifndef INCG_PHI_CORE_TYPE_TRAITS_FALSE_T_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_FALSE_T_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/type_traits/bool_constant.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename... TypesT>
struct false_t : public bool_constant<false>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename... TypesT>
constexpr PHI_INLINE_VARIABLE bool false_v = false;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_FALSE_T_HPP
