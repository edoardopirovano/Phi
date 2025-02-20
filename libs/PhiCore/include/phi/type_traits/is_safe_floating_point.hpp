#ifndef INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_FLOATING_POINT_HPP
#define INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_FLOATING_POINT_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/inline_variables.hpp"
#include "phi/type_traits/bool_constant.hpp"
#include "phi/type_traits/remove_cv.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

template <typename TypeT>
class floating_point;

namespace detail
{
    template <typename TypeT>
    struct is_safe_floating_point_impl : public false_type
    {};

    template <typename TypeT>
    struct is_safe_floating_point_impl<floating_point<TypeT>> : public true_type
    {};
} // namespace detail

template <typename TypeT>
struct is_safe_floating_point : public detail::is_safe_floating_point_impl<remove_cv_t<TypeT>>
{};

template <typename TypeT>
struct is_not_safe_floating_point : public bool_constant<!is_safe_floating_point<TypeT>::value>
{};

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_safe_floating_point_v = is_safe_floating_point<TypeT>::value;

template <typename TypeT>
PHI_INLINE_VARIABLE constexpr bool is_not_safe_floating_point_v =
        is_not_safe_floating_point<TypeT>::value;

#endif

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TYPE_TRAITS_IS_SAFE_FLOATING_POINT_HPP
