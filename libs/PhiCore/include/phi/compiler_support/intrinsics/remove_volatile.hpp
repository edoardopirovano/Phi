#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_VOLATILE_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_VOLATILE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_REMOVE_VOLATILE) && !defined(PHI_SUPPORTS_REMOVE_VOLATILE)
#    if PHI_HAS_INTRINSIC_REMOVE_VOLATILE()
#        define PHI_REMOVE_VOLATILE(type)      __remove_volatile(type)
#        define PHI_SUPPORTS_REMOVE_VOLATILE() 1
#    else
#        define PHI_REMOVE_VOLATILE(type)      false
#        define PHI_SUPPORTS_REMOVE_VOLATILE() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_REMOVE_VOLATILE
#        define PHI_SUPPORTS_REMOVE_VOLATILE() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_VOLATILE_HPP
