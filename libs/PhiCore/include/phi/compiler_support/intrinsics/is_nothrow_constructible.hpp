#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_CONSTRUCTIBLE_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_CONSTRUCTIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_NOTHROW_CONSTRUCTIBLE) && !defined(PHI_SUPPORTS_IS_NOTHROW_CONSTRUCTIBLE)
#    if PHI_HAS_INTRINSIC_IS_NOTHROW_CONSTRUCTIBLE()
#        define PHI_IS_NOTHROW_CONSTRUCTIBLE(...)       __is_nothrow_constructible(__VA_ARGS__)
#        define PHI_SUPPORTS_IS_NOTHROW_CONSTRUCTIBLE() 1
#    else
#        define PHI_IS_NOTHROW_CONSTRUCTIBLE(...)       false
#        define PHI_SUPPORTS_IS_NOTHROW_CONSTRUCTIBLE() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_NOTHROW_CONSTRUCTIBLE
#        define PHI_SUPPORTS_IS_NOTHROW_CONSTRUCTIBLE() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_CONSTRUCTIBLE_HPP
