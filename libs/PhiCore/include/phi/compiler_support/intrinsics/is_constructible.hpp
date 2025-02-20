#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTRUCTIBLE_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTRUCTIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_CONSTRUCTIBLE) && !defined(PHI_SUPPORTS_IS_CONSTRUCTIBLE)
#    if PHI_HAS_INTRINSIC_IS_CONSTRUCTIBLE()
#        define PHI_IS_CONSTRUCTIBLE(...)       __is_constructible(__VA_ARGS__)
#        define PHI_SUPPORTS_IS_CONSTRUCTIBLE() 1
#    else
#        define PHI_IS_CONSTRUCTIBLE(...)       false
#        define PHI_SUPPORTS_IS_CONSTRUCTIBLE() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_CONSTRUCTIBLE
#        define PHI_SUPPORTS_IS_CONSTRUCTIBLE() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTRUCTIBLE_HPP
